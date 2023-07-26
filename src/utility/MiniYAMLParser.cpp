#include "utility/MiniYAMLParser.hpp"

utility::MiniYAMLParser::MiniYAMLParser() : logger_(Logger("YAMLParser")) {}
utility::MiniYAMLParser::~MiniYAMLParser() { kv_map_.clear(); }
utility::MiniYAMLParser::MiniYAMLParser(const MiniYAMLParser& parser)
    : logger_(Logger("YAMLParser")) {
  for (auto kv_pair : parser.kv_map_) {
    kv_map_.insert(kv_pair);
  }
}
utility::MiniYAMLParser::MiniYAMLParser(MiniYAMLParser&& parser)
    : logger_(parser.logger_) {
  if (this != &parser) {
    kv_map_.clear();
    for (auto kv_pair : parser.kv_map_) {
      kv_map_.insert(kv_pair);
    }
    parser.kv_map_.clear();
  }
}
utility::MiniYAMLParser& utility::MiniYAMLParser::operator=(
    const MiniYAMLParser& parser) {
  for (auto kv_pair : parser.kv_map_) {
    kv_map_.insert(kv_pair);
  }
  return *this;
}
utility::MiniYAMLParser& utility::MiniYAMLParser::operator=(
    MiniYAMLParser&& parser) {
  if (this != &parser) {
    kv_map_.clear();
    for (auto kv_pair : parser.kv_map_) {
      kv_map_.insert(kv_pair);
    }
    parser.kv_map_.clear();
  }
  return *this;
}
utility::YAMLNode& utility::MiniYAMLParser::operator[](const std::string key) {
  try {
    return kv_map_.at(key);
  } catch (std::out_of_range& ex) {
    logger_.Error("YAML key - value pair does not exist for key : " + key);
    exit(1);
  } catch (std::exception& ex) {
    logger_.Error(ex.what());
    exit(1);
  }
}

void utility::MiniYAMLParser::Parse(const std::string filename) {
  try {
    std::ifstream file(filename);
    if (file) {
      std::string line;
      int line_number = -1;
      while (std::getline(file, line)) {
        line_number++;
        if (line[0] == '#' || line[0] == '\n') {
          // Skip comment and empty lines
          continue;
        }
        size_t index = line.find(":");
        if (index == std::string::npos) {
          throw std::runtime_error("YAML file cannot be parsed on line : " +
                                   std::to_string(line_number));
        }
        std::string key = line.substr(0, index);
        std::string value = line.substr(index + 2);  // :_ skip 2 characters
        kv_map_.insert(std::make_pair<const std::string, YAMLNode>(
            std::move(key), YAMLNode(value)));
      }

    } else {
      throw std::runtime_error("YAML file cannot be opened.");
    }
  } catch (std::exception& ex) {
    logger_.Error(ex.what());
    exit(1);
  }
}

utility::YAMLNode::YAMLNode(const std::string node_value)
    : node_value_(node_value) {}

template <>
std::string utility::YAMLNode::as<std::string>() {
  return node_value_;
}

template <>
int utility::YAMLNode::as<int>() {
  return stoi(node_value_);
}

template <>
float utility::YAMLNode::as<float>() {
  return stof(node_value_);
}

template <>
double utility::YAMLNode::as<double>() {
  return stod(node_value_);
}