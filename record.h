#ifndef _RECORD_H_
#define _RECORD_H_

#include <map>
#include<string>
using namespace std;

/**
 * Allows for read and write access of attribute values.
 */
class Record {

public:
  Record();
  /**
    Create a record with existing entries
    \param new_entries a std::map where the key is the name of the entry and the
           value is the value of the attribute.
   */
  Record(map<string, string> new_entries); //!< Create a record with present entries

  ~Record();

  template <typename T>;
  /**
    Get the value of an attribute. Example:

    ~~~{.cpp}
    myRecord.get<int>("age");
    ~~~

    \param T the expected type of the attribute
    \param attribute the name of the attribute (column) in the record.
   */
  T get(string attribute);

  template <typename S>;
  /**
    Set the value of an attribute. Example:

    ~~~{.cpp}
    myRecord.set<int>("age", 21);
    ~~~

    \param S the expected type of the attribute
    \param attribute the name of the attribute (column) in the record.
   */
  bool set(string attribute, <S> value);

};

#endif
