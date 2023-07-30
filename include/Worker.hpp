#ifndef WORKER_H_
#define WORKER_H_

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <functional>
#include <thread>

#include "ProtectedQueue.hpp"
#include "utility/Logger.hpp"

namespace http {

class Worker {
 public:
  Worker(utility::Logger logger);
  Worker(Worker& worker);
  Worker(Worker&& worker);
  Worker& operator=(Worker& worker);
  Worker& operator=(Worker&& worker);
  ~Worker();

  void Start();
  void Stop();

 protected:
  virtual void Runner() = 0;
  bool running_ = false;
  utility::Logger logger_;

 private:
  std::unique_ptr<std::thread> thread_ = nullptr;
};

}  // namespace http

#endif