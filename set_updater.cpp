#include "set_updater.h"
#include "table.h"

#include <sstream>

SetUpdater::SetUpdater(string set_clause) {
  Tokenizer tokenizer(TokenizerType::update, set_clause);
  set_ = tokenizer.tokenize();
  set_ = vector<Token>(set_.rbegin(), set_.rend());
}

void SetUpdater::update(deque<Record>::iterator record) {
  record_ = record;
  tokens_ = set_;
  return parse_update();
}

template<typename T>
T SetUpdater::primary() {
  Token t = stream_get();

  if (t.first == attribute_name)
    return record_->get<T>(t.second);
  else
    return (T) t.second;
}

template<typename T>
T SetUpdater::term() {
  T left = primary<T>();
  Token t = stream_get();

  switch (t.first) {
    case operand_multiply:
      return left * primary();
    case operand_divide:
      return left / primary();
    default:
      stream_unget(t);
      return left;
  }
}

string SetUpdater::expression() {
  T left = term<T>();
  Token t = stream_get();

  switch (t.first) {
    case operand_add:
      return left + term();
    case operand_subtract:
      return left - term();
    default:
      stream_unget(t);
      return left;
  }
}


void SetUpdater::parse_update() {
  while (tokens_.size() > 0 && tokens_.size() % 3 == 0) {
    Token attribute = stream_get();
    Token op = stream_get();

    if (attribute.first != attribute_name)
      throw QuerySyntaxError("Expected attribute name: " + attribute.second);
    if (op.first != conditional_eq)
      throw QuerySyntaxError("Expected equals operator: " + op.second);

    record_->set(attribute.second, expression());
  }

  if (tokens_.size() != 0)
    throw QuerySyntaxError("Set clause syntax error.");
}

Token SetUpdater::stream_get() {
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

void SetUpdater::stream_unget(Token token) {
  tokens_.push_back(token);
}