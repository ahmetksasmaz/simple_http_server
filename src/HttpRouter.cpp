#include "HttpRouter.hpp"

http::HttpRouter::HttpRouter() {}

http::HttpRouter::~HttpRouter() { routes_.clear(); }

void http::HttpRouter::RegisterRoute(
    http::methods::Method method, std::string uri,
    std::function<HttpResponse(HttpRequest)> handler) {
  routes_[std::pair<http::methods::Method, std::string>(method, uri)] = handler;
}

http::HttpResponse http::HttpRouter::ExecuteRouter(http::HttpRequest request) {
  http::methods::Method method = request.GetMethod();
  std::string uri = request.GetRequestURI();
  try {
    auto handler =
        routes_.at(std::pair<methods::Method, std::string>(method, uri));
    return handler(request);
  } catch (...) {
    // Method and URI pair not found
    // Check if there is any registered resource or file
    if (CheckResourceValid(uri)) {
      // Method not allowed
      HttpResponse method_not_allowed_response(
          http::status_codes::StatusCode::METHOD_NOT_ALLOWED,
          http::protocols::HTTP_1_1);
      method_not_allowed_response.AddHeader(headers::SERVER,
                                            "Simple-Http-Server");
      method_not_allowed_response.AddHeader(headers::DATE,
                                            HttpResponse::UTCDate());
      return method_not_allowed_response;
    } else {
      // 404 Not Found
      HttpResponse not_found_response(http::status_codes::StatusCode::NOT_FOUND,
                                      http::protocols::HTTP_1_1);
      not_found_response.AddHeader(headers::SERVER, "Simple-Http-Server");
      not_found_response.AddHeader(headers::DATE, HttpResponse::UTCDate());
      not_found_response.AddHeader(headers::CONTENT_TYPE, "text/html");
      return not_found_response;
    }
  }
}

bool http::HttpRouter::CheckResourceValid(std::string uri) {
  // Check whether there is a file
  std::ifstream file("../www" + uri);
  if (file.good()) {
    return true;
  }
  // Check whether there is a register
  for (auto &registered_route : routes_) {
    if (registered_route.first.second == uri) {
      return true;
    }
  }
  return false;
}