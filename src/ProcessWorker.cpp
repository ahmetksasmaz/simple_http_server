#include "ProcessWorker.hpp"

http::ProcessWorker::ProcessWorker(
    std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue,
    std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue,
    std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas,
    std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas)
    : processor_queue_(processor_queue),
      writer_queue_(writer_queue),
      request_datas_(request_datas),
      response_datas_(response_datas),
      Worker(utility::Logger("ProcessWorker")) {}
void http::ProcessWorker::Runner() {
  running_ = true;
  int counter = 0;
  while (running_) {
    TaskDescription process_task = processor_queue_.get()->Pop();
    std::string request_str =
        request_datas_.get()->FindAndRemove(process_task.client_fd_);
    // TODO Implement
    HttpRequest request = HttpRequest::Parse(request_str);
    // Handle
    // HttpResponse response();
    // response.Stringify();

    // TODO Remove this section

    std::string response_str = "HTTP/1.1 200 OK\n";
    response_str += "Date: Mon, 27 Jul 2009 12:28:53 GMT\n";
    response_str += "Server: Apache/2.2.14 (Win32)\n";
    response_str += "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n";
    response_str += "Content-Length: 60\n";
    response_str += "Content-Type: text/html\n";
    response_str += "Connection: Closed\r\n\r\n";
    response_str += "<html>\n";
    response_str += "<body>\n";
    response_str += "<h1>Hello,World!" + std::to_string(counter++) + "</h1>\n";
    response_str += "</body>\n";
    response_str += "</html>\n\n\n\n\n\n\n\n\n\n";
    // TODO Remove this section

    response_datas_.get()->Push(
        std::pair<int, std::string>(process_task.client_fd_, response_str));

    epoll_event new_event;
    new_event.events = EPOLLOUT | EPOLLET;
    new_event.data.fd = process_task.client_fd_;

    epoll_ctl(process_task.event_poll_fd_, EPOLL_CTL_MOD,
              process_task.client_fd_, &new_event);
  }
}