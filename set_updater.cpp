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

void SetUpdater::parse_update() {
  while (tokens_.size() > 0 && tokens_.size() % 3 == 0) {
    Token attribute = stream_get();
    Token op = stream_get();
    Token value = stream_get();

    if (attribute.first != attribute_name)
      throw QuerySyntaxError("Expected attribute name: " + attribute.second);
    if (op.first != conditional_eq)
      throw QuerySyntaxError("Expected equals operator: " + op.second);
    if (value.first > 15 || value.first > 15)
      throw QuerySyntaxError("Expected literal value: " + value.second);

    record_->set(attribute.second, value.second);
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