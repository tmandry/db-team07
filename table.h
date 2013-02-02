#ifndef TABLE_H_
#define TABLE_H_

#include <deque>

class Table
{
public:
  enum RecordType {
    type_unknown = 0,
    type_integer,
    type_floating,
    type_varchar,
    type_date
  };

  // Default constructor. Creates a table with no rows or columns.
  Table();
  Table(pair<string, RecordType> fields[]);

  ~Table();

  // Adds a column to the end of the table. Existing entries will be NULL for
  // this column.
  void add_column(string name, RecordType type);
  void del_column(string name);
  void rename_column(string from, string to);

  vector<pair<string, RecordType>> columns() const;

  int size() const;

  void insert(const Record& record);

  // A Table can be treated as a container. These correspond to C++ STL
  // container begin and end functions.
  RecordIterator begin() const;
  RecordIterator end() const;

  Table cross_join(const Table& other) const;

  int count(string column) const;
  // template these!
  float sum(string column) const;
  float min(string column) const;
  float max(string column) const;

private:
  deque<Record> records_;
};

#endif  // TABLE_H_
