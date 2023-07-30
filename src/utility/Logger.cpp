#include "utility/Logger.hpp"

bool utility::Logger::info_enabled_;
bool utility::Logger::error_enabled_;
bool utility::Logger::warn_enabled_;
bool utility::Logger::debug_enabled_;

utility::Logger::Logger(const std::string logger_name)
    : logger_name_(logger_name) {}

void utility::Logger::Print(const std::string log_type,
                            const std::string message) const {
  std::ostringstream ss;
  ss << "[" << logger_name_ << "][" << log_type << "] : " << message;
}

void utility::Logger::Info(const std::string message) const {
  if (info_enabled_) {
    Print("INFO", message);
  }
}
void utility::Logger::Error(const std::string message) const {
  if (error_enabled_) {
    Print("ERROR", message);
  }
}
void utility::Logger::Warn(const std::string message) const {
  if (warn_enabled_) {
    Print("WARN", message);
  }
}
void utility::Logger::Debug(const std::string message) const {
  if (debug_enabled_) {
    Print("DEBUG", message);
  }
}

std::string utility::Logger::GetName() const { return logger_name_; }