
#include "Worker.hpp"

http::Worker::Worker(utility::Logger logger) : logger_(logger) {}

http::Worker::Worker(http::Worker& worker) : logger_(worker.logger_) {}

http::Worker::Worker(http::Worker&& worker) : logger_(worker.logger_) {
  if (this != &worker) {
  }
}

http::Worker& http::Worker::operator=(http::Worker& worker) {}

http::Worker& http::Worker::operator=(http::Worker&& worker) {}

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