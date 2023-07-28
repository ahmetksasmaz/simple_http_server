#ifndef EVENT_POOL_H_
#define EVENT_POOL_H_

#include "ReadTask.hpp"
#include "Worker.hpp"

namespace http {

class EventPoolWorker : public Worker {
 public:
  // TODO Implement

 private:
  // TODO Implement
  void Runner();
  std::shared_ptr<ProtectedQueue<std::shared_ptr<ReadTask>>> reader_queue_ =
      nullptr;
};

}  // namespace http

#endif