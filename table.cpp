#include "table.h"
#include "exception.h"
#include "record.h"
#include "where_matcher.h"
#include "set_updater.h"

Table::Table() {
	records_ = deque<Record> ();
	columns_ = ColumnList ();
}

Table::Table(const ColumnList& columns) {
	columns_ = columns;
}

Table::~Table() {
}

Table* Table::clone_structure() {
  Table *clone = new Table(columns());
  clone->set_key(key());

  return clone;
}

void Table::add_column(string column_name, RecordType type) {
	columns_.push_back(make_pair(column_name, type));
}

void Table::del_column(string column_name) {
  for (ColumnList::iterator it = columns_.begin(); it != columns_.end(); ++it) {
	  if (it->first == column_name) {
		  columns_.erase(it);
      for (deque<Record>::iterator itt = records_.begin(); itt != records_.end(); itt++)
        itt->erase(column_name);
      return;
     }
	}
  throw ColumnDoesNotExistError("Could not find column " + column_name);
}

void Table::rename_column(string from, string to) {
	for (unsigned i = 0; i < columns_.size(); i++) {
		if (columns_[i].first == from) {
      columns_[i].first = to;
      return;
    }
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
  if (size() != 0)
    throw InvalidOperationError("Cannot add key to non-empty table");
	// TODO needs to check for duplicates within row
  bool key_found;
	for (string col : column_names) {
    key_found = false;
    for (ColumnList::iterator it = columns_.begin(); it != columns_.end(); it++)
      if (it->first == col)
        key_found = true;
    if (!key_found)
      throw ColumnDoesNotExistError("Could not find column " + col);
  }
	key_ = column_names;
}

vector<string> Table::key() const {
	return key_;
}

int Table::size() const {
  return records_.size();
}

void Table::insert(const Record& record) {
  // Check column names and order
  if (record.values_.size() != columns_.size()) {
    throw ColumnDoesNotExistError("Number of columns in record does not match number of columns in table");
  }
  for (unsigned i = 0; i < record.values_.size(); ++i) {
    if (record.values_[i].first != columns_[i].first) {
      throw ColumnDoesNotExistError("Column " + record.values_[i].first + " in record does not match column in table (" + columns_[i].first + ")");
    }
  }

  // If there is a key, check for conflicts
	if (!key().empty()) {
		for (unsigned i = 0; i < records_.size(); i++) {
      bool unequal = false;
			for (unsigned j = 0; j < key_.size(); j++) {
				if (records_[i].get<string>(key_[j]) != record.get<string>(key_[j])) {
          unequal = true;
          break;
				}
			}
      if (!unequal) {
        throw KeyConflictError("Already a record with this key");
      }
		}
	}

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
  if (i > records_.size())
    throw InvalidOperationError("Index out of Range");
  return records_.at(i);
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
  if (!has_column(column_name))
    throw ColumnDoesNotExistError("Could not find column " + column_name);

  int ret = 0;
  for (const Record& record : records_)
    if (record.get<string>(column_name) != "NULL")
      ++ret;
  return ret;
}

void Table::drop_where(string where) {
  deque<Record>::iterator it = records_.begin();
  WhereMatcher matcher(where);
  while (it != records_.end()) {
    if (matcher.does_match(*it))
      it = drop(it);
    else
      it++;
  }
}

void Table::update(string where, string set) {
  deque<Record>::iterator it;
  WhereMatcher matcher(where);
  SetUpdater updater(set);
  for (it = records_.begin(); it != records_.end(); it++)
    if (matcher.does_match(*it))
      updater.update(it);
}

bool Table::has_column(string column_name) const {
  for (auto name_type : columns_)
    if (name_type.first == column_name) return true;
  return false;
}

deque<Record>::iterator Table::drop(deque<Record>::iterator record) {
  return records_.erase(record);
}