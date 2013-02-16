#include "table.h"

Table::Table() {
	records_ = deque<Record> ();
	columns_ = vector<ColumnList> ();
};

Table::Table(const ColumnList& columns) {
	columns_ = columns;
};

Table::~Table() {
	delete columns_;
};

void add_column(string column_name, RecordType type) { 
	records_.pushback();
};

void Table::add_column(string column_name, RecordType type) {

}

void Table::del_column(string column_name) {

}

void Table::rename_column(string from, string to) {

}

ColumnList Table::columns() const {

}

unsigned int Table::index_for(string column_name) const {

}

void Table::set_key(vector<string> column_names) {

}

int Table::size() const {

}

void Table::insert(const Record& record) {

}

TableIterator Table::begin() const {
  return records_.begin();
}

TableIterator Table::end() const {
  return records_.end();
}

const Record& Table::first() const {
  return records_.first();
}

const Record& Table::last() const {
  return records_.last();
}

const Record& Table::at(unsigned int i) const {
  return records_.at(i);
  // TODO catch/rethrow exception
}

Table Table::cross_join(const Table& other) const {

}

Table Table::natural_join(const Table& other) const {

}

int Table::count(string column_name) const {
  int ret = 0;
  for (const Record& record : records_) {
    if (record.get(column_name) != "NULL")
      ++ret;
  }
  return ret;
}

