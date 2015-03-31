#ifndef RECEIVE_H_
#define RECEIVE_H_

#include "message.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class my_receive
{
public:
  my_receive(boost::asio::io_service& io_service)
	: socket_(io_service)
  {
  }
  boost::asio::ip::tcp::socket& socket()
  {
    return socket_;
  }
  size_t read(message& messenger){
 	try
	  {
		// ヘッダーを受信
		size_t header_bytes = boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
		std::cout << "header_bytes: " << header_bytes << std::endl;
		// ヘッダーをデシリアライズ
		std::istringstream is(std::string(inbound_header_, header_length));
		size_t inbound_data_size = 0;
		if(!(is >> std::hex >> inbound_data_size))
		  {
			// 失敗？
			return -1;
		  }

		// 本体を受信
		inbound_data_.resize(inbound_data_size);
		size_t data_bytes = boost::asio::read(socket_, 
											  boost::asio::buffer(inbound_data_));
		std::cout << "data_bytes : " << data_bytes << std::endl;
		// 本体をデシリアライズ
		std::string archive_data(&inbound_data_[0], inbound_data_.size());
		std::istringstream archive_stream(archive_data);
		boost::archive::text_iarchive archive(archive_stream);
		archive >> messenger;

		return data_bytes;
	  }
	catch(std::exception& e)
	  {
		std::cerr << "de-serialized error : " << e.what() << std::endl;
		return 0;
	  }
  }

  size_t read(beta_message& messenger){
 	try
	  {
		// ヘッダーを受信
		size_t header_bytes = boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
		std::cout << "header_bytes: " << header_bytes << std::endl;
		// ヘッダーをデシリアライズ
		std::istringstream is(std::string(inbound_header_, header_length));
		size_t inbound_data_size = 0;
		if(!(is >> std::hex >> inbound_data_size))
		  {
			// 失敗？
			return -1;
		  }

		// 本体を受信
		inbound_data_.resize(inbound_data_size);
		size_t data_bytes = boost::asio::read(socket_, 
											  boost::asio::buffer(inbound_data_),
											  boost::asio::transfer_exactly(inbound_data_size));
		std::cout << "data_bytes : " << data_bytes << std::endl;
		// 本体をデシリアライズ
		std::string archive_data(&inbound_data_[0], inbound_data_.size());
		std::istringstream archive_stream(archive_data);
		boost::archive::text_iarchive archive(archive_stream);
	

		archive >> messenger;

 
		return data_bytes;
	  }
	catch(std::exception& e)
	  {
		std::cerr << "de-serialized error : " << e.what() << std::endl;
		return 0;
	  }
  }
  private:
	boost::asio::ip::tcp::socket socket_;

	enum { header_length = 8 };

	char inbound_header_[header_length];
  
	std::vector<char> inbound_data_;

  };

typedef boost::shared_ptr<my_receive> receive_ptr;

#endif
