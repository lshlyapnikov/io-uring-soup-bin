#include "./server.h"
#include <netinet/in.h>
#include <cstring>
#include <stdexcept>
#include "../util/boost_log_util.h"
#include "./soup_bin.h"

namespace lshl::soup_bin::server {

// NOLINTBEGIN(concurrency-mt-unsafe)
auto open_listening_socket(const std::uint16_t port) noexcept -> int {
  const int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    LOG_ERROR << "socket call failed: " << std::strerror(errno);
    return -1;
  }

  int enable = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    const auto error = errno;
    close(socket_fd);
    LOG_ERROR << "setsockopt call failed: " << std::strerror(error);
    return -1;
  }

  sockaddr_in address{};
  std::memset(&address, 0, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(INADDR_ANY);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (bind(socket_fd, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address)) < 0) {
    const auto error = errno;
    close(socket_fd);
    LOG_ERROR << "bind call failed: " << std::strerror(error);
    return -1;
  }

  if (listen(socket_fd, SOMAXCONN) < 0) {
    const auto error = errno;
    close(socket_fd);
    LOG_ERROR << "listen call failed: " << std::strerror(error);
    return -1;
  }

  return socket_fd;
}
// NOLINTEND(concurrency-mt-unsafe)

Server::Server(const std::uint16_t port) noexcept(false) : socket_{open_listening_socket(port)} {
  if (this->socket_ < 0) {
    throw std::runtime_error("Cannot create Server");
  }
};

Server::~Server() noexcept {
  if (this->socket_ >= 0) {
    if (close(this->socket_) < 0) {
      // NOLINTNEXTLINE(concurrency-mt-unsafe)
      LOG_ERROR << "Failed to close socket: " << std::strerror(errno);
    }
  };
}

}  // namespace lshl::soup_bin::server