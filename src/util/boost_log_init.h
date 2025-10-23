// Copyright 2024 Leonid Shlyapnikov.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <boost/log/trivial.hpp>

namespace lshl::soup_bin::util {

/// @brief Get log level from the BOOST_LOG_LEVEL environment variable. Call it from main function only.
/// @return Log level as boost::log::trivial::severity_level
auto get_log_level_from_env() noexcept -> boost::log::trivial::severity_level;

/// @brief Initialize logging with level from environment variable BOOST_LOG_LEVEL. Call it from main function only.
auto init_logging() noexcept -> void;

}  // namespace lshl::soup_bin::util
