#include "../core/client.h"
#include "../core/soup_bin.h"
#include "../util/boost_log_init.h"
#include "../util/boost_log_util.h"

namespace lshl::soup_bin::client::example {}

auto main() -> int {
  lshl::soup_bin::util::init_logging();
  LOG_INFO << "Client example...";

  return 0;
}