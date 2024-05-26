#include "pch.h"
#include "SimpleAsynchronousServer.h"
#include "SimpleServerPortNumber.h"

#include <boost/bind.hpp>
#include <iostream>

void con_handler::handle_read(std::size_t bytes_transferred)
{
    bytes_transferred /= sizeof(wchar_t);
    data[bytes_transferred] = 0;
    for (int i = 0; i < bytes_transferred; i++)
        if (data[i] > 0x7F)
            data[i] = L'?';
    BOOST_LOG(m_server->Logger) << data;
}

void con_handler::handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred)
  {
    if (ec)
    {
        if (ec == boost::asio::error::eof)
            m_server->RemoveConnection(this);
        else
            std::cerr << "err (receive): " << ec.message() << std::endl;
    }
    else
    {
      handle_read(bytes_transferred);
      start_receive();
    }
  }

void con_handler::start_receive()
{
    m_sock->async_receive(
        boost::asio::buffer(data),
        boost::bind(&con_handler::handle_receive, this,
        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void con_handler::handle_accept(const boost::system::error_code& ec)
  {
    if (ec)
    {
        if (ec == boost::asio::error::operation_aborted)
            std::cerr << "Server has been stopped." << std::endl;
        else
            std::cerr << "err (accept): " << ec.message() << std::endl;
    }
    else
    {
      mThread = new boost::thread(boost::bind(&con_handler::start_receive, this));
#pragma warning( push )
#pragma warning( disable : 4189 )
    con_handler* connection = new con_handler(m_server);
#pragma warning( pop )
    }
  }

void con_handler::start_accept()
{
    m_sock = new tcp::socket(m_server->m_io_service);
    m_server->InsertConnection(this);
    m_server->m_acceptor.async_accept(*m_sock,
        boost::bind(&con_handler::handle_accept, this,
          boost::asio::placeholders::error));
}

con_handler::con_handler(AsynchronousServer* aServer)
    : m_server(aServer), mThread(nullptr)
{
    start_accept();
}

con_handler::~con_handler()
{
    if (mThread)
        delete mThread;
    m_sock->close();
    delete m_sock;
}

AsynchronousServer::AsynchronousServer(boost::asio::io_service& io_service, src::wlogger& aLogger, ip::port_type aPort)
    : m_acceptor(io_service, tcp::endpoint( boost::asio::ip::address::from_string(SIMPLE_SERVER_LISTEN_ADDRESS), (aPort == 0) ? SIMPLE_SERVER_PORT_NUMBER : aPort)), m_io_service(io_service), Logger(aLogger)
  {
    m_acceptor.set_option(tcp::acceptor::reuse_address(true));
#pragma warning( push )
#pragma warning( disable : 4189 )
    con_handler* connection = new con_handler(this);
#pragma warning( pop )
  }

AsynchronousServer::~AsynchronousServer()
{
    for (auto i : m_connections)
        delete i;
    m_connections.clear();
}

void AsynchronousServer::InsertConnection(const con_handler* aConnection)
{
    m_connections.insert(aConnection);
}

void AsynchronousServer::RemoveConnection(const con_handler* aConnection)
{
    m_connections.erase(aConnection);
    delete aConnection;
}

void AsynchronousServer::Stop()
{
    post(m_acceptor.get_executor(), [this] { m_acceptor.cancel(); });
}
