#ifndef _RECORD_H_
#define _RECORD_H_

#include <map>
#include <string>
using namespace std;

/**
 * Allows for read and write access of field values.
 */
class Record {
public:
  /**
    Create a record with no fields and no data.
    */
  Record();
  /**
    Create a record with existing entries
    \param entries a std::vector of pairs, with the first element in the pair
           being the field/column name, and the second element being the value.
   */
  Record(vector<pair<string, string> > entries);

  ~Record();

  /**
    Get the value of a field by column name. Example:

    ~~~{.cpp}
    myRecord.get<int>("age");
    string name = myRecord.get("name");  // type T is inferred
    ~~~

    \param T The expected type of the field
    \param field The name of the field (column) in the record.
   */
  template <typename T>
  T get(string field);

  /**
    Set the value of a field by column name. Example:

    ~~~{.cpp}
    myRecord.set("age", 21);
    ~~~

    \param T The type of *field*; usually inferred by the compiler.
    \param field The name of the field (column) in the record.
   */
  template <typename T>
  bool set(string field, T value);

private:
  vector<pair<string, string> > values_;
};

#endif
