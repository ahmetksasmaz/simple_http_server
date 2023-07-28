#ifndef PROTECTED_QUEUE_H_
#define PROTECTED_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

namespace http {

template <typename T>
class ProtectedQueue {
 public:
  ProtectedQueue();
  ProtectedQueue(ProtectedQueue& queue) =
      delete;  // Popping rhs queue means move
  ProtectedQueue(ProtectedQueue&& queue);
  ProtectedQueue& operator=(ProtectedQueue& queue) =
      delete;  // Popping rhs queue means move
  ProtectedQueue& operator=(ProtectedQueue&& queue);
  ~ProtectedQueue();
  void Push(T element);
  T Pop();

 private:
  std::mutex mutex_;
  std::condition_variable condition_variable_;
  std::queue<T> queue_;
};

}  // namespace http

#endif