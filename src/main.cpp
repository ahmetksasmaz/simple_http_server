#include "utility/Logger.hpp"
#include "utility/MiniYAMLParser.hpp"

int main(int argc, char** argv) {
  std::string config_file_path = "../config/config.yaml";
  if (argc == 2) {
    config_file_path = std::string(argv[1]);
  }
  utility::Logger logger("Main");
  utility::MiniYAMLParser yaml_parser;
  yaml_parser.Parse(config_file_path);

  std::cout << "Value for ip_addr : "
            << yaml_parser["ip_addr"].as<std::string>() << std::endl;
  std::cout << "Value for port : " << yaml_parser["port"].as<int>()
            << std::endl;
  std::cout << "Value for max_worker_threads : "
            << yaml_parser["max_worker_threads"].as<int>() << std::endl;

  return 0;
}