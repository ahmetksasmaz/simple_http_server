// TODO Implement

#include "Worker.hpp"

http::Worker::~Worker() { Stop(); }

void http::Worker::Start() {
  running_ = true;
  if (thread_ != nullptr) {
    thread_->join();
  }
  thread_ = std::make_unique<std::thread>(&http::Worker::Runner, this);
}

void http::Worker::Stop() {
  running_ = false;
  if (thread_ != nullptr) {
    if (thread_->joinable()) thread_->join();
  }
}