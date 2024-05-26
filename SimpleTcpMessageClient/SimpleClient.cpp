#include "pch.h"
#include "SimpleClient.h"
#include "..\SimpleTcpMessageServer\SimpleServerPortNumber.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using ip::tcp;
using std::wstring;
using std::wcout;
using std::endl;


static bool keepGoing = true;
static void shutdown(int)
{
	keepGoing = false;
}

static std::wstring FormatTime(boost::posix_time::ptime aTime)
{
  static std::locale loc(std::wcout.getloc(),
                         new wtime_facet(L"%Y-%m-%d %H:%M:%s"));

  std::wstringstream wss;
  wss.imbue(loc);
  wss << aTime;
  return wss.str();
}

void StartSimpleClient(unsigned short aPort, const wchar_t* aString, unsigned short aIntervalSeconds)
{
  signal(SIGINT, shutdown);
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);

  socket.connect( tcp::endpoint( boost::asio::ip::address::from_string(SIMPLE_SERVER_LISTEN_ADDRESS), (aPort == 0) ? SIMPLE_SERVER_PORT_NUMBER : aPort ) );

  while (keepGoing)
  {
      wstring msg = L"[";
      msg += FormatTime(microsec_clock::universal_time()) + L"] " + aString;
      boost::system::error_code error;
      boost::asio::write( socket, boost::asio::buffer(msg), error );

      if (error)
      {
        std::cout << "send failed: " << error.message() << endl;
        break;
      }
      Sleep(aIntervalSeconds * 1000);
  }
}
