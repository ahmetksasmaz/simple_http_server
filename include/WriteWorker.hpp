#ifndef WRITE_WORKER_H_
#define WRITE_WORKER_H_

#include "Worker.hpp"
#include "WriteTask.hpp"

namespace http {

class WriteWorker : public Worker {
 public:
  // TODO Implement
 private:
  // TODO Implement
  void Runner();
  std::shared_ptr<ProtectedQueue<std::shared_ptr<WriteTask>>> writer_queue_ =
      nullptr;
};

}  // namespace http

#endif