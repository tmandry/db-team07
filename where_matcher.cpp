#include "where_matcher.h"

#include <sstream>

WhereMatcher::WhereMatcher(string where_clause) {
  Tokenizer tokenizer(where_clause);
  tokens_ = tokenizer.tokenize().reverse();
}

bool WhereMatcher::does_match(Record record) {
  record_ = record;
  return parse_and();
}

template <typename T>
T WhereMatcher::parse_value() {
  Token t = stream_get();

  if(t.first == attribute_name) {
    return record_.get<T>(t.second);
  } else {
    // will not handle dates or times currently
    stringstream ss;
    ss << t.second;

    T value;
    ss >> value;

    return value;
  }
}

bool WhereMatcher::parse_conditional() {
  Token left_token = stream_get();
  Token op_token = stream_get();
  TokenType value_type = left_token.first;

  // the left side is an attribute_name, we need to look ahead for the type
  if(value_type == attribute_name) {
    Token right_token = stream_get();
    value_type = right_token.first;
    stream_unget(right_token);
  }

  stream_unget(left_token);

  // apparently you can't declare variables of the same name, even if they were
  // never actually declared, need to figure something out
  switch(value_type) {
    case value_integer:
      int left = parse_value<int>();
      int right = parse_value<int>();
      break;
    case value_floating:
      float left = parse_value<float>();
      float right = parse_value<float>();
      break;
    case value_varchar:
      string left = parse_value<string>();
      string right = parse_value<string>();
      break;
    // not worrying about dates or times yet
    // case value_date:
    //   date left = <date>value();
    //   date right = <date>value();
    //   break;
    // case value_time:
    //   time left = <time>value();
    //   time right = <time>value();
  }

  switch(op_token.first) {
    case conditional_eq:
      return left == right;
    case conditional_neq:
      return left != right;
    case conditional_lt:
      return left < right;
    case conditional_gt:
      return left > right;
    case conditional_lte:
      return left <= right;
    case conditional_gte:
      return left >= right;
    default:
      stream_unget(t);
      // error
  }
}

bool WhereMatcher::parse_or() {
  bool left = parse_conditional();
  Token t = stream_get();

  if(t.first == bool_or) {
    return left || parse_conditional();
  } else {
    stream_unget(t);
    return left;
  }
}

bool WhereMatcher::parse_and() {
  bool left = parse_or();
  Token t = stream_get();

  if(t.first == bool_and) {
    return left && parse_or();
  } else {
    stream_unget(t);
    return left;
  }
}

Token WhereMatcher::stream_get() {
  Token token = *tokens_.end();
  tokens_.pop_back();

  return token;
}

void WhereMatcher::stream_unget(Token token) {
  tokens_.push_back(token);
}