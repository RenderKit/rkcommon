// Copyright 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#ifdef __linux__
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "rkcommon/common.h"

namespace rkcommon {
namespace tracing {

enum class EventType
{
  INVALID,
  BEGIN,
  END,
  MARKER,
  COUNTER
};

struct RKCOMMON_INTERFACE TraceEvent
{
  EventType type = EventType::INVALID;
  std::string name;
#ifdef __linux__
  rusage usage;
#endif
  std::chrono::steady_clock::time_point time;

  uint64_t counterValue;

  TraceEvent();

  TraceEvent(const EventType type);

  TraceEvent(const EventType type, const std::string &name);

  TraceEvent(const EventType type,
      const std::string &name,
      const uint64_t counterValue);
};

struct RKCOMMON_INTERFACE ThreadEventList
{
  std::vector<TraceEvent> events;
  std::string threadName;

  void beginEvent(const char *name);

  void endEvent();

  void setMarker(const char *name);

  void setCounter(const char *name, const uint64_t value);
};

class RKCOMMON_INTERFACE TraceRecorder
{
  std::unordered_map<std::thread::id, std::shared_ptr<ThreadEventList>>
      threadTrace;

  std::mutex threadTraceMutex;

 public:
  /* Get the thread trace list, creating it if this is the first time
   * this thread has requested its list. This call locks the TraceRecorder,
   * so threads cache the returned value in thread_local storage to avoid
   * calling this each event.
   */
  std::shared_ptr<ThreadEventList> getThreadTraceList(
      const std::thread::id &id);

  void saveLog(const char *logFile, const char *processName);
};

float cpuUtilization(const TraceEvent &start, const TraceEvent &end);

std::string getProcStatus();

} // namespace tracing
} // namespace rkcommon

// TODO: I think having rkcommon always build the code but the client app
// set the defines should work fine?
#ifdef ENABLE_PROFILING

// Top-level C API for tracing, all functions are #define'd to nothing
// when tracing is disabled
void rkTraceBeginEvent(const char *name);

void rkTraceEndEvent();

void rkTraceSetMarker(const char *name);

// Counter values are displayed per-process by chrome:://tracing
// but are recorded per-thread without synchronization
void rkTraceSetCounter(const char *name, uint64_t value);

// Record the built-in counters traceVirtMem and traceRssMem tracking the
// virtual and resident memory sizes respectively
void rkTraceRecordMemUse();

void rkTraceSetThreadName(const char *name);

void rkTraceSaveLog(const char *logFile, const char *processName);

#else

#define rkTraceBeginEvent(X)
#define rkTraceEndEvent()
#define rkTraceSetMarker(X)
#define rkTraceSetThreadName(X)
#define rkTraceSetCounter(X, Y)
#define rkTraceRecordMemUse()
#define rkTraceSaveLog(X, Y)

#endif
