#include "HttpResponse.hpp"

http::HttpResponse::HttpResponse(const status_codes::StatusCode status_code,
                                 const protocols::Protocol protocol)
    : status_code_(status_code), http::HttpMessage::HttpMessage(protocol) {}

std::string http::HttpResponse::Stringify() {
  std::string response_str = "";

  // Status Line
  response_str += protocols::ProtocolToText(protocol_);
  response_str += " ";
  response_str += std::to_string(status_code_);
  response_str += " ";
  response_str += status_codes::StatusCodeToText(status_code_);
  response_str += "\r\n";

  // Headers
  if (status_code_ < 200 || status_code_ == 204 || status_code_ == 304) {
    headers_.erase(headers::CONTENT_LENGTH);
  } else {
    try {
      headers_.at(headers::CONTENT_LENGTH);
    } catch (...) {
      // Content length is not set
      headers_.insert(std::pair<headers::Header, std::string>(
          headers::CONTENT_LENGTH, std::to_string(entity_body_.size())));
    }
  }

  for (auto& header_pair : headers_) {
    headers::Header param = header_pair.first;
    std::string value = header_pair.second;
    if (param == headers::CONTENT_LENGTH) {
      // Override content-length
      value = std::to_string(entity_body_.size());
    }
    response_str += headers::HeaderToText(param);
    response_str += ": ";
    response_str += value;
    response_str += "\r\n";
  }

  response_str += "\r\n";
  // Content
  if (entity_body_.size()) {
    response_str += entity_body_;
  }

  return response_str;
}

http::status_codes::StatusCode http::HttpResponse::GetStatusCode() {
  return status_code_;
}

std::string http::HttpResponse::UTCDate() {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  std::tm* utc_time = std::gmtime(&time);
  std::ostringstream time_string_stream;
  time_string_stream << std::put_time(utc_time, "%a, %d %b %Y %H:%M:%S GMT");

  return time_string_stream.str();
}