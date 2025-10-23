// Copyright 2024 Leonid Shlyapnikov.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <boost/log/trivial.hpp>

namespace lshl::soup_bin::util {

auto get_log_level_from_env() -> boost::log::trivial::severity_level;

auto init_logging() noexcept -> void;

}  // namespace lshl::soup_bin::util
