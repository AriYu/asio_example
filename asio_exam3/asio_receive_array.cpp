#include <boost/asio.hpp>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <vector>

#include "message.h"


#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


namespace asio = boost::asio;
using asio::ip::tcp;

int main( int argc, char** argv )
{

  if ( argc < 2 ) {
	std::cerr << "usage: " << argv[0] << " host port" << std::endl;
	exit( EXIT_FAILURE );
  }

  message messenger;
  size_t archive_data_size = 0;
  {
	std::ostringstream archive_stream;
	boost::archive::text_oarchive archive(archive_stream);
	archive << messenger;
	std::string outbound_data = archive_stream.str();
	archive_data_size = sizeof(outbound_data);
	std::cout << "archive_data_size : " << archive_data_size << std::endl;
  }

  std::cout << "size of messenger : " << sizeof(messenger) << std::endl;
  std::cout << "size of bodies    : " << messenger.bodies_.size() << std::endl;
  std::cout << "size of positions : " << messenger.bodies_[0].positions_[24].x_ << std::endl;

  asio::io_service io_service;

  tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));
  tcp::socket socket(io_service);



  // 接続待機 接続があるまでブロック
  acc.accept(socket);
 
  std::cout << "connected ... " << std::endl;


  while(true){
	
	// メッセージ受信
	boost::system::error_code error;
	boost::asio::streambuf receive_buffer;
	// const size_t bytes = boost::asio::read(socket, 
	// 									   boost::asio::buffer(inbound_data), 
	// 									   error);
	const size_t bytes = boost::asio::read(socket, 
										   receive_buffer,
										   boost::asio::transfer_exactly(13779),
										   error);
	try
	  {
		std::string archive_data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
		std::istringstream archive_stream(archive_data);
		boost::archive::text_iarchive archive(archive_stream);
		archive >> messenger;
	  }
	catch(std::exception& e)
	  {
		std::cout << "exception : " << e.what() << std::endl;
	  }

	if (error && error != asio::error::eof) 
	  {
		std::cout << "receive failed: " << error.message() << std::endl;
	  }
	else
	  {
		if(bytes != 0)
		  {
			std::cout << "received " << bytes << " bytes " 
					  << " hand state : " << messenger.bodies_[0].isTracked_ << std::endl;
		  }
	  }
  }
}
