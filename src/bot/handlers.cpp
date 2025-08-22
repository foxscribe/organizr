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

#include "handlers.h"

#include "../global.h"
#include "../strings.h"

namespace o::bot::handlers {
constexpr auto& db = o::global::wrapper;
void authors(const dpp::slashcommand_t& event) {
    event.reply(dpp::message(o::s::AUTHORS).set_flags(dpp::m_ephemeral));
}
void license(const dpp::slashcommand_t& event) {
    event.reply(dpp::message(o::s::LICENSE).set_flags(dpp::m_ephemeral));
}
void repository(const dpp::slashcommand_t& event) {
    dpp::embed embed =
            dpp::embed()
                    .set_color(dpp::colors::orange_gold)
                    .set_title("Organizr")
                    .set_url(o::s::REPO_URL)
                    .set_description("A discord bot to organize lists");
    event.reply(dpp::message(embed).set_flags(dpp::m_ephemeral));
}
void lists(const dpp::slashcommand_t& event) {
    auto lists = db.getLists();
    if (lists.empty()) {
        event.reply(
                dpp::message("No lists available").set_flags(dpp::m_ephemeral));
        return;
    }
    std::string message = "";
    for (auto l : lists) {
        message += l.message();
    }
    event.reply(message);
}
void newlist(const dpp::slashcommand_t& event) {
    try {
        std::string name = std::get<std::string>(event.get_parameter("list"));
        db.addList(name);
        event.reply(dpp::message("Added new list").set_flags(dpp::m_ephemeral));
    } catch (std::runtime_error err) {
        event.reply(dpp::message("Cannot add the list you requested")
                            .set_flags(dpp::m_ephemeral));
    }
}
} // namespace o::bot::handlers