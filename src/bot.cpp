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
#include <iostream>
#include <ostream>
#include <stdexcept>

#include <dpp/dpp.h>
#include <pqxx/pqxx>

#include "db.h"
#include "strings.h"

constexpr const char* TOKEN_VARNAME   = "DISCORD_TOKEN";
constexpr const char* URL_VARNAME     = "DATABASE_URL";
constexpr const char* GUILDID_VARNAME = "GUILD_ID";

#include <chrono>
#include <iomanip>
#include <iostream>

#include <dpp/dpp.h>

void log_event(const dpp::log_t& event) {
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

int main() {
    pqxx::connection cx(std::getenv(URL_VARNAME));
    dpp::cluster bot(std::getenv(TOKEN_VARNAME));
    bot.on_log(log_event);
    bot.on_slashcommand([&bot, &cx](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "authors") {
            event.reply(
                    dpp::message(o::s::AUTHORS).set_flags(dpp::m_ephemeral));
        } else if (event.command.get_command_name() == "license") {
            event.reply(
                    dpp::message(o::s::LICENSE).set_flags(dpp::m_ephemeral));
        } else if (event.command.get_command_name() == "repository") {
            dpp::embed embed =
                    dpp::embed()
                            .set_color(dpp::colors::orange_gold)
                            .set_title("Organizr")
                            .set_url(o::s::REPO_URL)
                            .set_description("A discord bot to organize lists");
            event.reply(dpp::message(embed).set_flags(dpp::m_ephemeral));
        } else if (event.command.get_command_name() == "lists") {
            auto lists = o::db::getLists(cx);
            if (lists.empty()) {
                event.reply(dpp::message("No lists available")
                                    .set_flags(dpp::m_ephemeral));
                return;
            }
            std::string message = "";
            for (auto l : lists) {
                message += l.message();
            }
            event.reply(message);
        } else if (event.command.get_command_name() == "newlist") {
            try {
                std::string name =
                        std::get<std::string>(event.get_parameter("list"));
                pqxx::work tx(cx);
                tx.exec("INSERT INTO list (val) VALUES ($1)",
                        pqxx::params{name});
                tx.commit();
                event.reply(dpp::message("Added new list")
                                    .set_flags(dpp::m_ephemeral));
            } catch (std::runtime_error err) {
                bot.log(dpp::loglevel::ll_critical, err.what());
                event.reply(dpp::message("Cannot add the list you requested")
                                    .set_flags(dpp::m_ephemeral));
            }
        } else if (event.command.get_command_name() == "newtask") {
            auto lists          = o::db::getLists(cx);
            std::string message = "";
            for (auto l : lists) {
                message += l.message();
            }
            event.reply(message);
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
                                      dpp::snowflake(
                                              std::getenv(GUILDID_VARNAME)));
        bot.global_bulk_command_delete();
    });
    bot.on_resumed([&bot](const dpp::resumed_t& event) {
        bot.log(dpp::loglevel::ll_info, "Got `on_resumed` event");
    });
    bot.log(dpp::loglevel::ll_info, "Starting bot...");
    bot.start(dpp::st_wait);
    return 0;
}