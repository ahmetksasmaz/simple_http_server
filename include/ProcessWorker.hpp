#ifndef PROCESS_WORKER_H_
#define PROCESS_WORKER_H_

#include "ProcessTask.hpp"
#include "Worker.hpp"
#include "WriteTask.hpp"

namespace http {

class ProcessWorker : public Worker {
 public:
  // TODO Implement
 private:
  // TODO Implement
  void Runner();
  std::shared_ptr<ProtectedQueue<std::shared_ptr<ProcessTask>>>
      processor_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<std::shared_ptr<WriteTask>>> writer_queue_ =
      nullptr;
};

}  // namespace http

#endif