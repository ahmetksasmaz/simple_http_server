#include "ProtectedQueue.hpp"

#include "EventPollWorker.hpp"

template <typename T>
http::ProtectedQueue<T>::ProtectedQueue() {}

template <typename T>
http::ProtectedQueue<T>::ProtectedQueue(
    http::ProtectedQueue<T>&& protected_queue) {
  if (this != &protected_queue) {
    mutex_.lock();
    while (queue_.size()) {
      queue_.pop();
    }
    protected_queue.mutex_.lock();
    while (protected_queue.queue_.size()) {
      T element = protected_queue.queue_.front();
      protected_queue.queue_.pop();
      queue_.push(element);
    }
    protected_queue.mutex_.unlock();
    mutex_.unlock();
  }
}

template <typename T>
http::ProtectedQueue<T>& http::ProtectedQueue<T>::operator=(
    http::ProtectedQueue<T>&& protected_queue) {
  if (this != &protected_queue) {
    mutex_.lock();
    while (queue_.size()) {
      queue_.pop();
    }
    protected_queue.mutex_.lock();
    while (protected_queue.queue_.size()) {
      T element = protected_queue.queue_.front();
      protected_queue.queue_.pop();
      queue_.push(element);
    }
    protected_queue.mutex_.unlock();
    mutex_.unlock();
  }
  return *this;
}

template <typename T>
http::ProtectedQueue<T>::~ProtectedQueue() {
  mutex_.lock();
  while (queue_.size()) {
    queue_.pop();
  }
  mutex_.unlock();
}

template <typename T>
void http::ProtectedQueue<T>::Push(T element) {
  std::unique_lock<std::mutex> lock(mutex_);
  queue_.push(element);
  condition_variable_.notify_one();
}

template <typename T>
T http::ProtectedQueue<T>::Pop() {
  std::unique_lock<std::mutex> lock(mutex_);

  condition_variable_.wait(lock, [this]() { return !queue_.empty(); });

  T element = queue_.front();
  queue_.pop();

  return element;
}

template class http::ProtectedQueue<http::TaskDescription>;