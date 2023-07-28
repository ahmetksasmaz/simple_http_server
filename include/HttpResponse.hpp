#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "HttpMessage.hpp"

namespace http {

class HttpResponse : protected HttpMessage {
 public:
  // TODO : hem base class'ta hem de derived class ta protocol tanımladım ve
  // default değer verdim, bunu nasıl daha güzel yaparım
  HttpResponse(const status_codes::StatusCode status_code,
               const protocols::Protocol protocol = protocols::HTTP_1_1);
  // Because HttpResponse class doesn't have dynamically allocated memory
  // No need user defined copy / move constructors and assignment operators
  // We can use default ones, destructor included
  std::string& Stringify() const;

 private:
  const status_codes::StatusCode status_code_;
};

}  // namespace http

#endif