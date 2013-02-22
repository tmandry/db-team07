#ifndef WHEREMATCHER_H_
#define WHEREMATCHER_H_

#include "exception.h"
#include "record.h"
#include "tokenizer.h"

#include <vector>
#include <string>
using namespace std;

class EXPORT WhereMatcher {
public:
  WhereMatcher(string where_clause);
  bool does_match(Record record);
private:
  vector<Token> tokens_;
  vector<Token> query_;
  Record record_;

  bool parse_conditional();
  bool parse_not();
  bool parse_or();
  bool parse_and();

  template <typename T>
  T parse_value();

  Token stream_get();
  void stream_unget(Token token);
};

template <typename T>
T WhereMatcher::parse_value() {
    Token t = stream_get();

    if (t.first == attribute_name) {
      return record_.get<T>(t.second);
    } else {
      stringstream ss;
      ss << t.second;

      T value;
      ss >> value;

      return value;
    }
  }

#endif