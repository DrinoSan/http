//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
#define SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H

// System Headers
#include <arpa/inet.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>


// Project Headers
#include "HttpMessage_t.h"
#include "HttpParser_t.h"
#include "HttpRequest_t.h"
#include "HttpResponse_t.h"

// Namespaces
namespace fs = std::filesystem;

// Defining of some values
constexpr int BACK_LOG = 10000;
constexpr int NUM_WORKERS = 5;
constexpr int NUM_EVENTS = 100;
constexpr int CHUNK_SIZE = 1000;

using HttpRequestHandler_t =
		std::function<HttpResponse_t(HttpRequest_t&)>;

class SimpleHttpServer_t
{
public:
	SimpleHttpServer_t();

	~SimpleHttpServer_t() = default;

	void registerRequestHandler(std::string uri,
			HttpRequest_t::HttpMethode methode,
			HttpRequestHandler_t callback);

	bool startServer(std::string ipAddr = "", int64_t port = 8000);

	void serve_static_file(const fs::path& root_dir, const std::string& path,
			std::ostringstream& stream, size_t& fileSize, HttpResponse_t& response);

	HttpRequestHandler_t fileServer(std::string rootDir);

	HttpRequestHandler_t stripPrefix(const std::string prefix, HttpRequestHandler_t handler);

private:
	int setNonBlocking(int sockfd);

	void listen_and_accept();

	void prepare_kqueue_events();

	void process_worker_events(int worker_idx);

	std::vector<std::string> split_path(const std::string& path, char delimiter);

private:
	SSL_CTX *ssl_context;
	SSL *ssl;

	HttpParser_t httpParser;
	std::map<std::string, std::string> http_request;
	std::filesystem::path root;
	std::string fileServerPath;
	std::map<std::string,
			std::map<HttpRequest_t::HttpMethode, HttpRequestHandler_t>>
			requestHandler;

	struct sockInfos_t
	{
		int sockfd;
	};

private:
	HttpRequest_t handle_read(struct sockInfos_t* sockInfo);

	void handle_write(struct sockInfos_t* sockInfo, HttpRequest_t httpRequest);

	sockInfos_t listeningSocket;

	// Kqueue stuff
	int working_kqueue_fd[NUM_WORKERS];
	struct kevent working_events[NUM_WORKERS][NUM_EVENTS];
	struct kevent working_chevents[NUM_WORKERS][NUM_EVENTS];

	std::thread listenerThread;
	std::thread workerThread[NUM_WORKERS];
	// Todo: Fix unused variable
	[[maybe_unused]] int kq;

	// Handler for not registerd paths
	HttpResponse_t pageNotFound(HttpRequest_t* httpReq);


	int sockfd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];
};

#endif  // SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
