// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "library.h"
#include "FileName.h"

#include <algorithm>

#ifndef _WIN32
#include <dlfcn.h>
#include <sys/times.h>
#endif

namespace {

  std::string directory_from_path(const std::string &path)
  {
    // Remove the filename from the path
    const size_t lastPathSep = path.find_last_of("/\\");
    if (lastPathSep == std::string::npos)
      throw std::runtime_error("could not get absolute path of module directory");
    return path.substr(0, lastPathSep + 1);
  }

  std::string library_location(const void *address)
  {
    // implementation taken from OIDN module.cpp

    if (address == nullptr)
      throw std::runtime_error("library_location(): NULL address provided");

  #if defined(_WIN32)

    // Get the handle of the module which contains the address
    HMODULE module;
    const DWORD flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
        | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
    if (!GetModuleHandleExA(flags, reinterpret_cast<LPCSTR>(address), &module))
      throw std::runtime_error("GetModuleHandleExA failed");

    // Get the path of the module
    // Since we don't know the length of the path, we use a buffer of increasing
    // size
    DWORD pathSize = MAX_PATH + 1;
    for (;;) {
      std::vector<char> path(pathSize);
      DWORD result = GetModuleFileNameA(module, path.data(), pathSize);
      if (result == 0)
        throw std::runtime_error("GetModuleFileNameA failed");
      else if (result < pathSize)
        return directory_from_path(path.data());
      else
        pathSize *= 2;
    }

  #else

    // dladdr should return an absolute path on Linux except for the main
    // executable On macOS it should always return an absolute path
    Dl_info info;
    if (dladdr(address, &info)) {
      // Check whether the path is absolute
      if (info.dli_fname && info.dli_fname[0] == '/')
        return directory_from_path(info.dli_fname);
    }

  #if defined(__APPLE__)
    // This shouldn't happen
    throw std::runtime_error("failed to get absolute path with dladdr");
  #else
    // We failed to get an absolute path, so we try to parse /proc/self/maps
    std::ifstream file("/proc/self/maps");
    if (!file)
      throw std::runtime_error("could not open /proc/self/maps");

    // Parse the lines
    for (std::string lineStr; std::getline(file, lineStr);) {
      std::istringstream line(lineStr);

      // Parse the address range
      uintptr_t addressBegin, addressEnd;
      line >> std::hex;
      line >> addressBegin;
      if (line.get() != '-')
        continue; // parse error
      line >> addressEnd;
      if (!isspace(line.peek()) || !line)
        continue; // parse error

      // Check whether the address is in this range
      if (reinterpret_cast<uintptr_t>(address) < addressBegin
          || reinterpret_cast<uintptr_t>(address) >= addressEnd)
        continue;

      // Skip the permissions, offset, device, inode
      std::string str;
      for (int i = 0; i < 4; ++i)
        line >> str;

      // Parse the path
      line >> std::ws;
      if (!std::getline(line, str))
        continue; // no path or parse error

      // Check whether the path is absolute
      if (str[0] == '/')
        return directory_from_path(str);
    }

    throw std::runtime_error("could not find address in /proc/self/maps");
  #endif

  #endif
  }

}  // namespace

namespace rkcommon {

  Library::Library(
      const void *anchorAddress, const std::string &name, const Version &version)
      : libraryName(name), libraryVersion(version)
  {
    bool success = false;

    try {
      success = loadLibrary(anchorAddress);
    } catch (const std::exception &e) {
      // handle exceptions from e.g. library_location()
      throw std::runtime_error(
          "Load of " + name + " failed due to: '" + e.what() + "'");
    }

    if (!success) {
      throw std::runtime_error(
          "Load of " + name + " failed due to: '" + errorMessage + "'");
    }
  }

  Library::Library(void *const _lib)
      : libraryName("<pre-loaded>"), lib(_lib), freeLibOnDelete(false)
  {
  }

