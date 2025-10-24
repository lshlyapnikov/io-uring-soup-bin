#pragma once

#include <liburing.h>
#include <sys/socket.h>
#include <array>
#include <cstdint>
#include <variant>

namespace lshl::soup_bin::server {

class Server {
 public:
  explicit Server(const uint16_t port) noexcept(false);
  virtual ~Server() = default;
  Server(const Server&) = delete;
  Server(Server&&) = delete;
  auto operator=(const Server&) -> Server& = delete;
  auto operator=(Server&&) -> Server& = delete;

 private:
  int socket_;
};

}  // namespace lshl::soup_bin::server
