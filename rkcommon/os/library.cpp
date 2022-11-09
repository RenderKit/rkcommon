// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "library.h"
#include "FileName.h"

#include <algorithm>

#ifndef _WIN32
#include <dlfcn.h>
#include <sys/times.h>
#endif

#if defined(__MACOSX__) || defined(__APPLE__)
#define RKCOMMON_LIB_EXT ".dylib"
#else
#define RKCOMMON_LIB_EXT ".so"
#endif

extern "C" {
/* Export a symbol to ask the dynamic loader about in order to locate this
 * library at runtime. */
RKCOMMON_INTERFACE int _rkcommon_anchor()
{
  return 0;
}
}

namespace {

  std::string library_location()
  {
#if defined(_WIN32) && !defined(__CYGWIN__)
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(&_rkcommon_anchor, &mbi, sizeof(mbi));
    char pathBuf[16384];
    if (!GetModuleFileNameA(
            static_cast<HMODULE>(mbi.AllocationBase), pathBuf, sizeof(pathBuf)))
      return std::string();

    std::string path = std::string(pathBuf);
    path.resize(path.rfind('\\') + 1);
#else
    const char *anchor = "_rkcommon_anchor";
    void *handle       = dlsym(RTLD_DEFAULT, anchor);
    if (!handle)
      return std::string();

    Dl_info di;
    int ret = dladdr(handle, &di);
    if (!ret || !di.dli_saddr || !di.dli_fname)
      return std::string();

    std::string path = std::string(di.dli_fname);
    path.resize(path.rfind('/') + 1);
#endif

    return path;
  }

}  // namespace

namespace rkcommon {

  Library::Library(const std::string &name, bool) : libraryName(name)
  {
    bool success = loadLibrary(false);
    const std::string firstErrorMessage = errorMessage;
    if (!success)
      success = loadLibrary(true);

    if (!success) {
      throw std::runtime_error("First load of " + name +
          " failed due to: '" + firstErrorMessage +
          "', second attempt failed due to: '" + errorMessage + "'");
    }
  }

  Library::Library(void *const _lib)
      : libraryName("<pre-loaded>"), lib(_lib), freeLibOnDelete(false)
  {
  }

  bool Library::loadLibrary(bool withAnchor)
  {
    std::string file = libraryName;
    std::string errorMsg;
    std::string libLocation = withAnchor ? library_location() : std::string();
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
    std::string fullName = libLocation + "lib" + file + RKCOMMON_LIB_EXT;
    lib                  = dlopen(fullName.c_str(), RTLD_LAZY | RTLD_LOCAL);
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

  void LibraryRepository::add(const std::string &name, bool anchor)
  {
    if (libraryExists(name))
      return;  // lib already loaded.

    repo.push_back(rkcommon::make_unique<Library>(name, anchor));
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

  void LibraryRepository::addDefaultLibrary()
  {
    // already populate the repo with "virtual" libs, representing the default
    // OSPRay core lib
#ifdef _WIN32
    // look in exe (i.e. when OSPRay is linked statically into the application)
    repo.push_back(rkcommon::make_unique<Library>(GetModuleHandle(0)));

    // look in ospray.dll (i.e. when linked dynamically)
#if 0
    // we cannot get a function from ospray.dll, because this would create a
    // cyclic dependency between ospray.dll and ospray_common.dll

    // only works when ospray_common is linked statically into ospray
    const void * functionInOSPRayDLL = rkcommon::getSymbol;
    // get handle to current dll via a known function
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(functionInOSPRayDLL, &mbi, sizeof(mbi));
    repo["dlldefault"] = new Library(mbi.AllocationBase);
#else
    repo.push_back(rkcommon::make_unique<Library>(std::string("ospray")));
#endif
#else
    repo.push_back(rkcommon::make_unique<Library>(RTLD_DEFAULT));
#endif
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
