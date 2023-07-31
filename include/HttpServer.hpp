#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <queue>
#include <string>

#include "EventPollWorker.hpp"
#include "HttpRouter.hpp"
#include "ProcessWorker.hpp"
#include "ProtectedQueue.hpp"
#include "ProtectedUnorderedMap.hpp"
#include "ReadWorker.hpp"
#include "WriteWorker.hpp"
#include "utility/Logger.hpp"

namespace http {

class HttpServer {
 public:
  HttpServer(const std::string ip_addr, const int port,
             const int maximum_connections, const int maximum_events,
             const int event_polls, const int read_worker_num,
             const int process_worker_num, const int write_worker_num);
  HttpServer(HttpServer& server) = delete;  // A Http server should be unique
  HttpServer(HttpServer&& server);
  HttpServer& operator=(HttpServer& server) =
      delete;  // A Http server should be unique
  HttpServer& operator=(HttpServer&& server);
  ~HttpServer();

  void Runner();

  void Start();
  void Stop();

 private:
  std::string ip_addr_;
  int port_;
  int maximum_connections_;
  int maximum_events_;
  int event_poll_num_;
  int read_worker_num_;
  int process_worker_num_;
  int write_worker_num_;
  int socket_fd_ = -1;
  bool running_;
  utility::Logger logger_;
  std::vector<std::unique_ptr<EventPollWorker>> event_poll_workers_;
  std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue_ = nullptr;
  std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas_;
  std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas_;
  std::vector<std::unique_ptr<ReadWorker>> reader_workers_;
  std::vector<std::unique_ptr<ProcessWorker>> processor_workers_;
  std::vector<std::unique_ptr<WriteWorker>> writer_workers_;
  std::shared_ptr<HttpRouter> router_;
  std::unique_ptr<std::thread> thread_;
};

}  // namespace http

#endif