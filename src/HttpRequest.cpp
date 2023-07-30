#include "HttpRequest.hpp"

http::HttpRequest::HttpRequest(const methods::Method method,
                               const std::string request_uri,
                               const protocols::Protocol protocol)
    : method_(method),
      request_uri_(request_uri),
      http::HttpMessage::HttpMessage(protocol) {}

http::HttpRequest& http::HttpRequest::Parse(const std::string message) {
  std::stringstream message_stream(message);
  std::string line;
  std::getline(message_stream, line);
  std::istringstream request_line_parser_stream(line);
  std::string method, uri, protocol;
  request_line_parser_stream >> method >> uri >> protocol;
  methods::Method method_enum;
  protocols::Protocol protocol_enum;

  protocol_enum =
      protocol == "HTTP/1.1" ? protocols::HTTP_1_1 : protocols::NOT_SUPPORTED;
  method_enum = methods::ParseMethod(method);

  static HttpRequest request(method_enum, uri, protocol_enum);

  int content_length = 0;

  while (std::getline(message_stream, line)) {
    // Find ':'
    auto index = line.find(": ");
    if (index != std::string::npos) {
      std::string param = line.substr(0, index);
      std::string value = line.substr(index + 2);
      if (value[value.size() - 1] == '\r') {
        value = value.substr(0, value.size() - 1);
      }
      std::transform(param.begin(), param.end(), param.begin(), ::toupper);
      headers::Header header = headers::ParseHeader(param);
      if (header == headers::CONTENT_LENGTH) {
        content_length = stoi(value);
      }
      request.headers_.insert(
          std::pair<headers::Header, std::string>(header, value));
    } else {
      // EOF of Content Section Started
      break;
    }
  }
  if (content_length > 0) {
    while (std::getline(message_stream, line)) {
      request.entity_body_ += line;
    }
  }

  return request;
}

http::methods::Method http::HttpRequest::GetMethod() { return method_; }

std::string http::HttpRequest::GetRequestURI() { return request_uri_; }