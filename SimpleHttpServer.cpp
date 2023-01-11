//
// Created by becirbeg on 22.10.2022.
//

// System Headers
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <utility>
#include <signal.h>

// Project Headers
#include "HttpRequest_t.h"
#include "HttpResponse_t.h"
#include "SimpleHttpServer.h"

std::atomic<int> counter{ 0 };

//----------------------------------------------------------------------------
void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//----------------------------------------------------------------------------
SimpleHttpServer_t::SimpleHttpServer_t()
{
	kq = kqueue();
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::registerRequestHandler(
		std::string uri, HttpRequest_t::HttpMethode methode,
		HttpRequestHandler_t callback)
{
	requestHandler[uri].insert(std::make_pair(methode, callback));
}

//----------------------------------------------------------------------------
int SimpleHttpServer_t::setNonBlocking(int sockfd)
{
	int flags, s;
	flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		return -1;
	}
	flags |= O_NONBLOCK;
	s = fcntl(sockfd, F_SETFL, flags);
	if (s == -1)
	{
		perror("fcntl");
		return -1;
	}
	return 0;
}

volatile sig_atomic_t flag = 0;

void cleanup(int sig)
{
	flag = 1;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::listen_and_accept()
{
	int worker_idx{ 0 };

//	signal(SIGINT, cleanup);

	while (true)
	{
		auto* newSocketConnection = new sockInfos_t;
		newSocketConnection->sockfd =
				accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
		++counter;
		if (newSocketConnection->sockfd == -1)
		{
			std::cout << "Accept socket failed" << std::endl;
			delete newSocketConnection;
			--counter;
			continue;
		}

		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
		std::cout << "Server: got connection from " << s << std::endl;

		// Put this new socket connection also as a 'filter' event
		// to watch in kqueue, so we can now watch for events on this
		// new socket.
		EV_SET(working_chevents[worker_idx], newSocketConnection->sockfd,
				EVFILT_READ, EV_ADD, 0, 0, newSocketConnection);
		if (kevent(working_kqueue_fd[worker_idx], working_chevents[worker_idx], 1,
				NULL, 0, NULL) < 0)
		{
			std::cout << "kevent error 3: " << errno << std::endl;
			if(errno != 0)
			{
				exit(EXIT_FAILURE);
			}
			std::cout << "Kevent returned -1, but we will ignore this because errno indicates success" << std::endl;
		}

		++worker_idx;

		if (worker_idx == NUM_WORKERS)
			worker_idx = 0;

//		if(flag)
//		{
//			std::cout << "GOT SIGINT in listen" << std::endl;
//			break;
//		}
	}
	std::cout << "EXITING LISTEN FUNCTION" << std::endl;

	// Make me reachable
	// Need to listen for cancel events to exit the loop
	close(listeningSocket.sockfd);
}

//----------------------------------------------------------------------------
HttpRequest_t SimpleHttpServer_t::handle_read(
		SimpleHttpServer_t::sockInfos_t* sockInfo)
{
	HttpRequest_t httpRequest;
	memset(httpRequest.buffer, '\0', BUFFER_SIZE);


	int bytesRead;
	while((bytesRead = recv(sockInfo->sockfd, httpRequest.buffer, BUFFER_SIZE, 0)))
	{
		if(bytesRead <= 0)
		{
			// We have a error or client closed connection
			break;
		}
		if(strstr(httpRequest.buffer, "\r\n\r\n"))
		{
			// We found end
			break;
		}
	}

	httpParser.mapHeaders(httpRequest);

	return httpRequest;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::handle_write(SimpleHttpServer_t::sockInfos_t* sockInfo,
		HttpRequest_t httpRequest)
{
	auto tokens = split_path(httpRequest.httpUri, '/');

	// Debug
	for (const auto& token: tokens)
		std::cout << "TOKEN: " << token << std::endl;

	std::string uri{ httpRequest.httpUri };
	if (tokens.size() > 1)
	{
		std::cout << "We found a sub url" << std::endl;

		// Check if static. If not, we probably have a nested path: /home/foo/bar
		if (tokens[0] == fileServerPath)
		{
			// Index 0 should be the basic path
			uri = "/" + tokens[0] + "/";
			for (size_t i = 1; i < tokens.size(); ++i)
			{
				if (i == tokens.size() - 1)
				{
					httpRequest.resource += tokens[i];
					break;
				}
				httpRequest.resource += tokens[i] + "/";
			}
		}
	}

	auto it_uri = requestHandler.find(uri);
	if (it_uri == requestHandler.end())
	{
		HttpResponse_t httpResponse = pageNotFound(&httpRequest);
		send(sockInfo->sockfd, httpResponse.httpMessage.data(),
				httpResponse.httpMessageLength, 0);
		return;
	}

	auto it_methode = it_uri->second.find(httpRequest.httpMethode);
	if (it_methode == it_uri->second.end())
	{
		std::cout << "Methode not registed for this path: " << uri << std::endl;
		return;
	}
	auto resp = it_methode->second;
	auto response = resp(httpRequest);

	size_t data_sent = 0;
	while (data_sent < response.httpMessageLength)
	{
		size_t chunk_size = response.httpMessageLength - data_sent;
		// I only want to sent max 1000bytes over the network
		if (chunk_size > CHUNK_SIZE)
		{
			chunk_size = CHUNK_SIZE;
		}

		auto bytes_sent = send(sockInfo->sockfd, response.httpMessage.data() + data_sent, chunk_size, 0);

		data_sent += bytes_sent;
	}
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::prepare_kqueue_events()
{
	for (int i = 0; i < NUM_WORKERS; ++i)
	{
		if ((working_kqueue_fd[i] = kqueue()) < 0)
		{
			std::cout << "Could not create worker fd for kqueue" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::process_worker_events(int worker_idx)
{
	int64_t new_events;  //, socket_connection_fd, client_len;

	// File descriptor for kqueue
	auto worker_kfd = working_kqueue_fd[worker_idx];

	while (true)
	{
//		if(flag)
//		{
//			std::cout << "GOT SIGINT" << std::endl;
//			break;
//		}
		new_events =
				kevent(worker_kfd, NULL, 0, working_events[worker_idx], NUM_EVENTS, NULL);

		if (new_events == -1)
		{
			std::cout << "Kevent error 2" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::cout << "WorkerFD: " << worker_kfd << " TRIGGERED EVENTS: " << new_events << std::endl;
		for (int i = 0; i < new_events; i++)
		{
			int event_fd = working_events[worker_idx][i].ident;

			SimpleHttpServer_t::sockInfos_t* sockInfo =
					reinterpret_cast<SimpleHttpServer_t::sockInfos_t*>(
							working_events[worker_idx][i].udata);

			// When the client disconnects an EOF is sent. By
			// closing the file descriptor the event is
			// automatically removed from the kqueue
			if (working_events[worker_idx][i].flags & EV_EOF)
			{
				std::cout << "Client has disconnected" << std::endl;
				while (close(sockInfo->sockfd) == -1)
				{
					std::cout << "ERRNO: " << errno << std::endl;
				}
				delete sockInfo;
//				std::cout << "COUNTER: " << --counter << std::endl;
			}

				// If the new event's file descriptor is the same as the
				// listening socket's file descriptor, we are sure that
				// a new client wants to connect to our socket.
			else if (event_fd == listeningSocket.sockfd)
			{
				std::cout << "THIS CAN NOT HAPPEN" << std::endl;
				continue;

			}
			else if (working_events[worker_idx][i].filter & EVFILT_READ)
			{
				// Read bytes from socket
				HttpRequest_t httpRequest = handle_read(sockInfo);
				handle_write(sockInfo, std::move(httpRequest));
			}

//			std::cout << "COUNTER: " << counter << std::endl;
		}
	}
}

//----------------------------------------------------------------------------
bool SimpleHttpServer_t::startServer(std::string ipAddr, int64_t port)
{
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;	// I dont care ipv4 or 6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	int rv;
	if ((rv = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	int yes = 1;
	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1)
		{
			std::cout << "server: socket" << std::endl;
			continue;
		}

		// So I don't get the annoying failed to bind errors
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1)
		{
			std::cout << "setsockopt" << std::endl;
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			std::cout << "server: bind" << std::endl;
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACK_LOG) == -1)
	{
		std::cout << "listen" << std::endl;
		exit(1);
	}

	// Here we need to setup the kqueue for each worker thread
	prepare_kqueue_events();

	// start listnener thread here for incoming connections
	listenerThread = std::thread(&SimpleHttpServer_t::listen_and_accept, this);
	// We also need to call join afterwards probably in a stopServer methode

	std::cout << "Server waiting for connections...\n";

	for (int i = 0; i < NUM_WORKERS; ++i)
	{
		workerThread[i] =
				std::thread(&SimpleHttpServer_t::process_worker_events, this, i);
	}

	listenerThread.join();
	for (int i = 0; i < NUM_WORKERS; ++i)
	{
		workerThread[i].join();
	}

	std::cout << "WE ARE FINISHED HERE AND ARE CLOSING THE PROGRAM" << std::endl;

	return true;
}

//----------------------------------------------------------------------------
HttpResponse_t SimpleHttpServer_t::pageNotFound(HttpRequest_t* httpReq)
{
	HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::NotFound };

	// Setting some headers
	httpResponse.setHeader("Server", "Sandi");
	httpResponse.setHeader("Content-Type", "text/html");

	// Preparing answer from server
	std::string resp_msg = "<h1>404 Page Not found</h1>";
	resp_msg += "\nRequested: " + httpReq->httpUri + "\n";

	// Building Body
	httpResponse.buildResponseBody(resp_msg);

	return httpResponse;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::serve_static_file(const fs::path& root_dir,
		const std::string& path,
		std::ostringstream& stream, size_t& fileSize, HttpResponse_t& response)
{
	// Check if the path contains ".." to prevent access to parent directories
	if (path.find("..") != std::string::npos)
	{
		// Return a "Forbidden" response if the path is not safe
		response.statusCode = HttpResponse_t::HttpStatusCode::Forbidden;
		response.setHeader("Content-Length", "0");
		response.setHeader("Content-Type", "text/html");
		return;
	}

	// Append the requested path to the root directory
	auto file_path = root_dir / path;

	// Check if the file exists
	if (!fs::exists(file_path))
	{
		// Return a "Not Found" response if the file does not exist
		std::cout << "WE FOUND NOTHING for: " << file_path << std::endl;
		response.statusCode = HttpResponse_t::HttpStatusCode::NotFound;
		response.setHeader("Content-Length", "0");
		response.setHeader("Content-Type", "text/html");

		stream << "<h1>404 Page Not found</h1>";
		return;
	}

	// Open the file in read-only mode
	std::ifstream file{ file_path, std::ios::binary };

	// Get the file size
	auto size = fs::file_size(file_path);
	fileSize = size;

	// Set the Content-Type header based on the file's extension
	std::string content_type = "text/plain";
	if (file_path.extension() == ".html")
	{
		content_type = "text/html";
	}
	else if (file_path.extension() == ".css")
	{
		content_type = "text/css";
	}
	else if (file_path.extension() == ".js")
	{
		content_type = "application/javascript";
	}
	else if (file_path.extension() == ".wasm")
	{
		content_type = "application/wasm";
	}

	// Send the response headers
	response.setHeader("Content-Type", content_type);
	response.setHeader("Content-Length", std::to_string(size));

	// Send the file contents
	stream << file.rdbuf();
}

//----------------------------------------------------------------------------
std::vector<std::string> SimpleHttpServer_t::split_path(const std::string& path,
		char delimiter)
{
	std::string token;
	std::vector<std::string> tokens;
	std::istringstream token_stream(path);

	while (std::getline(token_stream, token, delimiter))
	{
		if (token.empty())
			continue;
		tokens.push_back(token);
	}

	return tokens;
}

//----------------------------------------------------------------------------
HttpRequestHandler_t SimpleHttpServer_t::fileServer(std::string rootDir)
{
	// the root_dir path depends on where the binary is executed
	const auto root_dir = fs::path{ rootDir };
	root = root_dir;

	auto serveStatic =
			[this, root_dir](const HttpRequest_t& request) -> HttpResponse_t
			{
				HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };

				std::ostringstream stream;
				size_t fileSize;
				this->serve_static_file(root_dir, request.resource, stream, fileSize, httpResponse);

				auto body = stream.str();

				httpResponse.httpMessageLength = body.size();
				httpResponse.httpResponseBody = body;
				httpResponse.httpMessage = httpResponse.httpResponseBody;

				httpResponse.setHeader("Server", "Sandi");
				httpResponse.buildResponseBody(body);

				return httpResponse;
			};

	return serveStatic;
}

//----------------------------------------------------------------------------
HttpRequestHandler_t
SimpleHttpServer_t::stripPrefix(const std::string prefix, HttpRequestHandler_t handler)
{
	fileServerPath = prefix;
	fileServerPath = fileServerPath.substr(1, prefix.length()).substr(0, prefix.length() - 2);

	auto serveStatic =
			[this, prefix, handler](HttpRequest_t& request) -> HttpResponse_t
			{
				request.httpUri = request.httpUri.substr(prefix.length());
				return handler(request);
			};

	return serveStatic;
}