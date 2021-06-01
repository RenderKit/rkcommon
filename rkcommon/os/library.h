// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../common.h"
// std
#include <memory>
#include <string>
#include <vector>

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

    bool loadLibrary(bool withAnchor);

    std::string libraryName;
    std::string errorMessage;
    void *lib{nullptr};
    bool freeLibOnDelete{true};

    friend class LibraryRepository;

    template <typename T, typename... Args>
    friend inline std::unique_ptr<T> make_unique(Args &&... args);
  };

  class RKCOMMON_INTERFACE LibraryRepository
  {
   public:
    static LibraryRepository *getInstance();
    static void cleanupInstance();

    ~LibraryRepository();

    LibraryRepository(const LibraryRepository &) = delete;
    LibraryRepository &operator=(const LibraryRepository &) = delete;

    // add/remove a library to/from the repo
    void add(const std::string &name, bool anchor = false);
    void remove(const std::string &name);

    /* returns address of a symbol from any library in the repo */
    void *getSymbol(const std::string &sym) const;

    /* add the default library to the repo */
    void addDefaultLibrary();

    bool libraryExists(const std::string &name) const;

   private:
    using const_library_iterator_t =
        std::vector<std::unique_ptr<Library>>::const_iterator;
    using library_iterator_t = std::vector<std::unique_ptr<Library>>::iterator;

    const_library_iterator_t findLibrary(const std::string &name) const;
    library_iterator_t findLibrary(const std::string &name);

    static std::unique_ptr<LibraryRepository> instance;
    LibraryRepository() = default;

    std::vector<std::unique_ptr<Library>> repo;
  };
}  // namespace rkcommon
