#ifndef HTTP_ROUTER_H_
#define HTTP_ROUTER_H_

#include <fstream>
#include <functional>
#include <map>

#include "HttpEnumerations.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

namespace http {
class HttpRouter {
 public:
  HttpRouter();
  ~HttpRouter();

  void RegisterRoute(methods::Method method, std::string uri,
                     std::function<HttpResponse(HttpRequest)> handler);

  HttpResponse ExecuteRouter(HttpRequest request);

 private:
  bool CheckResourceValid(std::string uri);
  std::map<std::pair<methods::Method, std::string>,
           std::function<HttpResponse(HttpRequest)>>
      routes_;
};

}  // namespace http

#endif