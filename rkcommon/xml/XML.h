// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// ospcomon
#include "../common.h"
#include "../math/vec.h"
#include "../os/FileName.h"

// stl
#include <map>
#include <memory>
#include <stack>
#include <vector>

namespace rkcommon {
  namespace xml {

    struct Node;
    struct XMLDoc;

    /*! a XML node, consisting of a name, a list of properties, and a
      set of child nodes */
    struct RKCOMMON_INTERFACE Node
    {
      Node(XMLDoc *doc) : doc(doc) {}

      /*! checks if given node has given property */
      bool hasProp(const std::string &name) const;

      /*! return value of property with given name if present; and throw an
       * exception if not */
      std::string getProp(const std::string &name) const;

      /*! return value of property with given name if present, else return
       * 'fallbackValue' */
      std::string getProp(const std::string &name,
                          const std::string &fallbackValue) const;

      /*! name of the xml node (i.e., the thing that's in
          "<name>....</name>") */
      std::string name;

      /*! the content string, i.e., the thing that's between
          "<name>..." and "...</name>" */
      std::string content;

      /*! \brief list of xml node properties properties.  '

        \detailed prop'erties in xml nodes are the 'name="value"'
        inside the <node name1="value1" name2="value2"> ... </node>
        description */
      std::map<std::string, std::string> properties;

      /*! list of child nodes */
      std::vector<Node> child;

      //! pointer to parent doc
      /*! \detailed this points back to the parent xml doc that
          contained this node. note this is intentionally NOT a ref to
          avoid cyclical dependencies. Ie, do NOT use this unless
          you're sure that the XMLDoc node that contained the given
          node is still around! */
      XMLDoc *doc{nullptr};
    };

    /*! a entire xml document */
    struct RKCOMMON_INTERFACE XMLDoc : public Node
    {
      //! constructor
      XMLDoc() : Node(this) {}

      //! the name (and path etc) of the file that this doc was read from
      FileName fileName;
    };

    /*! parse an XML file with given file name, and return a pointer
      to it.  In case of any error, this function will free all
      already-allocated data, and throw a std::runtime_error
      exception */
    RKCOMMON_INTERFACE std::shared_ptr<XMLDoc> readXML(const std::string &fn);

    /*! helper class for writing sg nodes in XML format */
    struct Writer
    {
      Writer(FILE *xml, FILE *bin);

      /*! write document header, may only be called once */
      void writeHeader(const std::string &version);
      /*! write document footer. may only be called once, at end of write */
      void writeFooter();

      //! open a new xml node with given node type */
      void openNode(const std::string &type);
      void writeProperty(const std::string &name, const std::string &value);
      void writeContent(const std::string &name, const std::string &value);
      //! close last open node type */
      void closeNode();
      /*! align output pos on binary file to given alignment */
      void alignData(size_t alignment);
      /*! write given data into data file, and return offset value at
          which it was written */
      size_t writeData(const void *ptr, size_t size);
      FILE *xml, *bin;

     private:
      struct State
      {
        bool hasContent{false};
        std::string type;
      };

      void spaces();
      std::stack<State *> state;
    };

  }  // namespace xml
}  // namespace rkcommon
