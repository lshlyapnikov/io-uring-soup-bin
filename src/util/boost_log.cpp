// Copyright 2024 Leonid Shlyapnikov.
// SPDX-License-Identifier: Apache-2.0

#include "./boost_log.h"
#include <cstdlib>
#include <string>
// NOLINTBEGIN(misc-include-cleaner)
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
// NOLINTEND(misc-include-cleaner)

namespace lshl::soup::util {

auto get_log_level_from_env() -> boost::log::trivial::severity_level {
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  const char* env_p = std::getenv("BOOST_LOG_LEVEL");
  if (env_p != nullptr) {
    const std::string level(env_p);
    if (level == "trace") {
      return boost::log::trivial::trace;
    } else if (level == "debug") {
      return boost::log::trivial::debug;
    } else if (level == "info") {
      return boost::log::trivial::info;
    } else if (level == "warning") {
      return boost::log::trivial::warning;
    } else if (level == "error") {
      return boost::log::trivial::error;
    } else if (level == "fatal") {
      return boost::log::trivial::fatal;
    }
  }
  return boost::log::trivial::info;  // Default to info if not set
}

auto init_logging() noexcept -> void {
  // NOLINTNEXTLINE(misc-include-cleaner)
  boost::log::core::get()->set_filter(boost::log::trivial::severity >= get_log_level_from_env());
}

}  // namespace lshl::soup::util
