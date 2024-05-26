#include "pch.h"
#include "SimpleAsynchronousServer.h"
#include "SimpleServer.h"

#include <signal.h>
#include <iostream>

static AsynchronousServer* aServer;
static void shutdown(int)
{
	aServer->Stop();
}

void StartSimpleServer(unsigned short aPort)
{
  signal(SIGINT, shutdown);
  boost::shared_ptr< file_sink > sink = init_logging();
  src::wlogger lg;
  try 
  {
    boost::asio::io_service io_service;
    AsynchronousServer server(io_service, lg, aPort);
    aServer = &server;
    io_service.run();
  } 
  catch(std::exception& e) 
  {
    std::cerr << e.what() << std::endl;
  }

    stop_logging(sink);
    std::cout << "Logging stopped." << std::endl;
}
