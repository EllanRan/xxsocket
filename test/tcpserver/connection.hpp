//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2013-2016 halx99
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <array>
#include <memory>
#include <boost/asio.hpp>
// #include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
// #include "request_parser.hpp"

namespace tcp {
namespace server {

class connection_manager;

/// Represents a single connection from a client.
class connection
  : public std::enable_shared_from_this<connection>
{
public:
  connection(const connection&)/* = delete*/;
  connection& operator=(const connection&)/* = delete*/;

  ~connection(void);

  /// Construct a connection with the given socket.
  explicit connection(boost::asio::ip::tcp::socket socket,
      connection_manager& manager/*, request_handler& handler*/);

  /// Start the first asynchronous operation for the connection.
  void start();

  /// Stop all asynchronous operations associated with the connection.
  void stop();

  /// get socket
  boost::asio::ip::tcp::socket& get_socket();

  const char* get_address(void) const;
  unsigned short get_port(void) const;

  inline time_t expire_time(void) const 
  {
      return expire_msec_;
  }
  inline void expire_time(time_t expire_msec)
  {
      expire_msec_ = expire_msec;
  }

  void close();

private:
  /// Perform an asynchronous read operation.
  void do_read_head(size_t offset = 0);

  /// Do remain http content
  void do_read_content(void);

  /// Perform an asynchronous write operation.
  void do_write();

  /// Strand to ensure the connection's handlers are not called concurrently.
  /// Use io_service_pool do not need it.
  /// boost::asio::io_service::strand strand_;

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The manager for this connection.
  connection_manager& connection_manager_;

  /// The handler used to process the incoming request.
  request_handler request_handler_;

  /// Buffer for incoming data.
  std::array<char, 1024> buffer_;
  // char buffer_[8192];

  /// The incoming request.
  request request_;

  /// The reply to be sent back to the client.
  // reply reply_;

  /// total bytes transferred
  size_t  total_bytes_received_; // http package total

  char    v4_address_[32];

  unsigned short port_;

  time_t  expire_msec_; // message expire timestamp_ milliseconds
};

typedef std::shared_ptr<connection> connection_ptr;

} // namespace server
} // namespace http

#endif // HTTP_CONNECTION_HPP

//namespace boost {
//    template<> inline 
//        void checked_delete<http::server::connection>(http::server::connection* x)
//    {
//        x->~connection();
//        ngx::get_thread_handle()->release(x);
//        // x->GetFactory()->DestroyObject(x);
//    }
//};

//static const size_t connection_size = sizeof(http::server::connection);

