#include "HttpServer.hpp"

http::HttpServer::HttpServer(const std::string ip_addr, const int port,
                             const int maximum_connections,
                             const int maximum_events, const int event_poll_num,
                             const int read_worker_num,
                             const int process_worker_num,
                             const int write_worker_num)
    : ip_addr_(ip_addr),
      port_(port),
      maximum_connections_(maximum_connections),
      maximum_events_(maximum_events),
      event_poll_num_(event_poll_num),
      read_worker_num_(read_worker_num),
      process_worker_num_(process_worker_num),
      write_worker_num_(write_worker_num),
      logger_(utility::Logger("HttpServer")) {
  logger_.Debug("-----------------------");
  logger_.Debug("Server parameters:");
  logger_.Debug("IP Address:" + ip_addr_);
  logger_.Debug("Port:" + std::to_string(port_));
  logger_.Debug("# Of Maximum Connections Listened:" +
                std::to_string(maximum_connections_));
  logger_.Debug("# Of Maximum Events:" + std::to_string(maximum_events_));
  logger_.Debug("# Of Read Workers:" + std::to_string(read_worker_num_));
  logger_.Debug("# Of Processs Workers:" + std::to_string(process_worker_num_));
  logger_.Debug("# Of Write Workers:" + std::to_string(write_worker_num_));
  logger_.Debug("-----------------------");

  router_ = std::make_shared<HttpRouter>();
  logger_.Debug("Router created.");

  // Registering routes
  router_.get()->RegisterRoute(
      http::methods::GET, "/index.html",
      [](http::HttpRequest request) -> http::HttpResponse {
        std::ifstream file("../www/index.html");
        HttpResponse response(http::status_codes::OK,
                              http::protocols::HTTP_1_1);
        response.AddHeader(headers::SERVER, "Simple-Http-Server");
        response.AddHeader(headers::DATE, HttpResponse::UTCDate());

        std::ostringstream ss;
        ss << file.rdbuf();
        response.SetEntityBody(ss.str());

        return response;
      });
  router_.get()->RegisterRoute(
      http::methods::POST, "/echo_content",
      [](http::HttpRequest request) -> http::HttpResponse {
        HttpResponse response(http::status_codes::OK,
                              http::protocols::HTTP_1_1);
        response.AddHeader(headers::SERVER, "Simple-Http-Server");
        response.AddHeader(headers::DATE, HttpResponse::UTCDate());
        std::string response_str = "<html>\n";
        response_str += "<head></head>\n";
        response_str += "<body>\n";
        response_str += "<h1>\n";
        response_str += "Echo : " + request.GetEntityBody() + "\n";
        response_str += "</h1>\n";
        response_str += "</body>\n";
        response_str += "</html>\n";
        response.SetEntityBody(response_str);

        return response;
      });

  reader_queue_ = std::make_shared<ProtectedQueue<TaskDescription>>();
  logger_.Debug("Reader queue created.");

  processor_queue_ = std::make_shared<ProtectedQueue<TaskDescription>>();
  logger_.Debug("Processor queue created.");

  writer_queue_ = std::make_shared<ProtectedQueue<TaskDescription>>();
  logger_.Debug("Writer queue created.");

  request_datas_ = std::make_shared<ProtectedUnorderedMap<int, std::string>>();
  logger_.Debug("Request data unordered map created.");

  response_datas_ = std::make_shared<ProtectedUnorderedMap<int, std::string>>();
  logger_.Debug("Response data unordered map created.");

  for (int i = 0; i < event_poll_num_; i++) {
    event_poll_workers_.push_back(std::make_unique<EventPollWorker>(
        maximum_events_, reader_queue_, writer_queue_));
  }
  logger_.Debug("Event poll workers created.");

  for (int i = 0; i < read_worker_num_; i++) {
    reader_workers_.push_back(std::make_unique<ReadWorker>(
        reader_queue_, processor_queue_, request_datas_));
  }
  logger_.Debug("Read workers created.");

  for (int i = 0; i < process_worker_num_; i++) {
    processor_workers_.push_back(std::make_unique<ProcessWorker>(
        processor_queue_, writer_queue_, router_, request_datas_,
        response_datas_));
  }
  logger_.Debug("Processor workers created.");

  for (int i = 0; i < write_worker_num_; i++) {
    writer_workers_.push_back(
        std::make_unique<WriteWorker>(writer_queue_, response_datas_));
  }
  logger_.Debug("Writer workers created.");

  Start();
}

http::HttpServer::HttpServer(http::HttpServer&& server)
    : ip_addr_(server.ip_addr_),
      port_(server.port_),
      maximum_connections_(server.maximum_connections_),
      maximum_events_(server.maximum_events_),
      read_worker_num_(server.read_worker_num_),
      process_worker_num_(server.process_worker_num_),
      write_worker_num_(server.write_worker_num_),
      logger_(server.logger_) {
  if (this != &server) {
    reader_queue_ = std::move(server.reader_queue_);
    processor_queue_ = std::move(server.processor_queue_);
    writer_queue_ = std::move(server.writer_queue_);
    reader_workers_.clear();
    processor_workers_.clear();
    writer_workers_.clear();
    for (auto& event_poll_worker : server.event_poll_workers_) {
      event_poll_workers_.push_back(std::move(event_poll_worker));
    }
    for (auto& reader_worker : server.reader_workers_) {
      reader_workers_.push_back(std::move(reader_worker));
    }
    for (auto& processor_worker : server.processor_workers_) {
      processor_workers_.push_back(std::move(processor_worker));
    }
    for (auto& writer_worker : server.writer_workers_) {
      writer_workers_.push_back(std::move(writer_worker));
    }
  }
}

