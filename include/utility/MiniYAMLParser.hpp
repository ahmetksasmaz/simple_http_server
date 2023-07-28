#ifndef MINI_YAML_PARSER_H_
#define MINI_YAML_PARSER_H_

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "Logger.hpp"

namespace utility {

class YAMLNode {
 public:
  YAMLNode(const std::string node_value);
  // Because YAMLNode class doesn't have dynamically allocated memory
  // No need user defined copy / move constructors and assignment operators
  // We can use default ones, destructor included
  template <typename T>
  T as();

 private:
  std::string node_value_;
};

class MiniYAMLParser {
 public:
  MiniYAMLParser();
  MiniYAMLParser(const MiniYAMLParser& parser);
  MiniYAMLParser(MiniYAMLParser&& parser);
  MiniYAMLParser& operator=(const MiniYAMLParser& parser);
  MiniYAMLParser& operator=(MiniYAMLParser&& parser);
  YAMLNode& operator[](const std::string key);
  ~MiniYAMLParser();
  void Parse(const std::string filename);

 private:
  Logger logger_;
  std::unordered_map<std::string, YAMLNode> kv_map_;
};
}  // namespace utility

#endif