#include <boost/asio.hpp>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <vector>

#include "message.h"
#include "receive.hpp"

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

  
 
  asio::io_service io_service;


  tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));
  tcp::socket socket(io_service);

  receive_ptr new_receiver(new my_receive(acc.get_io_service()));
  acc.accept(new_receiver->socket());


  // 接続待機 接続があるまでブロック
  //acc.accept(socket);
 
  std::cout << "connected ... " << std::endl;


  while(true){
	
	// メッセージ受信
	try
	  {
		boost::system::error_code error;
		//message messenger;
		beta_message messenger;
		size_t received_bytes = new_receiver->read(messenger);
		if (error && error != asio::error::eof) 
		  {
			std::cout << "receive failed: " << error.message() << std::endl;
		  }
		else
		  {
			if(received_bytes != 0 && received_bytes > 0)
			  {
				std::cout << "received " << received_bytes << " bytes " << std::endl; 
				for(size_t i = 0; i < 6; i++)
				  {
					if(messenger.bodies_[i].isTracked_ == true)
					  {
						std::cout << "body[" << i << "]::right hand state: " 
								  << messenger.bodies_[i].right_hand_state_ << std::endl;
						std::cout << "body[" << i << "]::left hand state : " 
								  << messenger.bodies_[i].left_hand_state_ << std::endl;
						std::cout << std::endl;	
					  }
				  }

			  }
		  }
	  }catch(std::exception& e)
	  {
		std::cout << "exception is calld : " << e.what() << std::endl;
	  }
	
  }
}
