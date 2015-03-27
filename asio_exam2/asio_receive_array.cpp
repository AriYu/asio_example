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

  tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), 18080));
  tcp::socket socket(io_service);

  // 接続待機
  acc.accept(socket);

  // メッセージ受信
  asio::streambuf receive_buffer;
  boost::system::error_code error;
  asio::read(socket, receive_buffer, asio::transfer_all(), error);

  if (error && error != asio::error::eof) {
	std::cout << "receive failed: " << error.message() << std::endl;
  }
  else {
	const std::vector<float> foo = boost::asio::buffer_cast<const float*>(receive_buffer.data());
	const std::vector<float> foo = (receive_buffer.data());
	//	const char* data = asio::buffer_cast<const char*>(receive_buffer.data());
	for(unsigned int  i = 0; i < foo.size(); i++){
	  std::cout << foo[i] << std::endl;
	}
  }
}
