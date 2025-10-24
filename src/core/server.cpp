#include "./server.h"
#include <netinet/in.h>
#include <cstring>
#include <system_error>
#include "./soup_bin.h"

namespace lshl::soup_bin::server::test {}

namespace lshl::soup_bin::server {
Server::Server(const uint16_t port) noexcept(false) : socket_(socket(PF_INET, SOCK_STREAM, 0)) {
  if (this->socket_ < 0) {
    throw std::system_error(errno, std::generic_category(), "socket call failed");
  }

  int enable = 1;
  if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    const auto error = errno;
    close(this->socket_);
    throw std::system_error(error, std::generic_category(), "setsockopt call failed");
  }

  sockaddr_in address{};
  std::memset(&address, 0, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(INADDR_ANY);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (bind(this->socket_, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address)) < 0) {
    const auto error = errno;
    close(this->socket_);
    throw std::system_error(error, std::generic_category(), "bind call failed");
  }
};

}  // namespace lshl::soup_bin::server