  bool Library::loadLibrary(const void *anchorAddress)
  {
    std::string file = libraryName;
    std::string errorMsg;
    std::string libLocation = anchorAddress != nullptr
        ? library_location(anchorAddress)
        : std::string();

#ifdef _WIN32
    std::string fullName = libLocation + file + ".dll";
    lib                  = LoadLibrary(fullName.c_str());
    if (lib == nullptr) {
      DWORD err = GetLastError();
      LPTSTR lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    err,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR)&lpMsgBuf,
                    0,
                    NULL);

      errorMsg = lpMsgBuf;

      LocalFree(lpMsgBuf);
    }
#else
    std::string versionStr;
    for (int i: libraryVersion)
      versionStr += "." + std::to_string(i);

    std::string fullName = libLocation + "lib" + file;
#if defined(__MACOSX__) || defined(__APPLE__)
    fullName += versionStr + ".dylib";
#else
    fullName += ".so" + versionStr;
#endif

    lib = dlopen(fullName.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (lib == nullptr)
      errorMsg = dlerror();
#endif

    if (lib == nullptr) {
      errorMessage =
          "could not open module lib " + libraryName + ": " + errorMsg;
      return false;
    }

    return true;
  }

  Library::~Library()
  {
    /* Only dlclose/free libraries if we're not running through addrsan
     * so that the shared library symbols remain accessible to addrsan
     * at exit (see https://github.com/google/sanitizers/issues/89)
     */
#ifndef RKCOMMON_ADDRSAN
    if (freeLibOnDelete && lib) {
#ifdef _WIN32
      FreeLibrary((HMODULE)lib);
#else
      dlclose(lib);
#endif
    }
#endif
  }

  void *Library::getSymbol(const std::string &sym) const
  {
#ifdef _WIN32
    return GetProcAddress((HMODULE)lib, sym.c_str());
#else
    return dlsym(lib, sym.c_str());
#endif
  }

  std::unique_ptr<LibraryRepository> LibraryRepository::instance;

  LibraryRepository *LibraryRepository::getInstance()
  {
    if (instance.get() == nullptr)
      instance = std::unique_ptr<LibraryRepository>(new LibraryRepository);

    return instance.get();
  }

  void LibraryRepository::cleanupInstance()
  {
    LibraryRepository::instance.reset();
  }

  LibraryRepository::~LibraryRepository()
  {
    // Close libraries in the opposite order that they were opened
    while (!repo.empty()) {
      repo.pop_back();
    }
  }

  void LibraryRepository::add(const void *anchorAddress,
    const std::string &name, const Library::Version &version)
  {
    if (libraryExists(name))
      return; // lib already loaded.

    repo.push_back(rkcommon::make_unique<Library>(
      anchorAddress, name, version));
  }

  void LibraryRepository::remove(const std::string &name)
  {
    auto lib = findLibrary(name);
    if (lib != repo.end()) {
      repo.erase(lib);
    }
  }

  void *LibraryRepository::getSymbol(const std::string &name) const
  {
    void *sym = nullptr;
    for (auto lib = repo.cbegin(); sym == nullptr && lib != repo.end(); ++lib) {
      sym = (*lib)->getSymbol(name);
    }

    return sym;
  }

  bool LibraryRepository::libraryExists(const std::string &name) const
  {
    return findLibrary(name) != repo.end();
  }

  LibraryRepository::const_library_iterator_t LibraryRepository::findLibrary(
      const std::string &name) const
  {
    auto fnd = std::find_if(
        repo.begin(), repo.end(), [&](const std::unique_ptr<Library> &l) {
          return l->libraryName == name;
        });
    return fnd;
  }

  LibraryRepository::library_iterator_t LibraryRepository::findLibrary(
      const std::string &name)
  {
    auto fnd = std::find_if(
        repo.begin(), repo.end(), [&](const std::unique_ptr<Library> &l) {
          return l->libraryName == name;
        });
    return fnd;
  }

}  // namespace rkcommon
