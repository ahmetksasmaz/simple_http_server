#include "HttpServer.hpp"
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

  const std::string ip_addr = yaml_parser["ip_addr"].as<std::string>();
  const int port = yaml_parser["port"].as<int>();
  const int maximum_connections = yaml_parser["maximum_connections"].as<int>();
  const int event_fds = yaml_parser["event_fds"].as<int>();
  const int read_workers = yaml_parser["read_workers"].as<int>();
  const int process_workers = yaml_parser["process_workers"].as<int>();
  const int write_workers = yaml_parser["write_workers"].as<int>();

  const bool info_enabled = yaml_parser["info"].as<bool>();
  const bool error_enabled = yaml_parser["error"].as<bool>();
  const bool warn_enabled = yaml_parser["warn"].as<bool>();
  const bool debug_enabled = yaml_parser["debug"].as<bool>();

  utility::Logger::info_enabled_ = info_enabled;
  utility::Logger::error_enabled_ = error_enabled;
  utility::Logger::warn_enabled_ = warn_enabled;
  utility::Logger::debug_enabled_ = debug_enabled;

  http::HttpServer http_server(ip_addr, port, maximum_connections, event_fds,
                               read_workers, process_workers, write_workers);

  while (true) {
    // TODO User interaction
  }

  return 0;
}