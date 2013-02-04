#ifndef TABLE_H_
#define TABLE_H_

#include <deque>
#include <string>
#include <vector>

using namespace std;

class Record;

/**
 * A table.
 *
 * Tables consist of columns (each with a name and a type) and rows (records).
 *
 * A table may or may not be inside a database. Often it is used for result sets
 * from queries on a larger table.
 */
class Table
{
public:
  /** An enum specifying the type of a column. */
  enum RecordType {
    undefined_type = 0,
    integer        = 1,
    floating       = 2,
    varchar        = 3,
    date           = 4,
    time           = 5
  };

  /**
   * A const iterator over the records in a table.
   *
   * Do not depend on the underlying type of TableIterator in your code, but
   * rather a generic iterator interface.
   */
  typedef deque<Record>::const_iterator TableIterator;

  /** Creates a table with no rows or columns. */
  Table();
  /** Creates a table with the given column names and types and no rows. */
  Table(pair<string, RecordType> fields[]);

  ~Table();

  /**
   * Adds a column to the end of the table.
   *
   * Existing entries will be NULL for this column.
   */
  void add_column(string name, RecordType type);

  /** Deletes a column, erasing any associated data. */
  void del_column(string name);

  /** Renames a column, keeping the existing type and data. */
  void rename_column(string from, string to);

  /** Returns a list of columns and their types. */
  vector<pair<string, RecordType>> columns() const;

  /**
   * Defines the tuple of columns used as a key.
   * \param columns A list of column names that make up the key.
   *
   * Every row in the table must have a unique key. If a new row is inserted
   * with a key that already exists in the table, insertion will fail.
   *
   * This function must be called before inserting any rows into the table.
   */
  void set_key(vector<string> columns);

  /** Returns the number of rows in the table. */
  int size() const;

  /** Inserts a row to the end of a table. */
  void insert(const Record& record);

  /**
   * A Table can be treated as a container. The *begin* and *end* functions
   * correspond to C++ STL container begin and end functions.
   */
  TableIterator begin() const;
  TableIterator end() const;

  /**
   * Computes a cross join with another table.
   *
   * A cross join contains every possible combination of rows in the two
   * tables.
   */
  Table cross_join(const Table& other) const;

  /**
   * Computes a natural join with another table.
   *
   * The other table should have a key, and this table should have columns
   * matching that key.
   */
  Table natural_join(const Table& other) const;

  /**
   * Computes the number of non-NULL values in the given column in the table.
   */
  int count(string column) const;

  /**
   * Computes the sum of all values in the given column in the table.
   */
  template<typename T>
  T sum(string column) const;

  /**
   * Computes the smallest value of all values in the given column in the table.
   */
  template<typename T>
  T min(string column) const;

  /**
   * Computes the largest value of all values in the given column in the table.
   */
  template<typename T>
  T max(string column) const;

private:
  deque<Record> records_;
};

#endif  // TABLE_H_
