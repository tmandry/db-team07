#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <string>
#include <vector>
#include <map>

#include "record.h"
#include "table.h"

/** The entry point for creating tables, deleting records,
    and running queries.
 */
class Database {
public:
  Database(); //!< Creates an empty database

  /**
    Add a table to the database
    \param name what to call the table in the database
    \param table the Table to be inserted into the table
    \sa Table
   */
  void create_table(string name, Table* table);

  /**
    Remove a table from the database
    \param name which table to remove from the database
    \sa list_tables()
   */
  void drop_table(string name);

  /**
    Get a list of all the tables currently in the database
   */
  vector<string> list_tables();

  /**
    Get pointers to all of the tables currently in the database
   */
  map<string, Table*> get_tables();

  /**
    Perform a query on the database. An example query:

    ~~~{.cpp}
    myDatabase.query("name", "students", "gender == 'male'");
    ~~~

    \param select which columns to include in the returned Table
    \param from which table to query from
    \param where the conditions for the query to match
    \return A pointer to Table with all of the records that match the query
   */
  Table* query(string select, string from, string where);

  /**
    Delete all records that match the query
    \param select which columns to include in the returned Table
    \param from which table to query from
    \param where the conditions for the query to match
   */
  void delete(string select, string from, string where);

  /**
    Mass modify records in table. Examples:

    ~~~{.cpp}
    myDatabase.update("students", "gender == 'male'", "gender = 'm'");

    myDatabase.update("students", "gender == 'male'", "gender = 'm', school = 'A&M'");

    myDatabase.update("students", "gender == 'female'", "age = age * 2");
    ~~~

    \param table name of the table to update records in
    \param where a SQL where clause to find records in the table
    \param set a SQL set clause
   */
  void update(string table, string where, string set);

  /**
    Save the database to a file
    \param filename the output file
   */
  void save(string filename);

  /**
    Load a database from a file, this will clear any existing records
    \param filename the input file
   */
  void load(string filename);

  /**
    Merge two another database into this one. The database at the pointer will not be affected.
    \param database A pointer to the database that you want to merge INTO this
           one.
   */
   void merge(Database* database);

  /**
    Make a copy of this database
    \returns a one for one copy / clone of this database
   */
  Database copy();
};

#endif