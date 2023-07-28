#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <queue>
#include <string>

#include "EventPoolWorker.hpp"
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
  HttpServer(const std::string ip_addr, const int port,
             const int maximum_connections, const int event_fds,
             const int read_worker_num, const int process_worker_num,
             const int write_worker_num);
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
  int maximum_connections_;
  int event_fds_;
  int read_worker_num_;
  int process_worker_num_;
  int write_worker_num_;
  int socket_fd_ = -1;
  utility::Logger logger_;
  std::unique_ptr<EventPoolWorker> event_pool_worker_ = nullptr;
  std::shared_ptr<ProtectedQueue<std::shared_ptr<ReadTask>>> reader_queue_ =
      nullptr;
  std::shared_ptr<ProtectedQueue<std::shared_ptr<ProcessTask>>>
      processor_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<std::shared_ptr<WriteTask>>> writer_queue_ =
      nullptr;
  std::vector<std::unique_ptr<ReadWorker>> reader_workers_;
  std::vector<std::unique_ptr<ProcessWorker>> processor_workers_;
  std::vector<std::unique_ptr<WriteWorker>> writer_workers_;
};

}  // namespace http

#endif