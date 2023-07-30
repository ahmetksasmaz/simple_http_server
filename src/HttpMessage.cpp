#include "HttpMessage.hpp"

http::HttpMessage::HttpMessage(protocols::Protocol protocol)
    : protocol_(protocol) {}

http::HttpMessage::HttpMessage(const HttpMessage& message)
    : protocol_(message.protocol_) {
  for (auto header_pair : message.headers_) {
    headers_.insert(header_pair);
  }
  entity_body_ = message.entity_body_;
}

http::HttpMessage::HttpMessage(HttpMessage&& message) {
  if (this != &message) {
    headers_.clear();
    entity_body_.clear();

    protocol_ = message.protocol_;

    for (auto header_pair : message.headers_) {
      headers_.insert(header_pair);
    }
    entity_body_ = message.entity_body_;

    message.headers_.clear();
    message.entity_body_.clear();
  }
}

http::HttpMessage& http::HttpMessage::operator=(
    const http::HttpMessage& message) {
  protocol_ = message.protocol_;

  for (auto header_pair : message.headers_) {
    headers_.insert(header_pair);
  }
  entity_body_ = message.entity_body_;

  return *this;
}

http::HttpMessage& http::HttpMessage::operator=(http::HttpMessage&& message) {
  if (this != &message) {
    headers_.clear();
    entity_body_.clear();

    protocol_ = message.protocol_;

    for (auto header_pair : message.headers_) {
      headers_.insert(header_pair);
    }
    entity_body_ = message.entity_body_;

    message.headers_.clear();
    message.entity_body_.clear();
  }
  return *this;
}

http::HttpMessage::~HttpMessage() {
  headers_.clear();
  entity_body_.clear();
}

http::protocols::Protocol http::HttpMessage::GetProtocol() { return protocol_; }

std::string http::HttpMessage::GetHeader(headers::Header header) {
  try {
    return headers_.at(header);
  } catch (...) {
    return "";
  }
}

std::string http::HttpMessage::GetEntityBody() { return entity_body_; }

void http::HttpMessage::AddHeader(headers::Header header, std::string value) {
  headers_[header] = value;
}

void http::HttpMessage::SetEntityBody(std::string entity_body) {
  entity_body_ = entity_body;
}