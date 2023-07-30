#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "HttpMessage.hpp"

namespace http {

class HttpRequest : public HttpMessage {
 public:
  HttpRequest(const methods::Method method, const std::string request_uri,
              const protocols::Protocol protocol = protocols::HTTP_1_1);
  // Because HttpRequest class doesn't have dynamically allocated memory
  // No need user defined copy / move constructors and assignment operators
  // We can use default ones, destructor included
  static HttpRequest& Parse(const std::string message);

  methods::Method GetMethod();
  std::string GetRequestURI();

 private:
  const methods::Method method_;
  const std::string request_uri_;
};

}  // namespace http

#endif