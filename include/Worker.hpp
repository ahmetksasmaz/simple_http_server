#ifndef WORKER_H_
#define WORKER_H_

#include <functional>
#include <thread>

#include "ProtectedQueue.hpp"
#include "Task.hpp"

namespace http {

class Worker {
 public:
  // TODO Implement
  ~Worker();
  void Start();
  void Stop();

 protected:
  virtual void Runner() = 0;
  bool running_ = false;

 private:
  // TODO Implement
  std::unique_ptr<std::thread> thread_ = nullptr;
};

}  // namespace http

#endif