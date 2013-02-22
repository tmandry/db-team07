#include "database.h"
#include "where_matcher.h"

#include <sstream>
#include <iostream>

Database::Database() {
}

Database::~Database() {
  for (auto name_table : tables_)
    delete name_table.second;
}

void Database::add_table(string table_name, Table* table) {
  if (tables_.find(table_name) != tables_.end())
    throw InvalidOperationError("Table " + table_name + " already exists in the database");
  tables_[table_name] = table;
}

void Database::drop_table(string table_name) {
  TableMap::iterator it = tables_.find(table_name);
  if (it == tables_.end())
    throw TableDoesNotExistError("Table " + table_name + " could not be found");
  delete it->second;
  tables_.erase(it);
}

vector<string> Database::table_names() {
  vector<string> ret;
  for (auto name_table : tables_)
    ret.push_back(name_table.first);
  return ret;
}

Table* Database::table(string table_name) {
  TableMap::iterator it = tables_.find(table_name);
  if (it == tables_.end())
    throw TableDoesNotExistError("Table " + table_name + " could not be found");
  return it->second;
}

Table* Database::table_if_exists(string table_name) {
  TableMap::iterator it = tables_.find(table_name);
  if (it == tables_.end())
    return 0;
  return it->second;
}

Table* Database::query(string select, string from, string where) {
  Table *source = table(from);
  Table *results = source->clone_structure();

  Table::TableIterator it;
  WhereMatcher matcher(where);
  for (it = source->begin(); it != source->end(); it++) {
    if (matcher.does_match(*it))
      results->insert(*it);
  }

  Table::ColumnList all_columns = results->columns();
  vector<string> columns_to_select = split_select(select);
  if (columns_to_select[0] != "*")
    for (unsigned int i = 0; i < all_columns.size(); i++) {
      // if column in table is not in selected columns, remove it from query
      if (find(columns_to_select.begin(), columns_to_select.end(), all_columns[i].first) == columns_to_select.end())
        results->del_column(all_columns[i].first);
    }

  return results;
}

void Database::delete_from(string from, string where) {
	Table *source = table(from);
  source->drop_where(where);
}

void Database::update(string table_name, string where, string set) {
  Table *source = table(table_name);
  source->update(where, set);
}

void Database::save(string filename) {

}

void Database::load(string filename) {

}

void Database::merge(const Database& database) {
  for (auto name_table : database.tables_) {
    delete tables_[name_table.first];  // returns NULL if it doesn't exist, and delete does nothing
    tables_[name_table.first] = new Table(*name_table.second);
  }
}

Database Database::copy() {
  return Database();
}

vector<string> Database::split_select(string select) {
  stringstream ss(select);

  vector<string> result;
  string buffer;
  while (ss >> buffer) {
    if (buffer[buffer.size() - 1] == ',')
      buffer.pop_back();
    result.push_back(buffer);
  }

  return result;
}
