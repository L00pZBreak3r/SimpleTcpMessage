#pragma once

#include <unordered_set>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "SimpleLogFile.h"

using namespace boost::asio;
using ip::tcp;

class AsynchronousServer;

class con_handler
{
private:
  enum { max_length = 1024 };
  wchar_t data[max_length];
  AsynchronousServer* m_server;
  tcp::socket* m_sock;
  boost::thread* mThread;

  void handle_accept(const boost::system::error_code& ec);

  void start_receive();
  void handle_read(std::size_t bytes_transferred);

public:
   con_handler(AsynchronousServer* aServer);
   ~con_handler();

    void start_accept();
    void handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred);
};

class AsynchronousServer {

private:
  tcp::acceptor m_acceptor;
  boost::asio::io_service& m_io_service;
  std::unordered_set< const con_handler* > m_connections;

  void InsertConnection(const con_handler* aConnection);
  void RemoveConnection(const con_handler* aConnection);
  
  friend void con_handler::start_accept();
  friend void con_handler::handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred);

public:
    src::wlogger& Logger;

    AsynchronousServer(boost::asio::io_service& io_service, src::wlogger& aLogger, ip::port_type aPort);
    ~AsynchronousServer();

    void Stop();
};
