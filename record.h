#ifndef _RECORD_H_
#define _RECORD_H_
#pragma warning(disable: 4251)

#include <map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#include "exception.h"

/**
 * Allows for read and write access of field values.
 *
 * In the context of a table, fields are known as columns, but within a record
 * they are called fields.
 */
class EXPORT Record {
public:
  /**
   * A const iterator over the fields in a record.
   *
   * Each element accessed by the iterator is a pair. The first element is the
   * field name, the second is the value (in string form.)
   * \sa begin(), end()
   */
  typedef vector<pair<string, string> >::const_iterator RecordIterator;

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
   * Returns an iterator to the first field in the record.
   *
   * Note that the RecordIterator gives you values in string form. If you want
   * automatic conversions, use *get* instead.
   *
   * \sa get(), end()
   */
  RecordIterator begin() const;
  /**
   * Returns an iterator past the end of the fields in the record.
   * \sa begin()
   */
  RecordIterator end() const;

  /**
    Get the value of a field by column name. The field is converted to the
    requested C++ type if possible (otherwise, an \a InvalidTypeError is thrown.)

    Example:
    ~~~{.cpp}
    myRecord.get<int>("age");
    string name = myRecord.get<string>("name");
    ~~~

    Throws a \a ColumnDoesNotExistError if \a field doesn't exist.

    \param T The expected type of the field
    \param field The name of the field (column) in the record.
   */
  template <typename T>
  T get(string field) const;

  template <>
  string get<string>(string field) const;

  /**
    Set the value of a field by column name. The field is converted from the
    given C++ type if possible.

    Example:
    ~~~{.cpp}
    myRecord.set("age", 21);
    myRecord.set("name", "Abraham Lincoln");
    ~~~

    \param T The type of *field*; usually inferred by the compiler.
    \param field The name of the field (column) in the record.
   */
  template <typename T>
  void set(string field, T new_value);

  template <>
  void set<string>(string field, string new_value);

protected:
  friend class Table;
  void join(const Record& other);
  void erase(string field);

private:
  vector<pair<string, string> > values_;
};

template <typename T>
T Record::get(string field) const {
  // This does not work for strings, because stringstream will only give you the
  // first word.
  for (unsigned i = 0; i < values_.size(); i++) {
    if (values_[i].first == field) {
      if (values_[i].second == "")
        return T();

      stringstream ss;
      ss << values_[i].second;

      T value;
      ss >> value;

      return value;
    }
  }

  throw ColumnDoesNotExistError(field);
}

template <>
string Record::get<string>(string field) const {
  for (unsigned i = 0; i < values_.size(); i++) {
    if (values_[i].first == field) {
      return values_[i].second;
    }
  }
  throw ColumnDoesNotExistError(field);
}

template <typename T>
void Record::set(string field, T new_value) {
  // This does not work for strings, because stringstream will only give you the
  // first word.
  if (!TypeIsValid<T>::value)
    throw InvalidTypeError("Invalid type conversion: " + field);

  stringstream ss;
  ss << new_value;
  string string_value;
  ss >> string_value;

  for (unsigned i = 0; i < values_.size(); i++) {
    if (values_[i].first == field) {
      values_[i].second = string_value;
      return;
    }
  }

  values_.push_back(make_pair(field, string_value));
}

template <>
void Record::set<string>(string field, string new_value) {
  for (unsigned i = 0; i < values_.size(); i++) {
    if (values_[i].first == field) {
      values_[i].second = new_value;
      return;
    }
  }

  values_.push_back(make_pair(field, new_value));
}

template< typename T >
struct TypeIsValid
{
    static const bool value = false;
};

// check for type string
template<>
struct TypeIsValid< string >
{
    static const bool value = true;
};

// check for type string literal
template<>
struct TypeIsValid< const char * >
{
    static const bool value = true;
};

// check for type int
template<>
struct TypeIsValid< int >
{
    static const bool value = true;
};

// check for type floating point
template<>
struct TypeIsValid< double >
{
    static const bool value = true;
};

template<>
struct TypeIsValid < float >
{
  static const bool value = true;
};

#endif
