#ifndef WRITE_WORKER_H_
#define WRITE_WORKER_H_

#include <cstring>
#include <iostream>
#include <string>

#include "ProtectedUnorderedMap.hpp"
#include "TaskDescription.hpp"
#include "Worker.hpp"

namespace http {

class WriteWorker : public Worker {
 public:
  WriteWorker(
      std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue,
      std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas);

 private:
  void Runner();
  std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue_ = nullptr;
  std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas_ =
      nullptr;
};

}  // namespace http

#endif