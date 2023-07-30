#ifndef READ_WORKER_H_
#define READ_WORKER_H_

#include <cstring>
#include <iostream>
#include <string>

#include "ProtectedUnorderedMap.hpp"
#include "TaskDescription.hpp"
#include "Worker.hpp"

namespace http {

class ReadWorker : public Worker {
 public:
  ReadWorker(
      std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue,
      std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue,
      std::shared_ptr<ProtectedUnorderedMap<int, std::string>> read_datas);

 private:
  void Runner();
  std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue_ = nullptr;
  std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas_ =
      nullptr;
  static const int buffer_size_ = 4096;
  char buffer_[buffer_size_];
};

}  // namespace http

#endif