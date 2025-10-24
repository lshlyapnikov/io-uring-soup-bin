#pragma once

#include <liburing.h>
#include <sys/socket.h>
#include <cstdint>

namespace lshl::soup_bin::server {

/// @warning This function is NOT thread-safe. Don't call it from multiple threads simultaneously.
auto open_listening_socket(const uint16_t port) noexcept -> int;

class Server {
 public:
  /// Opens the listening socket.
  /// @warning This constructor is NOT thread-safe. Don't call it from multiple threads simultaneously.
  explicit Server(const std::uint16_t port) noexcept(false);

  /// Closes the listening socket.
  virtual ~Server() noexcept;

  // Disable copy and move semantics
  Server(const Server&) = delete;
  Server(Server&&) = delete;
  auto operator=(const Server&) -> Server& = delete;
  auto operator=(Server&&) -> Server& = delete;

 private:
  int socket_;
};

}  // namespace lshl::soup_bin::server
