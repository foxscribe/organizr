/**
 * Copyright (C) 2025 Iaroslav "foxscribe" Lisov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "logging.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>

namespace o::logging {
void cout_logger(const dpp::log_t& event) {
    if (event.severity == 0) {
        return;
    }
    auto now          = std::chrono::system_clock::now();
    auto time_t_now   = std::chrono::system_clock::to_time_t(now);
    auto local_tm     = *std::localtime(&time_t_now);
    const char* color = "";
    const char* reset = "\033[0m";
    switch (event.severity) {
        case dpp::ll_critical:
            color = "\033[1;31m";
            break; // red
        case dpp::ll_error:
            color = "\033[31m";
            break; // red
        case dpp::ll_warning:
            color = "\033[33m";
            break; // yellow
        case dpp::ll_info:
            color = "\033[36m";
            break; // cyan
        case dpp::ll_debug:
            color = "\033[32m";
            break; // green
        default:
            color = "";
            break;
    }
    std::cout << "[" << std::put_time(&local_tm, "%F %T") << "] " << color
              << "[" << dpp::utility::loglevel(event.severity) << "] "
              << event.message << reset << std::endl;
}
} // namespace o::logging