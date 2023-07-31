
# Simple Http Server

A simple event-driven multi-threaded HTTP server that has capability of 10000 concurrent connections and 100000+ req/sec with HTTP/1.1 protocol


## Features

- 10000 concurrent connections
- 100000+ req/sec on modern computer
- Event-driven multi-threading architecture
- Route simple HTTP request and create HTTP response

## Design

Configuration parameters are read with a mini YAML parser, then are fed to HTTPServer object. HttpServer object creates a socket and binds ip, initializes read, process and write queues; event poll, read, process and write worker threads and a router. Then starts to listen server socket.

When a connection is established on server socket, HTTPServer object adds to proper event poll's interest list.

When an event is triggered in event poll, firstly an error check is done. Then if event is read, event poll worker creates a read task and push it to the read task queue.

Read worker pops from read task queue and reads data from proper socket. Then sends to the process worker thread, creates a process task and push it to the process task queue.

Process worker pops from process task queue and parse HTTPRequest object from it. Routes this request and stringifies returned HTTPResponse object. Then creates write task and push it to the write task queue.

Write worker pops from write task queue and writes data onto proper socket. Finally triggers an read event for socket.

### Components

- 1 main loop
- 5 event poll worker threads
- 10 read worker threads
- 10 process worker threads
- 10 write worker threads
- 1 router

### Event Poll Worker

Checks whether there is any event triggered on its interest list and route event to proper workers as task.

### Read Worker

Gets read task from read task queue and reads from socket, then creates process task and push it to the process queue.

### Process Worker

Gets process task from process task queue, parse HTTP request, route and process it, create HTTP response. Create task for write worker and push it to the write queue.

### Write Worker

Gets write task from write task queue, write the data to proper socket and trigger an read event for socket.

### Router

Registers handlers on initialization, gets HTTP request and routes proper handler. Return created HTTP response.
## Setup Guide

### Building

```sh
mkdir build
cd build
cmake ..
make
```

### Setting

Set maximum file descriptor per process in Linux

``` sh
ulimit -n 65535
```
### Running

``` sh
cd build
./simple_http_server
```

### Test

``` sh
curl http://0.0.0.0:8080/index.html
```

``` sh
curl --data "Echo!" http://0.0.0.0:8080/echo_content
```
## Performance Benchmark

### Installing Benchmark Tool

``` sh
sudo apt-get install wrk
```

### Benchmark Results

#### Computer Specifications

- CPU : Intel® Core™ i7-10750H CPU @ 2.60GHz × 12
- RAM : 24,0 GiB
- OS : Ubuntu 22.04.2 LTS 64-bit

#### Running

``` sh
wrk -c 10000 -d 10 -t 10 http://0.0.0.0:8080/index.html
```
#### Results

``` sh
Running 10s test @ http://0.0.0.0:8080/index.html
  10 threads and 10000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    39.70ms    2.79ms  80.25ms   81.85%
    Req/Sec    20.60k     2.41k   26.49k    64.44%
  2029730 requests in 10.07s, 199.38MB read
Requests/sec: 201552.58
Transfer/sec:     19.80MB
```
## TODO

- URI regex matching
- Implement other HTTP methods beside GET and POST
- URI parameter parsing
- Copy & Move construstor implementation for remaining classes
- Copy & Move assignment operator implementation for remaining classes