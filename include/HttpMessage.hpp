#ifndef HTTP_MESSAGE_H_
#define HTTP_MESSAGE_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

#include "HttpEnumerations.hpp"

namespace http {

class HttpMessage {
 public:
  HttpMessage(const protocols::Protocol protocol = protocols::HTTP_1_1);
  HttpMessage(const HttpMessage& message);
  HttpMessage(HttpMessage&& message);
  HttpMessage& operator=(const HttpMessage& message);
  HttpMessage& operator=(HttpMessage&& message);
  virtual ~HttpMessage();

 protected:
  protocols::Protocol protocol_;
  // By using std::map, we enforce headers to be sorted in practically
  // General > Request|Response > Entity
  std::map<headers::Header, std::string> headers_;
  std::string entity_body_;
};

}  // namespace http

#endif