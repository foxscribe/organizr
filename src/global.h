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

#include "database/DatabaseWrapper.h"

namespace o::global {
constexpr const char* TOKEN_VARNAME   = "DISCORD_TOKEN";
constexpr const char* URL_VARNAME     = "DATABASE_URL";
constexpr const char* GUILDID_VARNAME = "GUILD_ID";

inline o::db::DatabaseWrapper wrapper(std::getenv(URL_VARNAME));
inline dpp::cluster bot(std::getenv(TOKEN_VARNAME));
} // namespace o::global