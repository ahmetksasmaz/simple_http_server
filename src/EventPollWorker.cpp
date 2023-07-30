#include "EventPollWorker.hpp"

http::EventPollWorker::EventPollWorker(
    int maximum_events,
    std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue,
    std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue)
    : maximum_events_(maximum_events),
      reader_queue_(reader_queue),
      writer_queue_(writer_queue),
      Worker(utility::Logger("EventPollWorker")) {
  epoll_socket_fd_ = epoll_create1(0);
  if (epoll_socket_fd_ < 0) {
    logger_.Error("Socket cannot be created.");
    exit(1);
  }

  logger_.Debug("Socket created.");

  epoll_events_ = std::make_unique<epoll_event[]>(maximum_events_);
}

http::EventPollWorker::EventPollWorker(http::EventPollWorker& event_poll)
    : Worker(event_poll.logger_) {}

http::EventPollWorker::EventPollWorker(http::EventPollWorker&& event_poll)
    : Worker(event_poll.logger_) {
  if (this != &event_poll) {
  }
}

http::EventPollWorker& http::EventPollWorker::operator=(
    http::EventPollWorker& event_poll) {}

http::EventPollWorker& http::EventPollWorker::operator=(
    http::EventPollWorker&& event_poll) {}

http::EventPollWorker::~EventPollWorker() {
  close(epoll_socket_fd_);
  logger_.Debug("Socket closed.");
}

void http::EventPollWorker::Runner() {
  running_ = true;
  while (running_) {
    // Iterate triggered events

    int number_of_triggered_fds =
        epoll_wait(epoll_socket_fd_, epoll_events_.get(), maximum_events_, -1);

    for (int i = 0; i < number_of_triggered_fds; i++) {
      epoll_event& event = epoll_events_.get()[i];

      if ((event.events & EPOLLHUP) || (event.events & EPOLLERR)) {
        // Unwanted event occured, remove client fd from interest list
        epoll_ctl(epoll_socket_fd_, EPOLL_CTL_DEL, event.data.fd, NULL);
        close(event.data.fd);
        logger_.Debug(
            "Unwanted event occured, client socket removed from interest "
            "list.");
      } else if (event.events & EPOLLIN) {
        // A client socket is triggered as read, create task and push to proper
        // queue
        TaskDescription read_task(epoll_socket_fd_, event.data.fd);
        reader_queue_.get()->Push(read_task);
        logger_.Debug("Task pushed to reader queue.");
      } else if (event.events & EPOLLOUT) {
        // A client socket is triggered as write, create task and push to proper
        // queue
        TaskDescription write_task(epoll_socket_fd_, event.data.fd);
        writer_queue_.get()->Push(write_task);
        logger_.Debug("Task pushed to writer queue.");
      } else {
        // Unwanted event occured, remove client fd from interest list
        epoll_ctl(epoll_socket_fd_, EPOLL_CTL_DEL, event.data.fd, NULL);
        close(event.data.fd);
        logger_.Debug(
            "Unwanted event occured, client socket removed from interest "
            "list.");
      }
    }
  }
}

int http::EventPollWorker::GetEventPollSocket() { return epoll_socket_fd_; }