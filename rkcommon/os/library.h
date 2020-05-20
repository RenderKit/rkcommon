// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../common.h"
// std
#include <map>
#include <memory>
#include <string>

namespace rkcommon {

  class RKCOMMON_INTERFACE Library
  {
   public:
    /* opens a shared library */
    Library(const std::string &name, bool anchor = false);
    ~Library();

    /* returns address of a symbol from the library */
    void *getSymbol(const std::string &sym) const;

   private:
    Library(void *const lib);
    std::string libraryName;
    void *lib;
    bool freeLibOnDelete{true};
    friend class LibraryRepository;
  };

  class RKCOMMON_INTERFACE LibraryRepository
  {
   public:
    static LibraryRepository *getInstance();
    static void cleanupInstance();

    ~LibraryRepository();

    // add/remove a library to/from the repo
    void add(const std::string &name, bool anchor = false);
    void remove(const std::string &name);

    /* returns address of a symbol from any library in the repo */
    void *getSymbol(const std::string &sym) const;

    /* add the default library to the repo */
    void addDefaultLibrary();

    bool libraryExists(const std::string &name) const;

   private:
    static std::unique_ptr<LibraryRepository> instance;
    LibraryRepository() = default;
    std::map<std::string, Library *> repo;
  };
}  // namespace rkcommon
