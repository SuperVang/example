//
// Created by books on 2020/7/11.
//

#include <fstream>
#include <iostream>

#include <pugixml.hpp>

#include <gtest/gtest.h>

TEST(PUGIXML_NODE, root_node)
{
  std::string file = "data.xml";
  pugi::xml_document xmlDocument;
  pugi::xml_parse_result xmlParseResult = xmlDocument.load_file(file.c_str());

  pugi::xml_node root = xmlDocument.root();
  std::cout << "root: " << root.name() << "\t" << root.value() << std::endl;
}

TEST(PUGIXML, traverse_children)
{
  std::string file = "data.xml";
  pugi::xml_document xmlDocument;
  pugi::xml_parse_result xmlParseResult = xmlDocument.load_file(file.c_str());

  pugi::xml_node root = xmlDocument.root();
  for (pugi::xml_node child = root.first_child(); child;
       child = child.next_sibling()) {
    std::cout << "child: " << child.name() << "\t" << child.child_value()
              << std::endl;
  }
}
