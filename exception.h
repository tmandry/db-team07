/**
 * \file exception.h
 * Defines the exception classes used by this database.
 *
 * Every exception inherits from std::exception, and a detailed message can be
 * obtained using *what()*.
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>

using namespace std;

class ColumnDoesNotExistError : public invalid_argument {
public:
  ColumnDoesNotExistError(const string& what)
    : invalid_argument(what) {}
};

class RowDoesNotExistError : public invalid_argument {
public:
  RowDoesNotExistError(const string& what)
    : invalid_argument(what) {}
};

class TableDoesNotExistError : public invalid_argument {
public:
  TableDoesNotExistError(const string& what)
    : invalid_argument(what) {}
};

class InvalidOperationError : public logic_error {
public:
  InvalidOperationError(const string& what)
    : logic_error(what) {}
};

class KeyConflictError : public InvalidOperationError {
public:
  KeyConflictError(const string& what)
    : InvalidOperationError(what) {}
};

/** Thrown when the specified value cannot be converted to the requested type. */
class InvalidTypeError : public logic_error {
public:
  InvalidTypeError(const string& what)
    : logic_error(what) {}
};

class QuerySyntaxError : public invalid_argument {
public:
  QuerySyntaxError(const string& what)
    : invalid_argument(what) {}
};

class IOError : public runtime_error {
public:
  IOError(const string& what)
    : runtime_error(what) {}
};

#endif  // EXCEPTION_H_
