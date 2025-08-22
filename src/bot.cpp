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

#include <cstdlib>

#include <dpp/dpp.h>
#include <pqxx/pqxx>

#include "bot/HandlerManager.h"
#include "bot/handlers.h"
#include "global.h"
#include "logging/logging.h"

int main() {
    constexpr auto& bot = o::global::bot;
    o::bot::HandlerManager handlers;
    handlers("authors", o::bot::handlers::authors);
    handlers("license", o::bot::handlers::license);
    handlers("repository", o::bot::handlers::repository);
    handlers("lists", o::bot::handlers::lists);
    handlers("newlist", o::bot::handlers::newlist);
    bot.on_log(o::logging::cout_logger);
    bot.on_slashcommand([&handlers](const dpp::slashcommand_t& event) {
        const std::string command = event.command.get_command_name();
        const auto& handler       = handlers[command];
        if (handler) {
            handler(event);
        }
    });
    bot.on_ready([&bot](const dpp::ready_t& event) {
        bot.log(dpp::loglevel::ll_info,
                "Got `on_ready` event... will recreate commands");
        auto lst = dpp::slashcommand("lists", "See lists", bot.me.id),
             nls = dpp::slashcommand("newlist", "Add list", bot.me.id),
             atr = dpp::slashcommand("authors", "See authors", bot.me.id),
             lic = dpp::slashcommand("license", "See license", bot.me.id),
             rep = dpp::slashcommand("repository", "See repository", bot.me.id);
        nls.add_option(dpp::command_option(dpp::co_string,
                                           "list",
                                           "The name of list",
                                           true));
        bot.guild_bulk_command_create({lst, nls, atr, lic, rep},
                                      dpp::snowflake(std::getenv(
                                              o::global::GUILDID_VARNAME)));
        bot.global_bulk_command_delete();
    });
    bot.on_resumed([&bot](const dpp::resumed_t& event) {
        bot.log(dpp::loglevel::ll_info, "Got `on_resumed` event");
    });
    bot.log(dpp::loglevel::ll_info, "Starting bot...");
    bot.start(dpp::st_wait);
    return 0;
}