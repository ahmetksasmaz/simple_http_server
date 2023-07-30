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
  while (running_) {
    TaskDescription process_task = processor_queue_.get()->Pop();
    std::string request_str =
        request_datas_.get()->FindAndRemove(process_task.client_fd_);
    // TODO Implement
    HttpRequest request = HttpRequest::Parse(request_str);
    // Handle
    HttpResponse dummy_response(status_codes::OK, protocols::HTTP_1_1);

    dummy_response.AddHeader(headers::CONTENT_TYPE, "text/html");
    dummy_response.AddHeader(headers::SERVER, "Simple-Http-Server");
    dummy_response.AddHeader(headers::DATE, HttpResponse::UTCDate());

    std::string entity_body = "";
    entity_body += "<html>\n";
    entity_body += "<body>\n";
    entity_body +=
        "<h1>Hello there! Time as UTC : " + HttpResponse::UTCDate() + "</h1>\n";
    entity_body += "</body>\n";
    entity_body += "</html>";

    dummy_response.SetEntityBody(entity_body);

    std::string response_str = dummy_response.Stringify();

    response_datas_.get()->Push(
        std::pair<int, std::string>(process_task.client_fd_, response_str));

    epoll_event new_event;
    new_event.events = EPOLLOUT | EPOLLET;
    new_event.data.fd = process_task.client_fd_;

    epoll_ctl(process_task.event_poll_fd_, EPOLL_CTL_MOD,
              process_task.client_fd_, &new_event);
  }
}