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

#include <functional>

#include <dpp/dpp.h>

namespace o::bot {
class HandlerManager {
  private:
    std::unordered_map<std::string,
                       std::function<void(const dpp::slashcommand_t&)>>
            handlers;

  public:
    constexpr void operator()(
            std::string name,
            const std::function<void(const dpp::slashcommand_t&)>& handler) {
        handlers[name] = handler;
    }

    constexpr std::function<void(const dpp::slashcommand_t&)>& operator[](
            std::string name) {
        return handlers[name];
    }
};
}; // namespace o::bot