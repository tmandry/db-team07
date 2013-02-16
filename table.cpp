#include "table.h"
#include "exception.h"

Table::Table() {
	records_ = deque<Record> ();
	columns_ = ColumnList ();
};

Table::Table(const ColumnList& columns) {
	columns_ = columns;
};

Table::~Table() {
	delete columns_;
	delete records_;
};

void Table::add_column(string column_name, RecordType type) { 
	columns_.push_back(make_pair(column_name, type));
};

void Table::del_column(string column_name) {
	for(int i = 0; i < columns_.size(); i++) {
		if(columns_[i].first == column_name)
			delete column_[i];
	}
	throw ColumnDoesNotExistError("Could not find column " + column_name);
}

void Table::rename_column(string from, string to) {
	for(int i = 0; i < columns_.size(); i++) {
		if(columns_[i].first == from)
			column_[i].first = to;
	}
	throw ColumnDoesNotExistError("Could not find column " + from);
}	

ColumnList Table::columns() const {

}

unsigned int Table::index_for(string column_name) const {
  for (unsigned int i = 0; i < columns_.size(); ++i)
    if (columns_[i] == column_name)
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
  ColumnList join_columns(columns_);
  join_columns.insert(join_columns.end(), other.columns_.begin(), other.columns_.end());
  Table join(join_columns);

  for (const Record& record1 : records_) {
    for (const Record& record2 : other.records_) {
      Record join_record(record1);
      join_record.join(other);
      join.insert(join_record);
    }
  }
  return join;
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