http::HttpServer& http::HttpServer::operator=(http::HttpServer&& server) {
  if (this != &server) {
    ip_addr_ = server.ip_addr_;
    port_ = server.port_;
    maximum_connections_ = server.maximum_connections_;
    maximum_events_ = server.maximum_events_;
    read_worker_num_ = server.read_worker_num_;
    process_worker_num_ = server.process_worker_num_;
    write_worker_num_ = server.write_worker_num_;
    logger_ = server.logger_;
    reader_queue_ = std::move(server.reader_queue_);
    processor_queue_ = std::move(server.processor_queue_);
    writer_queue_ = std::move(server.writer_queue_);
    reader_workers_.clear();
    processor_workers_.clear();
    writer_workers_.clear();
    for (auto& event_poll_worker : server.event_poll_workers_) {
      event_poll_workers_.push_back(std::move(event_poll_worker));
    }
    for (auto& reader_worker : server.reader_workers_) {
      reader_workers_.push_back(std::move(reader_worker));
    }
    for (auto& processor_worker : server.processor_workers_) {
      processor_workers_.push_back(std::move(processor_worker));
    }
    for (auto& writer_worker : server.writer_workers_) {
      writer_workers_.push_back(std::move(writer_worker));
    }
  }
  return *this;
}

http::HttpServer::~HttpServer() { Stop(); }

void http::HttpServer::Start() {
  int ret;

  // Create socket
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    logger_.Error("Socket cannot be created.");
    exit(1);
  }

  logger_.Debug("Socket created.");

  // Set socket options
  int opt = 1;

  ret = setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt));

  if (ret < 0) {
    logger_.Error("Socket cannot be set.");
    exit(1);
  }

  logger_.Debug("Socket is set.");

  // Bind socket
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  inet_pton(AF_INET, ip_addr_.c_str(), &(server_addr.sin_addr.s_addr));
  server_addr.sin_port = htons(port_);

  ret = bind(socket_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (ret < 0) {
    logger_.Error("Socket cannot bind.");
    exit(1);
  }

  logger_.Debug("Socket bind.");

  // Listen socket
  ret = listen(socket_fd_, maximum_connections_);
  if (ret < 0) {
    logger_.Error("Socket cannot listen.");
    exit(1);
  }

  logger_.Debug("Socket is listening.");

  int flags = fcntl(socket_fd_, F_GETFL, 0);
  fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK);

  logger_.Debug("Socket set to non blocking mode.");

  logger_.Info("Server listening on: " + ip_addr_ + ":" +
               std::to_string(port_));

  // Start event poll workers

  for (auto& event_poll_worker : event_poll_workers_) {
    event_poll_worker->Start();
  }

  logger_.Debug("Event poll workers started.");

  // Start workers
  for (auto& reader_worker : reader_workers_) {
    reader_worker->Start();
  }
  logger_.Debug("Reader workers started.");
  for (auto& processor_worker : processor_workers_) {
    processor_worker->Start();
  }
  logger_.Debug("Processor workers started.");
  for (auto& writer_worker : writer_workers_) {
    writer_worker->Start();
  }
  logger_.Debug("Writer workers started.");

  thread_ = std::make_unique<std::thread>(&http::HttpServer::Runner, this);
}
void http::HttpServer::Stop() {
  // Wait for event poll workers stop
  for (auto& event_poll_worker : event_poll_workers_) {
    event_poll_worker->Stop();
  }
  logger_.Debug("Event poll workers stopped.");

  // Wait for workers stop
  for (auto& reader_worker : reader_workers_) {
    reader_worker->Stop();
  }
  logger_.Debug("Reader workers stopped.");

  for (auto& processor_worker : processor_workers_) {
    processor_worker->Stop();
  }
  logger_.Debug("Processor workers stopped.");

  for (auto& writer_worker : writer_workers_) {
    writer_worker->Stop();
  }
  logger_.Debug("Writer workers stopped.");

  running_ = false;
  if (thread_ != nullptr) {
    if (thread_->joinable()) {
      thread_->join();
    }
  }

  // Close server socket
  close(socket_fd_);
  logger_.Debug("Socket closed.");
}

void http::HttpServer::Runner() {
  running_ = true;
  int event_poll_counter = 0;
  while (running_) {
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd =
        accept(socket_fd_, (sockaddr*)&client_addr, &client_addr_len);
    logger_.Info("Client accepted " +
                 std::string{inet_ntoa(client_addr.sin_addr)} + ":" +
                 std::to_string(client_addr.sin_port));
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    logger_.Debug("Client socket set to non blocking mode : " +
                  std::to_string(client_fd));

    epoll_event new_event;
    new_event.events = EPOLLIN | EPOLLET;
    new_event.data.fd = client_fd;
    epoll_ctl(event_poll_workers_[event_poll_counter]->GetEventPollSocket(),
              EPOLL_CTL_ADD, client_fd, &new_event);
    event_poll_counter = ++event_poll_counter % event_poll_num_;
    logger_.Debug("Client socket is added to interest list.");
  }
}