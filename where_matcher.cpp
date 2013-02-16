#include "where_matcher.h"

#include <sstream>

WhereMatcher::WhereMatcher(string where_clause) {
  Tokenizer tokenizer(where_clause);
  query_ = tokenizer.tokenize();
  query_ = vector<Token>(query_.rbegin(), query_.rend());
}

bool WhereMatcher::does_match(Record record) {
  record_ = record;
  tokens_ = query_;
  return parse_and();
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

  // In C++ you can't declare variables of the same name, even if they were
  // never actually declared, this is a really ugly fix, but if there is a way
  // to emulate some sort of dynamic typing C++, we could make this a lot better
  switch(op_token.first) {
    case conditional_eq:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() == parse_value<int>();
        case value_floating:
          return parse_value<float>() == parse_value<float>();
        case value_varchar:
        case value_date:
        case value_time:
          return parse_value<string>().compare(parse_value<string>()) == 0;
      }
    case conditional_neq:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() != parse_value<int>();
        case value_floating:
          return parse_value<float>() != parse_value<float>();
        case value_varchar:
        case value_date:
        case value_time:
          return parse_value<string>().compare(parse_value<string>()) != 0;
      }
    case conditional_lt:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() < parse_value<int>();
        case value_floating:
          return parse_value<float>() < parse_value<float>();
        case value_varchar:
        case value_date:
        case value_time:
          return parse_value<string>().compare(parse_value<string>()) > 0;
      }
    case conditional_gt:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() > parse_value<int>();
        case value_floating:
          return parse_value<float>() > parse_value<float>();
        case value_varchar:
        case value_date:
        case value_time:
		      return parse_value<string>().compare(parse_value<string>()) < 0;
      }
      break;
    case conditional_lte:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() <= parse_value<int>();
        case value_floating:
          return parse_value<float>() <= parse_value<float>();
        case value_varchar:
        case value_date:
        case value_time:
          return parse_value<string>().compare(parse_value<string>()) >= 0;
      }
      break;
    case conditional_gte:
      switch(value_type) {
        case value_integer:
          return parse_value<int>() >= parse_value<int>();
        case value_floating:
          return parse_value<float>() >= parse_value<float>();
        case value_varchar:
        case value_date:
        case value_time:
          return parse_value<string>().compare(parse_value<string>()) <= 0;
      }
      break;
    default:
      throw QuerySyntaxError("Unrecognized symbol: " + left_token.second);
  }
}

bool WhereMatcher::parse_or() {
  bool left = parse_conditional();
  Token t = stream_get();

  if(t.first == bool_or) {
    return left || parse_conditional();
  } else {
    if(t.first != value_undefined_type) {
      stream_unget(t);
    }
    return left;
  }
}

bool WhereMatcher::parse_and() {
  bool left = parse_or();
  Token t = stream_get();

  if(t.first == bool_and) {
    return left && parse_or();
  } else {
    if(t.first != value_undefined_type) {
      stream_unget(t);
    }
    return left;
  }
}

Token WhereMatcher::stream_get() {
  if (tokens_.size() > 0) {
    vector<Token>::iterator it = tokens_.end();
    it--;
    Token token = *it;
    tokens_.pop_back();
    return token;
  } else {
    return Token(value_undefined_type, "");
  }
}

void WhereMatcher::stream_unget(Token token) {
  tokens_.push_back(token);
}