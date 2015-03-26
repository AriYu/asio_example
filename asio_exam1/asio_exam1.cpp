#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> " << std::endl;
      return 1;
    }

	boost::asio::io_service io;

	while(true)
	  {
		ip::tcp::iostream buf;
		ip::tcp::acceptor acceptor(io, ip::tcp::endpoint(ip::tcp::v4(), std::atoi(argv[1])));
		acceptor.accept(*buf.rdbuf());

		std::string tmp;
		
		while(true){
		  buf >> tmp;
		  if(tmp == "quit"){
			buf << ">>bye"<< std::endl;
			break;
		  }
		  buf << ">>" << tmp << std::endl;
		}
	  }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
