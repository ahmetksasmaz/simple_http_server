#ifndef TASK_DESCRIPTION_H_
#define TASK_DESCRIPTION_H_

#include <iostream>
#include <memory>

namespace http {

struct TaskDescription {
  TaskDescription(int event_poll_fd, int client_fd)
      : event_poll_fd_(event_poll_fd), client_fd_(client_fd){};
  int event_poll_fd_;
  int client_fd_;
};
}  // namespace http

#endif