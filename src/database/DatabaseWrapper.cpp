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

#include "DatabaseWrapper.h"

#include <string>

namespace o::db {
DatabaseWrapper::DatabaseWrapper(std::string uri) : cx(pqxx::connection(uri)) {}
std::vector<o::list> DatabaseWrapper::getLists() {
    std::vector<o::list> result;
    pqxx::work tx(cx);
    pqxx::result rs = tx.exec("SELECT l.id, l.val, t.val "
                              "FROM list l "
                              "LEFT JOIN task t ON l.id = t.list_id "
                              "ORDER BY l.id");

    std::unordered_map<long, o::list> listsMap;
    for (const auto& row : rs) {
        long listId                  = row[0].as<long>();
        const std::string& listVal   = row[1].as<std::string>();
        const pqxx::field& taskField = row[2];

        auto it = listsMap.find(listId);
        if (it == listsMap.end()) {
            o::list newList;
            newList.val = listVal;
            if (!taskField.is_null()) {
                newList.tasks.push_back(taskField.as<std::string>());
            }
            listsMap[listId] = std::move(newList);
        } else if (!taskField.is_null()) {
            it->second.tasks.push_back(taskField.as<std::string>());
        }
    }
    for (auto& [_, l] : listsMap) {
        result.push_back(std::move(l));
    }
    tx.commit();
    return result;
}
void DatabaseWrapper::addList(const std::string& name) {
    pqxx::work tx(cx);
    tx.exec("INSERT INTO list (val) VALUES ($1)", pqxx::params{name});
    tx.commit();
}
} // namespace o::db