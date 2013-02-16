#include "table.h"


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

}

TableIterator Table::end() const {

}


const Record& Table::first() const {

}

const Record& Table::last() const {

}

const Record& Table::at(unsigned int i) const {

}


Table Table::cross_join(const Table& other) const {

}


Table Table::natural_join(const Table& other) const {

}

int Table::count(string column_name) const {

}

