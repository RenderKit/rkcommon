// Copyright 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>

#ifdef _WIN32
#include <process.h>
#endif

// We force the define on here to build the right header
// at compile time, but apps that build with profiling off
// will see the empty defines
#define ENABLE_PROFILING
#include "Tracing.h"

namespace rkcommon {
namespace tracing {

using namespace std::chrono;

static std::unique_ptr<TraceRecorder> traceRecorder =
    rkcommon::make_unique<TraceRecorder>();

static thread_local std::shared_ptr<ThreadEventList> threadEventList = nullptr;

std::ostream &operator<<(std::ostream &os, const EventType &ty)
{
  switch (ty) {
  case EventType::INVALID:
    os << "INVALID";
    break;
  case EventType::BEGIN:
    os << "B";
    break;
  case EventType::END:
    os << "E";
    break;
  case EventType::MARKER:
    os << "i";
    break;
  case EventType::COUNTER:
    os << "C";
    break;
  default:
    break;
  }
  return os;
}

TraceEvent::TraceEvent()
{
#ifdef __linux__
  std::memset(&usage, 0, sizeof(rusage));
  getrusage(RUSAGE_SELF, &usage);
#endif
  time = steady_clock::now();
}

TraceEvent::TraceEvent(const EventType ty) : TraceEvent()
{
  type = ty;
}

TraceEvent::TraceEvent(const EventType type, const std::string &n)
    : TraceEvent(type)
{
  name = n;
}

TraceEvent::TraceEvent(
    const EventType type, const std::string &name, const uint64_t value)
    : TraceEvent(type, name)
{
  counterValue = value;
}

void ThreadEventList::beginEvent(const char *name)
{
  events.push_back(TraceEvent(EventType::BEGIN, name));
}

void ThreadEventList::endEvent()
{
  events.push_back(TraceEvent(EventType::END));
}

void ThreadEventList::setMarker(const char *name)
{
  events.push_back(TraceEvent(EventType::MARKER, name));
}

void ThreadEventList::setCounter(const char *name, const uint64_t counterValue)
{
  events.push_back(TraceEvent(EventType::COUNTER, name, counterValue));
}

std::shared_ptr<ThreadEventList> TraceRecorder::getThreadTraceList(
    const std::thread::id &id)
{
  std::lock_guard<std::mutex> lock(threadTraceMutex);
  auto fnd = threadTrace.find(id);
  if (fnd == threadTrace.end()) {
    auto threadEventList = std::make_shared<ThreadEventList>();
    threadTrace[id] = threadEventList;
    return threadEventList;
  }
  return fnd->second;
}

void TraceRecorder::saveLog(const char *logFile, const char *processName)
{
  std::lock_guard<std::mutex> lock(threadTraceMutex);

  // chrome:://tracing / ui.perfetto.dev takes a JSON array of events, but to
  // keep dependencies down we don't need a JSON library to produce this simple
  // format
  std::ofstream fout(logFile);

#ifdef _WIN32
  const int pid = _getpid();
#else
  const int pid = getpid();
#endif

  fout << "[";

  // Emit metadata about the process name
  if (processName) {
    // Emit metadata event for the thread's ID/name
    fout << "{"
         << "\"ph\": \"M\","
         << "\"pid\":" << pid << ","
         << "\"tid\":" << 0 << ","
         << "\"name\":"
         << "\"process_name\","
         << "\"args\":{\"name\":\"" << processName << "\"}"
         << "},";
  }

  // Go through each thread and output its data
  // We renumber thread IDs here because chrome:://tracing UI doesn't display
  // the true thread ID numbers well
  int nextTid = 0;
  for (const auto &trace : threadTrace) {
    const std::thread::id tid = trace.first;

    // Emit metadata event for the thread's ID/name
    fout << "{"
         << "\"ph\": \"M\","
         << "\"pid\":" << pid << ","
         << "\"tid\":" << nextTid << ","
         << "\"name\":"
         << "\"thread_name\","
         << "\"args\":{\"name\":\"";
    if (!trace.second->threadName.empty()) {
      fout << trace.second->threadName << "\"}";
    } else {
      fout << tid << "\"}";
    }
    fout << "},";

    // Track the begin events so that when we hit an end we can compute CPU %
    // and other stats to include
    std::stack<const TraceEvent *> beginEvents;
    for (const auto &evt : trace.second->events) {
      if (evt.type == EventType::INVALID) {
        std::cerr << "Got invalid event type!?\n";
      }
      if (evt.type == EventType::BEGIN) {
        beginEvents.push(&evt);
      }
      if (evt.type == EventType::END && beginEvents.empty()) {
        std::cerr << "Tracing Error: Too many rkTraceEndEvent calls!\n";
        break;
      }

      const uint64_t timestamp =
          std::chrono::duration_cast<std::chrono::microseconds>(
              evt.time.time_since_epoch())
              .count();

      fout << "{"
           << "\"ph\": \"" << evt.type << "\","
           << "\"pid\":" << pid << ","
           << "\"tid\":" << nextTid << ","
           << "\"ts\":" << timestamp << ","
           << "\"name\":\"" << evt.name << "\","
           << "\"cat\":\"perf\"";

      // Compute CPU utilization % over the begin/end interval for end events
      float utilization = 0.f;
      uint64_t duration = 0;
      if (evt.type == EventType::END) {
        const TraceEvent *begin = beginEvents.top();
        utilization = cpuUtilization(*begin, evt);
        duration = std::chrono::duration_cast<std::chrono::microseconds>(
            evt.time - begin->time)
                       .count();

        fout << ", \"args\":{\"cpuUtilization\":" << utilization << "}";

        beginEvents.pop();
      } else if (evt.type == EventType::COUNTER) {
        fout << ", \"args\":{\"value\":" << evt.counterValue << "}";
      }
      fout << "},";

      // For each end event also emit an update of the CPU % utilization
      // counter for events that were long enough to reasonably measure
      // utilization
      if (evt.type == EventType::END && duration > 100) {
        fout << "{"
             << "\"ph\": \"C\","
             << "\"pid\":" << pid << ","
             << "\"tid\":" << nextTid << ","
             << "\"ts\":" << timestamp << ","
             << "\"name\":\"cpuUtilization\","
             << "\"cat\":\"perf\","
             << "\"args\":{\"value\":" << utilization << "}},";
      }
    }
    if (!beginEvents.empty()) {
      std::cerr << "Tracing Error: Missing end for some events!\n";
      while (!beginEvents.empty()) {
        std::cerr << "\t" << beginEvents.top()->name << "\n";
        beginEvents.pop();
      }
    }
    ++nextTid;
  }
  // We need to remove the last , we output to ensure the JSON array is correct
  // Overwrite it with the ] character.
  fout.seekp(-1, std::ios::cur);
  fout << "]";
}

float cpuUtilization(const TraceEvent &start, const TraceEvent &end)
{
#ifdef __linux__
  const double elapsed_cpu = end.usage.ru_utime.tv_sec
      + end.usage.ru_stime.tv_sec
      - (start.usage.ru_utime.tv_sec + start.usage.ru_stime.tv_sec)
      + 1e-6f
          * (end.usage.ru_utime.tv_usec + end.usage.ru_stime.tv_usec
              - (start.usage.ru_utime.tv_usec + start.usage.ru_stime.tv_usec));

  const double elapsed_wall =
      duration_cast<duration<double>>(end.time - start.time).count();
  return elapsed_cpu / elapsed_wall * 100.0;
#else
  return -1.f;
#endif
}

std::string getProcStatus()
{
  // Note: this file doesn't exist on OS X, would we want some alternative to
  // fetch this info?
  std::ifstream file("/proc/self/status");
  if (!file.is_open()) {
    return "";
  }
  return std::string(
      std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void getProcMemUse(uint64_t &virtMem, uint64_t &resMem)
{
  virtMem = 0;
  resMem = 0;
#ifdef __linux__
  // TODO: Windows?
  FILE *file = std::fopen("/proc/self/statm", "r");
  if (file) {
    // These values are measured in pages
    std::fscanf(file, "%lu %lu", &virtMem, &resMem);
    const int pageSize = getpagesize();
    virtMem *= pageSize;
    resMem *= pageSize;
  }
  std::fclose(file);
#endif
}

void initThreadEventList()
{
  if (!threadEventList) {
    threadEventList =
        traceRecorder->getThreadTraceList(std::this_thread::get_id());
  }
}

} // namespace tracing
} // namespace rkcommon

void rkTraceBeginEvent(const char *name)
{
  using namespace rkcommon::tracing;
  initThreadEventList();
  threadEventList->beginEvent(name);
}

void rkTraceEndEvent()
{
  using namespace rkcommon::tracing;
  // Begin takes care of getting the threadEventList set
  // in thread_local storage so we can assume it exists here
  threadEventList->endEvent();
}

void rkTraceSetMarker(const char *name)
{
  using namespace rkcommon::tracing;
  initThreadEventList();
  threadEventList->setMarker(name);
}

void rkTraceSetCounter(const char *name, uint64_t value)
{
  using namespace rkcommon::tracing;
  initThreadEventList();
  threadEventList->setCounter(name, value);
}

void rkTraceRecordMemUse()
{
  using namespace rkcommon::tracing;
  initThreadEventList();
  uint64_t virtMem = 0;
  uint64_t resMem = 0;
  getProcMemUse(virtMem, resMem);

  threadEventList->setCounter("rkTraceVirtMem_B", virtMem);
  threadEventList->setCounter("rkTraceRssMem_B", resMem);
}

void rkTraceSetThreadName(const char *name)
{
  using namespace rkcommon::tracing;
  initThreadEventList();
  threadEventList->threadName = name;
}

void rkTraceSaveLog(const char *logFile, const char *processName)
{
  using namespace rkcommon::tracing;
  traceRecorder->saveLog(logFile, processName);
}
