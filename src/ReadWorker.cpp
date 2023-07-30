#include "ReadWorker.hpp"

http::ReadWorker::ReadWorker(
    std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue,
    std::shared_ptr<ProtectedQueue<TaskDescription>> processor_queue,
    std::shared_ptr<ProtectedUnorderedMap<int, std::string>> request_datas)
    : reader_queue_(reader_queue),
      processor_queue_(processor_queue),
      request_datas_(request_datas),
      Worker(utility::Logger("ReadWorker")) {}

void http::ReadWorker::Runner() {
  running_ = true;
  int counter = 0;
  while (running_) {
    TaskDescription read_task = reader_queue_.get()->Pop();

    ssize_t byte_count =
        recv(read_task.client_fd_, buffer_, buffer_size_, MSG_NOSIGNAL);
    if (byte_count > 0) {
      std::string request_str{buffer_};
      TaskDescription process_task(read_task.event_poll_fd_,
                                   read_task.client_fd_);
      request_datas_.get()->Push(
          std::pair<int, std::string>(read_task.client_fd_, request_str));
      processor_queue_.get()->Push(process_task);
      logger_.Debug("Task pushed to processor queue.");
    } else if (byte_count == 0) {
      epoll_ctl(read_task.event_poll_fd_, EPOLL_CTL_DEL, read_task.client_fd_,
                NULL);

      close(read_task.client_fd_);
      logger_.Info("Client disconnected.");
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        reader_queue_.get()->Push(read_task);
        logger_.Info("Retrying read again.");
      } else {
        epoll_ctl(read_task.event_poll_fd_, EPOLL_CTL_DEL, read_task.client_fd_,
                  NULL);

        close(read_task.client_fd_);
        logger_.Warn("Some errors occured while reading, closed connection.");
      }
    }
  }
}