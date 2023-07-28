#include "HttpServer.hpp"

http::HttpServer::HttpServer(const std::string ip_addr, const int port)
    : ip_addr_(ip_addr), port_(port), logger_(utility::Logger("HttpServer")) {
  // TODO Implement
}

http::HttpServer::HttpServer(http::HttpServer&& server)
    : ip_addr_(server.ip_addr_), port_(server.port_), logger_(server.logger_) {
  if (this != &server) {
    event_pool_ = std::move(server.event_pool_);
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
    logger_ = server.logger_;
    event_pool_ = std::move(server.event_pool_);
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
  // TODO Implement
}
void http::HttpServer::Stop() {
  // TODO Implement
}