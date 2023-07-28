#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <memory>
#include <queue>
#include <string>

#include "EventPool.hpp"
#include "ProcessTask.hpp"
#include "ProcessWorker.hpp"
#include "ProtectedQueue.hpp"
#include "ReadTask.hpp"
#include "ReadWorker.hpp"
#include "WriteTask.hpp"
#include "WriteWorker.hpp"
#include "utility/Logger.hpp"

namespace http {

class HttpServer {
 public:
  HttpServer(const std::string ip_addr, const int port);
  HttpServer(HttpServer& server) = delete;  // A Http server should be unique
  HttpServer(HttpServer&& server);
  HttpServer& operator=(HttpServer& server) =
      delete;  // A Http server should be unique
  HttpServer& operator=(HttpServer&& server);
  ~HttpServer();

  void Start();
  void Stop();

 private:
  std::string ip_addr_;
  int port_;
  utility::Logger logger_;
  std::unique_ptr<EventPool> event_pool_;
  std::unique_ptr<ProtectedQueue<std::shared_ptr<ReadTask>>> reader_queue_;
  std::unique_ptr<ProtectedQueue<std::shared_ptr<ProcessTask>>>
      processor_queue_;
  std::unique_ptr<ProtectedQueue<std::shared_ptr<WriteTask>>> writer_queue_;
  std::vector<std::unique_ptr<ReadWorker>> reader_workers_;
  std::vector<std::unique_ptr<ProcessWorker>> processor_workers_;
  std::vector<std::unique_ptr<WriteWorker>> writer_workers_;
};

}  // namespace http

#endif