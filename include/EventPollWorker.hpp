#ifndef EVENT_POLL_WORKER_H_
#define EVENT_POLL_WORKER_H_

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

#include "ProtectedQueue.hpp"
#include "TaskDescription.hpp"
#include "Worker.hpp"

namespace http {

class EventPollWorker : public Worker {
 public:
  EventPollWorker(
      int maximum_events,
      std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue,
      std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue);
  EventPollWorker(EventPollWorker& event_poll);
  EventPollWorker(EventPollWorker&& event_poll);
  EventPollWorker& operator=(EventPollWorker& event_poll);
  EventPollWorker& operator=(EventPollWorker&& event_poll);
  ~EventPollWorker();

  int GetEventPollSocket();

 private:
  void Runner();
  std::shared_ptr<ProtectedQueue<TaskDescription>> reader_queue_ = nullptr;
  std::shared_ptr<ProtectedQueue<TaskDescription>> writer_queue_ = nullptr;
  std::unique_ptr<epoll_event[]> epoll_events_ = nullptr;
  int epoll_socket_fd_;
  int maximum_events_;
};

}  // namespace http

#endif