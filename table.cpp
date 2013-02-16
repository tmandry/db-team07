#include "table.h"
#include "exception.h"
#include "record.h"

Table::Table() {
	records_ = deque<Record> ();
	columns_ = ColumnList ();
}

Table::Table(const ColumnList& columns) {
	columns_ = columns;
}

Table::~Table() {
}

void Table::add_column(string column_name, RecordType type) {
	columns_.push_back(make_pair(column_name, type));
}

void Table::del_column(string column_name) {
	for(ColumnList::iterator it = columns_.begin(); it < columns_.end(); ++it) {
		if(it->first == column_name) {
			columns_.erase(it);
      return;
    }
	}
	throw ColumnDoesNotExistError("Could not find column " + column_name);
}

void Table::rename_column(string from, string to) {
	for (unsigned i = 0; i < columns_.size(); i++) {
		if(columns_[i].first == from)
			columns_[i].first = to;
	}
	throw ColumnDoesNotExistError("Could not find column " + from);
}

Table::ColumnList Table::columns() const {
  return columns_;
}

unsigned int Table::index_for(string column_name) const {
  for (unsigned int i = 0; i < columns_.size(); ++i)
    if (columns_[i].first == column_name)
      return i;
  throw ColumnDoesNotExistError("Could not find column " + column_name);
}

void Table::set_key(vector<string> column_names) {

}

int Table::size() const {
  return records_.size();
}

void Table::insert(const Record& record) {
  records_.push_back(record);
}

Table::TableIterator Table::begin() const {
  return records_.begin();
}

Table::TableIterator Table::end() const {
  return records_.end();
}

const Record& Table::first() const {
  return records_.front();
}

const Record& Table::last() const {
  return records_.back();
}

const Record& Table::at(unsigned int i) const {
  return records_.at(i);
  // TODO catch/rethrow exception
}

Table Table::cross_join(const Table& other) const {
  ColumnList join_columns(columns_);
  join_columns.insert(join_columns.end(), other.columns_.begin(), other.columns_.end());
  Table join(join_columns);

  for (const Record& record1 : records_) {
    for (const Record& record2 : other.records_) {
      Record join_record(record1);
      join_record.join(record2);
      join.insert(join_record);
    }
  }
  return join;
}

Table Table::natural_join(const Table& other) const {
  return Table();
}

int Table::count(string column_name) const {
  int ret = 0;
  for (const Record& record : records_) {
    if (record.get<string>(column_name) != "NULL")
      ++ret;
  }
  return ret;
}

