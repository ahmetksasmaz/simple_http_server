#include "ProcessWorker.hpp"

http::ProcessWorker::ProcessWorker(
    std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue,
    std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue,
    std::shared_ptr<HttpRouter> router,
    std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas,
    std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas)
    : processor_queue_(processor_queue),
      writer_queue_(writer_queue),
      router_(router),
      request_datas_(request_datas),
      response_datas_(response_datas),
      Worker(utility::Logger("ProcessWorker")) {}
void http::ProcessWorker::Runner() {
  running_ = true;
  while (running_) {
    TaskDescription process_task = processor_queue_.get()->Pop();
    std::string request_str =
        request_datas_.get()->FindAndRemove(process_task.client_fd_);
    HttpRequest *request = HttpRequest::Parse(request_str);
    HttpResponse response = router_.get()->ExecuteRouter(*request);
    delete request;
    std::string response_str = response.Stringify();

    response_datas_.get()->Push(
        std::pair<int, std::string>(process_task.client_fd_, response_str));

    epoll_event new_event;
    new_event.events = EPOLLOUT | EPOLLET;
    new_event.data.fd = process_task.client_fd_;

    epoll_ctl(process_task.event_poll_fd_, EPOLL_CTL_MOD,
              process_task.client_fd_, &new_event);
  }
}