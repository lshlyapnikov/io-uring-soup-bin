#include "../core/server.h"
#include "../core/soup_bin.h"
#include "../util/boost_log_init.h"
#include "../util/boost_log_util.h"

namespace lshl::soup_bin::server::example {}

auto main() -> int {
  lshl::soup_bin::util::init_logging();
  LOG_INFO << "Server example...";
  return 0;
}