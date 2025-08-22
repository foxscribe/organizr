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

#include "strings.h"

int main() {
  dpp::cluster bot(std::getenv("DISCORD_TOKEN"));
  bot.on_log(dpp::utility::cout_logger());
  bot.on_slashcommand([](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "authors") {
      event.reply(o::s::AUTHORS);
    } else if (event.command.get_command_name() == "license") {
      event.reply(o::s::LICENSE);
    } else if (event.command.get_command_name() == "repository") {
      event.reply("Not available yet");
    }
  });
  bot.on_ready([&bot](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(
          dpp::slashcommand("authors", "See authors", bot.me.id));
      bot.global_command_create(
          dpp::slashcommand("license", "See license", bot.me.id));
      bot.global_command_create(
          dpp::slashcommand("repository", "See repository", bot.me.id));
    }
  });
  bot.start(dpp::st_wait);
  return 0;
}