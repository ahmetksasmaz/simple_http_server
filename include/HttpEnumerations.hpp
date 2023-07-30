#ifndef HTTP_ENUMERATIONS_H_
#define HTTP_ENUMERATIONS_H_

#include <map>
#include <string>

namespace http {

namespace protocols {

enum Protocol { HTTP_1_1, NOT_SUPPORTED };

static std::string ProtocolToText(Protocol protocol) {
  return protocol == Protocol::HTTP_1_1 ? "HTTP/1.1" : "";
}

}  // namespace protocols

namespace headers {

enum Header {
  // General Headers
  CACHE_CONTROL,
  CONNECTION,
  DATE,
  FORWARDED,
  KEEP_ALIVE,
  MIME_VERSION,
  PRAGMA,
  UPGRADE,
  // Request Headers
  ACCEPT,
  ACCEPT_CHARSET,
  ACCEPT_ENCODING,
  ACCEPT_LANGUAGE,
  AUTHORIZATION,
  FROM,
  HOST,
  IF_MODIFIED_SINCE,
  PROXY_AUTHORIZATION,
  RANGE,
  REFERER,
  UNLESS,
  USER_AGENT,
  // Response Headers
  LOCATION,
  PROXY_AUTHENTICATE,
  PUBLIC,
  RETRY_AFTER,
  SERVER,
  WWW_AUTHENTICATE,
  // Entity Headers
  ALLOW,
  CONTENT_ENCODING,
  CONTENT_LANGUAGE,
  CONTENT_LENGTH,
  CONTENT_MD5,
  CONTENT_RANGE,
  CONTENT_TYPE,
  CONTENT_VERSION,
  DERIVED_FROM,
  EXPIRES,
  LAST_MODIFIED,
  LINK,
  TITLE,
  TRANSFER_ENCODING,
  URI_HEADER
};

static const std::unordered_map<Header, std::string> header_texts = {
    // General Headers
    {Header::CACHE_CONTROL, "Cache-Control"},
    {Header::CONNECTION, "Connection"},
    {Header::DATE, "Date"},
    {Header::FORWARDED, "Forwarded"},
    {Header::KEEP_ALIVE, "Keep-Alive"},
    {Header::MIME_VERSION, "Mime-Version"},
    {Header::PRAGMA, "Pragma"},
    {Header::UPGRADE, "Upgrade"},
    // Request Headers
    {Header::ACCEPT, "Accept"},
    {Header::ACCEPT_CHARSET, "Accept-Charset"},
    {Header::ACCEPT_ENCODING, "Accept-Encoding"},
    {Header::ACCEPT_LANGUAGE, "Accept-Language"},
    {Header::AUTHORIZATION, "Authorization"},
    {Header::FROM, "From"},
    {Header::HOST, "Host"},
    {Header::IF_MODIFIED_SINCE, "If-Modified-Since"},
    {Header::PROXY_AUTHORIZATION, "Proxy-Authorization"},
    {Header::RANGE, "Range"},
    {Header::REFERER, "Referer"},
    {Header::UNLESS, "Unless"},
    {Header::USER_AGENT, "User-Agent"},
    // Response Headers
    {Header::LOCATION, "Location"},
    {Header::PROXY_AUTHENTICATE, "Proxy-Authenticate"},
    {Header::PUBLIC, "Public"},
    {Header::RETRY_AFTER, "Retry-After"},
    {Header::SERVER, "Server"},
    {Header::WWW_AUTHENTICATE, "WWW-Authenticate"},
    // Entity Headers
    {Header::ALLOW, "Allow"},
    {Header::CONTENT_ENCODING, "Content-Encoding"},
    {Header::CONTENT_LANGUAGE, "Content-Language"},
    {Header::CONTENT_LENGTH, "Content-Length"},
    {Header::CONTENT_MD5, "Content-MD5"},
    {Header::CONTENT_RANGE, "Content-Range"},
    {Header::CONTENT_TYPE, "Content-Type"},
    {Header::CONTENT_VERSION, "Content-Version"},
    {Header::DERIVED_FROM, "Derived-From"},
    {Header::EXPIRES, "Expires"},
    {Header::LAST_MODIFIED, "Last-Modified"},
    {Header::LINK, "Link"},
    {Header::TITLE, "Title"},
    {Header::TRANSFER_ENCODING, "Transfer-Encoding"},
    {Header::URI_HEADER, "URI-Header"},
};

static std::string HeaderToText(Header header) {
  return header_texts.at(header);
}

enum HeaderState {
  START,
  MIDDLE_A,
  MIDDLE_ACCEPT,
  MIDDLE_ACCEPT_,
  MIDDLE_C,
  MIDDLE_CON,
  MIDDLE_CONTENT_,
  MIDDLE_CONTENT_L,
  MIDDLE_D,
  MIDDLE_F,
  MIDDLE_L,
  MIDDLE_P,
  MIDDLE_PR,
  MIDDLE_PROXY_AUTH,
  MIDDLE_R,
  MIDDLE_RE,
  MIDDLE_T,
  MIDDLE_U,
  ENDED,
  // General Headers
  END_CACHE_CONTROL,
  END_CONNECTION,
  END_DATE,
  END_FORWARDED,
  END_KEEP_ALIVE,
  END_MIME_VERSION,
  END_PRAGMA,
  END_UPGRADE,
  // Request Headers
  END_ACCEPT,
  END_ACCEPT_CHARSET,
  END_ACCEPT_ENCODING,
  END_ACCEPT_LANGUAGE,
  END_AUTHORIZATION,
  END_FROM,
  END_HOST,
  END_IF_MODIFIED_SINCE,
  END_PROXY_AUTHORIZATION,
  END_RANGE,
  END_REFERER,
  END_UNLESS,
  END_USER_AGENT,
  // Response Headers
  END_LOCATION,
  END_PROXY_AUTHENTICATE,
  END_PUBLIC,
  END_RETRY_AFTER,
  END_SERVER,
  END_WWW_AUTHENTICATE,
  // Entity Headers
  END_ALLOW,
  END_CONTENT_ENCODING,
  END_CONTENT_LANGUAGE,
  END_CONTENT_LENGTH,
  END_CONTENT_MD5,
  END_CONTENT_RANGE,
  END_CONTENT_TYPE,
  END_CONTENT_VERSION,
  END_DERIVED_FROM,
  END_EXPIRES,
  END_LAST_MODIFIED,
  END_LINK,
  END_TITLE,
  END_TRANSFER_ENCODING,
  END_URI_HEADER,
  END_NOT_SUPPORTED
};

static const std::map<std::pair<HeaderState, char>, std::pair<HeaderState, int>>
    header_state_tree = {
        {{HeaderState::START, 'A'}, {HeaderState::MIDDLE_A, 1}},
        {{HeaderState::MIDDLE_A, 'C'}, {HeaderState::MIDDLE_ACCEPT, 5}},
        {{HeaderState::MIDDLE_ACCEPT, '_'}, {HeaderState::MIDDLE_ACCEPT_, 1}},
        {{HeaderState::START, 'C'}, {HeaderState::MIDDLE_C, 1}},
        {{HeaderState::MIDDLE_C, 'O'}, {HeaderState::MIDDLE_CON, 2}},
        {{HeaderState::MIDDLE_CON, 'T'}, {HeaderState::MIDDLE_CONTENT_, 5}},
        {{HeaderState::MIDDLE_CONTENT_, 'L'},
         {HeaderState::MIDDLE_CONTENT_L, 1}},
        {{HeaderState::START, 'D'}, {HeaderState::MIDDLE_D, 1}},
        {{HeaderState::START, 'F'}, {HeaderState::MIDDLE_F, 1}},
        {{HeaderState::START, 'L'}, {HeaderState::MIDDLE_L, 1}},
        {{HeaderState::START, 'P'}, {HeaderState::MIDDLE_P, 1}},
        {{HeaderState::MIDDLE_P, 'R'}, {HeaderState::MIDDLE_PR, 1}},
        {{HeaderState::MIDDLE_PR, 'O'}, {HeaderState::MIDDLE_PROXY_AUTH, 8}},
        {{HeaderState::START, 'R'}, {HeaderState::MIDDLE_R, 1}},
        {{HeaderState::MIDDLE_R, 'E'}, {HeaderState::MIDDLE_RE, 1}},
        {{HeaderState::START, 'T'}, {HeaderState::MIDDLE_T, 1}},
        {{HeaderState::START, 'U'}, {HeaderState::MIDDLE_U, 1}},

        {{HeaderState::MIDDLE_C, 'A'}, {HeaderState::END_CACHE_CONTROL, 0}},
        {{HeaderState::MIDDLE_CON, 'N'}, {HeaderState::END_CONNECTION, 0}},
        {{HeaderState::MIDDLE_D, 'A'}, {HeaderState::END_DATE, 0}},
        {{HeaderState::MIDDLE_F, 'O'}, {HeaderState::END_FORWARDED, 0}},
        {{HeaderState::START, 'K'}, {HeaderState::END_KEEP_ALIVE, 0}},
        {{HeaderState::START, 'M'}, {HeaderState::END_MIME_VERSION, 0}},
        {{HeaderState::MIDDLE_PR, 'A'}, {HeaderState::END_PRAGMA, 0}},
        {{HeaderState::MIDDLE_U, 'P'}, {HeaderState::END_UPGRADE, 0}},
        {{HeaderState::MIDDLE_ACCEPT, 0}, {HeaderState::END_ACCEPT, 0}},
        {{HeaderState::MIDDLE_ACCEPT_, 'C'},
         {HeaderState::END_ACCEPT_CHARSET, 0}},
        {{HeaderState::MIDDLE_ACCEPT_, 'E'},
         {HeaderState::END_ACCEPT_ENCODING, 0}},
        {{HeaderState::MIDDLE_ACCEPT_, 'L'},
         {HeaderState::END_ACCEPT_LANGUAGE, 0}},
        {{HeaderState::MIDDLE_A, 'U'}, {HeaderState::END_AUTHORIZATION, 0}},
        {{HeaderState::MIDDLE_F, 'R'}, {HeaderState::END_FROM, 0}},
        {{HeaderState::START, 'H'}, {HeaderState::END_HOST, 0}},
        {{HeaderState::START, 'I'}, {HeaderState::END_IF_MODIFIED_SINCE, 0}},
        {{HeaderState::MIDDLE_PROXY_AUTH, 'O'},
         {HeaderState::END_PROXY_AUTHORIZATION, 0}},
        {{HeaderState::MIDDLE_R, 'A'}, {HeaderState::END_RANGE, 0}},
        {{HeaderState::MIDDLE_RE, 'F'}, {HeaderState::END_REFERER, 0}},
        {{HeaderState::MIDDLE_U, 'N'}, {HeaderState::END_UNLESS, 0}},
        {{HeaderState::MIDDLE_U, 'S'}, {HeaderState::END_USER_AGENT, 0}},
        {{HeaderState::MIDDLE_L, 'O'}, {HeaderState::END_LOCATION, 0}},
        {{HeaderState::MIDDLE_PROXY_AUTH, 'E'},
         {HeaderState::END_PROXY_AUTHENTICATE, 0}},
        {{HeaderState::MIDDLE_P, 'U'}, {HeaderState::END_PUBLIC, 0}},
        {{HeaderState::MIDDLE_RE, 'T'}, {HeaderState::END_RETRY_AFTER, 0}},
        {{HeaderState::START, 'S'}, {HeaderState::END_SERVER, 0}},
        {{HeaderState::START, 'W'}, {HeaderState::END_WWW_AUTHENTICATE, 0}},
        {{HeaderState::MIDDLE_A, 'E'}, {HeaderState::END_ALLOW, 0}},
        {{HeaderState::MIDDLE_CONTENT_, 'E'},
         {HeaderState::END_CONTENT_ENCODING, 0}},
        {{HeaderState::MIDDLE_CONTENT_L, 'A'},
         {HeaderState::END_CONTENT_LANGUAGE, 0}},
        {{HeaderState::MIDDLE_CONTENT_L, 'E'},
         {HeaderState::END_CONTENT_LENGTH, 0}},
        {{HeaderState::MIDDLE_CONTENT_, 'M'},
         {HeaderState::END_CONTENT_MD5, 0}},
        {{HeaderState::MIDDLE_CONTENT_, 'R'},
         {HeaderState::END_CONTENT_RANGE, 0}},
        {{HeaderState::MIDDLE_CONTENT_, 'T'},
         {HeaderState::END_CONTENT_TYPE, 0}},
        {{HeaderState::MIDDLE_CONTENT_, 'V'},
         {HeaderState::END_CONTENT_VERSION, 0}},
        {{HeaderState::MIDDLE_D, 'E'}, {HeaderState::END_DERIVED_FROM, 0}},
        {{HeaderState::START, 'E'}, {HeaderState::END_EXPIRES, 0}},
        {{HeaderState::MIDDLE_L, 'A'}, {HeaderState::END_LAST_MODIFIED, 0}},
        {{HeaderState::MIDDLE_L, 'I'}, {HeaderState::END_LINK, 0}},
        {{HeaderState::MIDDLE_T, 'I'}, {HeaderState::END_TITLE, 0}},
        {{HeaderState::MIDDLE_T, 'R'}, {HeaderState::END_TRANSFER_ENCODING, 0}},
        {{HeaderState::MIDDLE_U, 'R'}, {HeaderState::END_URI_HEADER, 0}},
        {{HeaderState::START, 'A'}, {HeaderState::END_NOT_SUPPORTED, 0}},
};

static Header ParseHeader(std::string header_str) {
  HeaderState current_header_state = HeaderState::START;
  std::string::iterator iter = header_str.begin();
  while (current_header_state < HeaderState::ENDED) {
    try {
      auto new_header_state_jump = header_state_tree.at(
          std::pair<HeaderState, char>(current_header_state, *iter));
      current_header_state = new_header_state_jump.first;
      iter += new_header_state_jump.second;
    } catch (...) {
      current_header_state = HeaderState::END_NOT_SUPPORTED;
    }
  }
  return (Header)(current_header_state - HeaderState::ENDED - 1);
}

}  // namespace headers

namespace methods {

enum Method {
  OPTIONS,
  GET,
  HEAD,
  POST,
  PUT,
  PATCH,
  COPY,
  MOVE,
  DELETE,
  LINK,
  UNLINK,
  TRACE,
  WRAPPED,
  NOT_SUPPORTED
};

enum MethodState {
  START,
  MIDDLE_P,
  ENDED,
  END_OPTIONS,
  END_GET,
  END_HEAD,
  END_POST,
  END_PUT,
  END_PATCH,
  END_COPY,
  END_MOVE,
  END_DELETE,
  END_LINK,
  END_UNLINK,
  END_TRACE,
  END_WRAPPED,
  END_NOT_SUPPORTED
};

static const std::map<std::pair<MethodState, char>, std::pair<MethodState, int>>
    method_state_tree = {
        {{MethodState::START, 'O'}, {MethodState::END_OPTIONS, 0}},
        {{MethodState::START, 'G'}, {MethodState::END_GET, 0}},
        {{MethodState::START, 'H'}, {MethodState::END_HEAD, 0}},
        {{MethodState::START, 'P'}, {MethodState::MIDDLE_P, 1}},
        {{MethodState::MIDDLE_P, 'O'}, {MethodState::END_POST, 0}},
        {{MethodState::MIDDLE_P, 'U'}, {MethodState::END_PUT, 0}},
        {{MethodState::MIDDLE_P, 'A'}, {MethodState::END_PATCH, 0}},
        {{MethodState::START, 'C'}, {MethodState::END_COPY, 0}},
        {{MethodState::START, 'M'}, {MethodState::END_MOVE, 0}},
        {{MethodState::START, 'D'}, {MethodState::END_DELETE, 0}},
        {{MethodState::START, 'L'}, {MethodState::END_LINK, 0}},
        {{MethodState::START, 'U'}, {MethodState::END_UNLINK, 0}},
        {{MethodState::START, 'T'}, {MethodState::END_TRACE, 0}},
        {{MethodState::START, 'W'}, {MethodState::END_WRAPPED, 0}},
};

static Method ParseMethod(std::string method_str) {
  MethodState current_method_state = MethodState::START;
  std::string::iterator iter = method_str.begin();
  while (current_method_state < MethodState::ENDED) {
    try {
      auto new_method_state_jump = method_state_tree.at(
          std::pair<MethodState, char>(current_method_state, *iter));
      current_method_state = new_method_state_jump.first;
      iter += new_method_state_jump.second;
    } catch (...) {
      current_method_state = MethodState::END_NOT_SUPPORTED;
    }
  }
  return (Method)(current_method_state - MethodState::ENDED - 1);
}
}  // namespace methods

namespace status_codes {

enum StatusCode {
  CONTINUE = 100,
  SWITCHING_PROTOCOLS = 101,
  OK = 200,
  CREATED = 201,
  ACCEPTED = 202,
  NON_AUTHORITATIVE_INFORMATION = 203,
  NO_CONTENT = 204,
  RESET_CONTENT = 205,
  PARTIAL_CONTENT = 206,
  MULTIPLE_CHOICES = 300,
  MOVED_PERMANENTLY = 301,
  MOVED_TEMPORARILY = 302,
  SEE_OTHER = 303,
  NOT_MODIFIED = 304,
  USE_PROXY = 305,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  PAYMENT_REQUIRED = 402,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  NONE_ACCEPTABLE = 406,
  PROXY_AUTHENTICATION_REQUIRED = 407,
  REQUEST_TIMEOUT = 408,
  CONFLICT = 409,
  GONE = 410,
  LENGTH_REQUIRED = 411,
  UNLESS_TRUE = 412,
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  BAD_GATEWAY = 502,
  SERVICE_UNAVAILABLE = 503,
  GATEWAY_TIMEOUT = 504
};

static const std::unordered_map<StatusCode, std::string> status_code_texts = {
    {StatusCode::CONTINUE, "Continue"},
    {StatusCode::SWITCHING_PROTOCOLS, "Switching Protocols"},
    {StatusCode::OK, "OK"},
    {StatusCode::CREATED, "Created"},
    {StatusCode::ACCEPTED, "Accepted"},
    {StatusCode::NON_AUTHORITATIVE_INFORMATION,
     "Non-Authoritative Information"},
    {StatusCode::NO_CONTENT, "No Content"},
    {StatusCode::RESET_CONTENT, "Reset Content"},
    {StatusCode::PARTIAL_CONTENT, "Partial Content"},
    {StatusCode::MULTIPLE_CHOICES, "Multiple Choices"},
    {StatusCode::MOVED_PERMANENTLY, "Moved Permanently"},
    {StatusCode::MOVED_TEMPORARILY, "Moved Temporarily"},
    {StatusCode::SEE_OTHER, "See Other"},
    {StatusCode::NOT_MODIFIED, "Not Modified"},
    {StatusCode::USE_PROXY, "Use Proxy"},
    {StatusCode::BAD_REQUEST, "Bad Request"},
    {StatusCode::UNAUTHORIZED, "Unauthorized"},
    {StatusCode::PAYMENT_REQUIRED, "Payment Required"},
    {StatusCode::FORBIDDEN, "Forbidden"},
    {StatusCode::NOT_FOUND, "Not Found"},
    {StatusCode::METHOD_NOT_ALLOWED, "Method Not Allowed"},
    {StatusCode::NONE_ACCEPTABLE, "None Acceptable"},
    {StatusCode::PROXY_AUTHENTICATION_REQUIRED,
     "Proxy Authentication Required"},
    {StatusCode::REQUEST_TIMEOUT, "Request Timeout"},
    {StatusCode::CONFLICT, "Conflict"},
    {StatusCode::GONE, "Gone"},
    {StatusCode::LENGTH_REQUIRED, "Length Required"},
    {StatusCode::UNLESS_TRUE, "Unless True"},
    {StatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {StatusCode::NOT_IMPLEMENTED, "Not Implemented"},
    {StatusCode::BAD_GATEWAY, "Bad Gateway"},
    {StatusCode::SERVICE_UNAVAILABLE, "Service Unavailable"},
    {StatusCode::GATEWAY_TIMEOUT, "Gateway Timeout"},
};

static std::string StatusCodeToText(StatusCode status_code) {
  return status_code_texts.at(status_code);
}

}  // namespace status_codes

}  // namespace http

#endif