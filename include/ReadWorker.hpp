#ifndef READ_WORKER_H_
#define READ_WORKER_H_

#include "ProcessTask.hpp"
#include "ReadTask.hpp"
#include "Worker.hpp"

namespace http {

class ReadWorker : public Worker {
 public:
  // TODO Implement
 private:
  // TODO Implement
  void Runner();
  std::shared_ptr<ProtectedQueue<std::shared_ptr<ReadTask>>> reader_queue_ =
      nullptr;
  std::shared_ptr<ProtectedQueue<std::shared_ptr<ProcessTask>>>
      processor_queue_ = nullptr;
};

}  // namespace http

#endif