#include "database.h"

Database::Database() {
}

void Database::add_table(string name, Table* table) {

}


void Database::drop_table(string name) {

}

vector<string> Database::table_names() {
  return vector<string>();
}


Table* Database::table(string table_name) {
  return 0;
}


Table* Database::table_if_exists(string table_name) {
  return 0;
}


Table* Database::query(string select, string from, string where) {
  return 0;
}


void Database::delete_from(string from, string where) {

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
