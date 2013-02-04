#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "record.h"
#include "table.h"

class Database {
public:
  Database();

  void create_table(string name, Table table);
  void drop_table(string name);
  vector<string> list_tables();
  vector<Table> get_tables();

  Table query(string select, string from, string where);
  void delete(string select, string from, string where);
};

#endif