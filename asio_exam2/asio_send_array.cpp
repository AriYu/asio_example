#include <boost/asio.hpp>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace boost::asio;

int main( int argc, char** argv )
{

  if ( argc < 3 ) {
	std::cerr << "usage: " << argv[0] << "host name " << " " << " host port" << std::endl;
	exit( EXIT_FAILURE );
  }

  boost::array<float, 5> foo = {1.0, 2.0, 3.0, 4.0, 5.0};
  BOOST_FOREACH( const float i, foo ) {
	std::cout << i << std::endl;
  }

  //    ip::tcp::socket socket( ios );
  boost::asio::io_service io_service;
   
  boost::asio::ip::tcp::socket socket(io_service);

  try
	{
	  boost::system::error_code error;
	  boost::asio::ip::tcp::resolver resolver(io_service);
	 
	  boost::asio::ip::tcp::endpoint 
		endpoint(*resolver.resolve({ argv[1], argv[2] }));

	  std::cout << "connecting to " << endpoint << "..." << std::endl;

	  socket.connect(endpoint, error);
	  
	  if(error){
		std::cout << "connect failed : " << error.message() << std::endl;
	  }else{
		std::cout << "connected" << std::endl;
	  }

	  const size_t bytes = boost::asio::write(socket, boost::asio::buffer( foo ));

	  std::cout << "sent " << bytes << " bytes" << std::endl;
	  return 0;
	}
  catch(std::exception&e)
	{
	  std::cerr << "Exception: " << e.what() << std::endl;
	  return 1;
	}
}
