#include "HttpRequest.hpp"

http::HttpRequest::HttpRequest(const methods::Method method,
                               const std::string request_uri,
                               const protocols::Protocol protocol)
    : method_(method),
      request_uri_(request_uri),
      http::HttpMessage::HttpMessage(protocol) {}

http::HttpRequest& http::HttpRequest::Parse(const std::string message) {
  // TODO Implement
}