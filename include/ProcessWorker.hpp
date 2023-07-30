#ifndef PROCESS_WORKER_H_
#define PROCESS_WORKER_H_

#include <cstring>
#include <iostream>
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ProtectedUnorderedMap.hpp"
#include "TaskDescription.hpp"
#include "Worker.hpp"

namespace http {

class ProcessWorker : public Worker {
 public:
  ProcessWorker(
      std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue,
      std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue,
      std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas,
      std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas);

 private:
  void Runner();
  std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue_ = nullptr;
  std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas_ =
      nullptr;
  std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas_ =
      nullptr;
};

}  // namespace http

#endif