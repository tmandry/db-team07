#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <string>
#include <vector>
#include <map>

#include "record.h"
#include "table.h"

/** Database is the entry point for creating tables, deleting records,
    and running queries.
 */
class Database {
public:
  Database(); //!< Creates an empty database

  //! Add a table to the database
  /**
    \param name what to call the table in the database
    \param table the Table to be inserted into the table
    \sa Table
   */
  void create_table(string name, Table* table);

  //! Remove a table from the database
  /**
    \param name which table to remove from the database
    \sa list_tables()
   */
  void drop_table(string name);

  //! Get a list of all the tables currently in the database
  vector<string> list_tables();

  //! Get pointers to all of the tables currently in the database
  map<string, Table*> get_tables();

  //! Perform a query on the database
  /**
    \param select which columns to include in the returned Table
    \param from which table to query from
    \param where the conditions for the query to match
    \return A pointer to Table with all of the records that match the query
   */
  Table* query(string select, string from, string where);

  //! Delete all records that match the query
  /**
    \param select which columns to include in the returned Table
    \param from which table to query from
    \param where the conditions for the query to match
   */
  void delete(string select, string from, string where);
};

#endif