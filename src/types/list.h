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

#pragma once

#include <string>
#include <vector>

namespace o {
struct list {
    std::string val;
    std::vector<std::string> tasks;

    std::string message() const {
        std::string msg = "```\n";
        msg += val + "\n";
        for (long i = 0; i < tasks.size(); i++) {
            msg += std::format("{}. {}\n", i + 1, tasks[i]);
        }
        msg += "```";
        return msg;
    }
};
} // namespace o