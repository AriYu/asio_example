#include <boost/asio.hpp>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <vector>


namespace asio = boost::asio;
using asio::ip::tcp;

int main( int argc, char** argv )
{

  if ( argc < 2 ) {
	std::cerr << "usage: " << argv[0] << " host port" << std::endl;
	exit( EXIT_FAILURE );
  }

  asio::io_service io_service;

  tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));
  tcp::socket socket(io_service);

  // 接続待機 接続があるまでブロック
  acc.accept(socket);
 
  while(true){

	// メッセージ受信
	boost::system::error_code error;
	boost::array<float, 5> foo;
	const size_t bytes = boost::asio::read(socket, boost::asio::buffer(&foo, sizeof(foo)), error);

	if (error && error != asio::error::eof) {
	  std::cout << "receive failed: " << error.message() << std::endl;
	}
	else {
	  if(bytes != 0){
		std::cout << "received " << bytes << " bytes" << std::endl;
		for(unsigned int  i = 0; i < foo.size(); i++){
		  std::cout << foo[i] << std::endl;
		}
	  }
	}
  }
}
