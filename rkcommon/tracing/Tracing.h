// Copyright 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <cstdint>
#include <list>
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
  // Refers to a string in the thread's stringCache, nullptr for end events
  const char *name = nullptr;
  // Refers to the event category in the thread's stringCache, may be null
  const char *category = nullptr;
#ifdef __linux__
  timeval ru_utime;
  timeval ru_stime;
#endif
  std::chrono::steady_clock::time_point time;

  uint64_t counterValue = 0;

  TraceEvent();

  TraceEvent(const EventType type);

  TraceEvent(const EventType type, const char *name, const char *category);

  TraceEvent(
      const EventType type, const char *name, const uint64_t counterValue);
};

struct RKCOMMON_INTERFACE ThreadEventList
{
  // We store events in chunks to reduce memory copy
  // costs when when tracking very large numbers of events
  std::list<std::vector<TraceEvent>> events;
  std::string threadName;
  // Applications are typically running a rendering loop, emitting
  // the same event name repeatedly. If these names are inline
  // strings they will have the same pointer and we can cache
  // them in a map to reduce string copying costs and overhead
  // Note: the string is wrapped in a shared/unique ptr
  // to guard against copy ctor use when adding to the map which would
  // invalidate the pointer to the string data
  std::unordered_map<const char *, std::shared_ptr<std::string>> stringCache;

  void beginEvent(const char *name, const char *category);

  void endEvent();

  void setMarker(const char *name, const char *category);

  void setCounter(const char *name, const uint64_t value);

 private:
  std::vector<TraceEvent> &getCurrentEventList();

  const char *getCachedString(const char *str);
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

// Begin an event, must be paired with an end event. Name is required,
// category is optional
void beginEvent(const char *name, const char *category);

void endEvent();

// Set a marker in the trace timeline, e.g., for things that have no duration
// Name is required, category is optional
void setMarker(const char *name, const char *category);

// Counter values are displayed per-process by chrome:://tracing
// but are recorded per-thread without synchronization
void setCounter(const char *name, uint64_t value);

// Record the built-in counters traceVirtMem and traceRssMem tracking the
// virtual and resident memory sizes respectively
void recordMemUse();

void setThreadName(const char *name);

void saveLog(const char *logFile, const char *processName);

} // namespace tracing
} // namespace rkcommon

#ifdef RKCOMMON_ENABLE_PROFILING

#define RKCOMMON_IF_TRACING_ENABLED(CMD) CMD

#else

#define RKCOMMON_IF_TRACING_ENABLED(CMD)

#endif
