#include "database.h"
#include "where_matcher.h"

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
  return new Table;
}

void Database::delete_from(string from, string where) {
	TableMap:: iterator it = tables_.find(from);
	if (it == tables_.end())
			throw TableDoesNotExistError("Table" + from + " could not be found");
	//WhereMatcher(where);
}

void Database::update(string table, string where, string set) {

}

void Database::save(string filename) {

}

void Database::load(string filename) {

}

void Database::merge(const Database& database) {

}

Database Database::copy() {
  return Database();
}
