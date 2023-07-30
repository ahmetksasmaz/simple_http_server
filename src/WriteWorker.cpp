#include "WriteWorker.hpp"

#include <fstream>

http::WriteWorker::WriteWorker(
    std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue,
    std::shared_ptr<ProtectedUnorderedMap<int, std::string>> response_datas)
    : writer_queue_(writer_queue),
      response_datas_(response_datas),
      Worker(utility::Logger("WriteWorker")) {}

void http::WriteWorker::Runner() {
  running_ = true;
  while (running_) {
    TaskDescription write_task = writer_queue_.get()->Pop();
    logger_.Debug("Writing response to socket : " +
                  std::to_string(write_task.client_fd_));
    std::string data =
        response_datas_.get()->FindAndRemove(write_task.client_fd_);
    int data_size = data.size();
    int written =
        send(write_task.client_fd_, data.c_str(), data_size, MSG_NOSIGNAL);
    int total_written = 0;
    if (written > 0) {
      total_written += written;
      while (total_written != data_size) {
        written = send(write_task.client_fd_, &(data.c_str()[total_written]),
                       data_size - total_written, MSG_NOSIGNAL);
        if (written <= 0) {
          break;
        }
        total_written += written;
      }
    }
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      writer_queue_.get()->Push(write_task);
      logger_.Info("Retrying write again.");
      continue;
    } else if (total_written != data_size) {
      // Connection reset by peer.
      epoll_ctl(write_task.event_poll_fd_, EPOLL_CTL_DEL, write_task.client_fd_,
                NULL);
      close(write_task.client_fd_);
      continue;
    }

    epoll_event new_event;
    new_event.events = EPOLLIN | EPOLLET;
    new_event.data.fd = write_task.client_fd_;
    epoll_ctl(write_task.event_poll_fd_, EPOLL_CTL_MOD, write_task.client_fd_,
              &new_event);

    logger_.Debug("Response written successfully");
  }
}