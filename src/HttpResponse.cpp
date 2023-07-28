#include "HttpResponse.hpp"

http::HttpResponse::HttpResponse(const status_codes::StatusCode status_code,
                                 const protocols::Protocol protocol)
    : status_code_(status_code), http::HttpMessage::HttpMessage(protocol) {}

std::string& http::HttpResponse::Stringify() const {
  // TODO Implement
}