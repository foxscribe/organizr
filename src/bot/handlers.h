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

#include <dpp/dpp.h>

namespace o::bot::handlers {
void authors(const dpp::slashcommand_t& event);
void license(const dpp::slashcommand_t& event);
void repository(const dpp::slashcommand_t& event);
void lists(const dpp::slashcommand_t& event);
void newlist(const dpp::slashcommand_t& event);
} // namespace o::bot::handlers
