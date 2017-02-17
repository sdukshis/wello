#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <csignal>

#include <hello/hello.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Object.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::JSON::Object;

using hello::greet;

namespace
{
  volatile std::sig_atomic_t running;
}

void signal_handler(int signal)
{
  running = false;
}

class HelloRequestHandler: public HTTPRequestHandler {
public:
	void handleRequest(HTTPServerRequest& request,
					   HTTPServerResponse& response) override {
		response.setContentType("application/json");

		Object body;
		std::stringstream ss;
		greet(ss);

		body.set("msg", ss.str());

		auto&& ostr = response.send();
		body.stringify(ostr);
	}
};

class HelloRequestHandlerFactory: public HTTPRequestHandlerFactory {
public:
	HelloRequestHandler* createRequestHandler(const HTTPServerRequest&) override {
		return new HelloRequestHandler;
	}
};

int main()
try {
	HTTPServer server{new HelloRequestHandlerFactory,
					  9090,
					  new HTTPServerParams};

	std::clog << "Starting web server..." << std::endl;
	server.start();

	if (SIG_ERR == std::signal(SIGTERM, signal_handler)) {
		throw std::runtime_error{"register signal failed"};
	}

	std::clog << "Web server started. Press enter to exit" << std::endl;
	for (running = true; running;) {
		std::this_thread::sleep_for(std::chrono::seconds{1});
	}
	std::clog << "Stopping web server..." << std::endl;

	return 0;
} catch(const Poco::Exception& e) {
	std::cerr << e.displayText() << std::endl;
	return EXIT_FAILURE;
}
