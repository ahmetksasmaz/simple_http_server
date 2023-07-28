#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <sstream>

namespace utility {

class Logger {
 public:
  Logger(const std::string logger_name);
  // Because Logger class doesn't have dynamically allocated memory
  // No need user defined copy / move constructors and assignment operators
  // We can use default ones, destructor included
  void Info(const std::string message) const;
  void Error(const std::string message) const;
  void Warn(const std::string message) const;
  void Debug(const std::string message) const;
  std::string GetName() const;

  static bool info_enabled_;
  static bool error_enabled_;
  static bool warn_enabled_;
  static bool debug_enabled_;

 private:
  void Print(const std::string log_type, const std::string message) const;
  std::string logger_name_;
};

}  // namespace utility

#endif