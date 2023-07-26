#include "utility/Logger.hpp"

utility::Logger::Logger(const std::string logger_name)
    : logger_name_(logger_name) {}

void utility::Logger::Print(const std::string log_type,
                            const std::string message) const {
  std::ostringstream ss;
  ss << "[" << logger_name_ << "][" << log_type << "] : " << message;
  std::cout << ss.str() << std::endl;
}

void utility::Logger::Info(const std::string message) const {
  Print("INFO", message);
}
void utility::Logger::Error(const std::string message) const {
  Print("ERROR", message);
}
void utility::Logger::Warn(const std::string message) const {
  Print("WARN", message);
}
void utility::Logger::Debug(const std::string message) const {
  Print("DEBUG", message);
}

std::string utility::Logger::GetName() const { return logger_name_; }