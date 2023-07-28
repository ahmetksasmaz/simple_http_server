#include "HttpServer.hpp"

http::HttpServer::HttpServer(const std::string ip_addr, const int port,
                             const int maximum_connections, const int event_fds,
                             const int read_worker_num,
                             const int process_worker_num,
                             const int write_worker_num)
    : ip_addr_(ip_addr),
      port_(port),
      maximum_connections_(maximum_connections),
      event_fds_(event_fds),
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
  logger_.Debug("# Of  Event Pool File Descriptors:" +
                std::to_string(event_fds_));
  logger_.Debug("# Of Read Workers:" + std::to_string(read_worker_num_));
  logger_.Debug("# Of Processs Workers:" + std::to_string(process_worker_num_));
  logger_.Debug("# Of Write Workers:" + std::to_string(write_worker_num_));
  logger_.Debug("-----------------------");

  reader_queue_ = std::make_shared<ProtectedQueue<std::shared_ptr<ReadTask>>>();
  logger_.Debug("Reader queue created.");

  processor_queue_ =
      std::make_shared<ProtectedQueue<std::shared_ptr<ProcessTask>>>();
  logger_.Debug("Processor queue created.");

  writer_queue_ =
      std::make_shared<ProtectedQueue<std::shared_ptr<WriteTask>>>();
  logger_.Debug("Writer queue created.");

  event_pool_worker_ = std::make_unique<EventPoolWorker>();
  logger_.Debug("Event pool worker created.");

  for (int i = 0; i < read_worker_num_; i++) {
    reader_workers_.push_back(std::make_unique<ReadWorker>());
  }
  logger_.Debug("Read workers created.");

  for (int i = 0; i < process_worker_num_; i++) {
    processor_workers_.push_back(std::make_unique<ProcessWorker>());
  }
  logger_.Debug("Processor workers created.");

  for (int i = 0; i < write_worker_num_; i++) {
    writer_workers_.push_back(std::make_unique<WriteWorker>());
  }
  logger_.Debug("Writer workers created.");

  Start();
}

http::HttpServer::HttpServer(http::HttpServer&& server)
    : ip_addr_(server.ip_addr_),
      port_(server.port_),
      maximum_connections_(server.maximum_connections_),
      event_fds_(server.event_fds_),
      read_worker_num_(server.read_worker_num_),
      process_worker_num_(server.process_worker_num_),
      write_worker_num_(server.write_worker_num_),
      logger_(server.logger_) {
  if (this != &server) {
    event_pool_worker_ = std::move(server.event_pool_worker_);
    reader_queue_ = std::move(server.reader_queue_);
    processor_queue_ = std::move(server.processor_queue_);
    writer_queue_ = std::move(server.writer_queue_);
    reader_workers_.clear();
    processor_workers_.clear();
    writer_workers_.clear();
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
    event_fds_ = server.event_fds_;
    read_worker_num_ = server.read_worker_num_;
    process_worker_num_ = server.process_worker_num_;
    write_worker_num_ = server.write_worker_num_;
    logger_ = server.logger_;
    event_pool_worker_ = std::move(server.event_pool_worker_);
    reader_queue_ = std::move(server.reader_queue_);
    processor_queue_ = std::move(server.processor_queue_);
    writer_queue_ = std::move(server.writer_queue_);
    reader_workers_.clear();
    processor_workers_.clear();
    writer_workers_.clear();
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
  logger_.Info("Server listening on: " + ip_addr_ + ":" +
               std::to_string(port_));

  // TODO Set event pool server socket

  // Start event pool
  event_pool_worker_->Start();

  logger_.Debug("Event pool started.");

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
}
void http::HttpServer::Stop() {
  // Wait for event pool stop
  event_pool_worker_->Stop();
  logger_.Debug("Event pool worker stopped.");

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

  // Close server socket
  close(socket_fd_);
  logger_.Debug("Socket closed.");
}