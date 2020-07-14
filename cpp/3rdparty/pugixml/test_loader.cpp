//
// Created by books on 2020/7/11.
//

#include <iostream>
#include <fstream>

#include <pugixml.hpp>

#include <gtest/gtest.h>

TEST(PUGIXML_DOCUMENT, load_from_file)
{
  std::string file = "data.xml";
  pugi::xml_document xmlDocument;
  pugi::xml_parse_result xmlParseResult = xmlDocument.load_file(file.c_str());

  std::cout << "Load result: " << xmlParseResult.description()
            << ", mesh name: "
            << xmlDocument.child("mesh").attribute("name").value() << std::endl;
}

TEST(PUGIXML_DOCUMENT, load_from_buffer)
{
  std::string clause = "<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>";

  pugi::xml_document xmlDocument;
  pugi::xml_parse_result xmlParseResult =
      xmlDocument.load_buffer(clause.c_str(), clause.size());

  std::cout << "Load result: " << xmlParseResult.description()
            << ", mesh name: "
            << xmlDocument.child("mesh").attribute("name").value() << std::endl;
}

TEST(PUGIXML_DOCUMENT,load_from_stream)
{
  std::string file = "data.xml";
  std::ifstream ifs(file);
  
  pugi::xml_document xmlDocument;
  pugi::xml_parse_result xmlParseResult = xmlDocument.load(ifs);
  
  std::cout << "Load result: " << xmlParseResult.description()
            << ", mesh name: "
            << xmlDocument.child("mesh").attribute("name").value() << std::endl;
}