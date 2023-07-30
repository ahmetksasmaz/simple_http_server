#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <chrono>
#include <ctime>
#include <iomanip>

#include "HttpMessage.hpp"

namespace http {

class HttpResponse : public HttpMessage {
 public:
  HttpResponse(const status_codes::StatusCode status_code,
               const protocols::Protocol protocol = protocols::HTTP_1_1);
  // Because HttpResponse class doesn't have dynamically allocated memory
  // No need user defined copy / move constructors and assignment operators
  // We can use default ones, destructor included
  std::string Stringify();

  status_codes::StatusCode GetStatusCode();

  static std::string UTCDate();

 private:
  const status_codes::StatusCode status_code_;
};

}  // namespace http

#endif