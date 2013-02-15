#ifndef WHEREMATCHER_H_
#define WHEREMATCHER_H_

#include "Record.h"
#include "Tokenizer.h"

#include <vector>
#include <string>
using namespace std;

class WhereMatcher {
public:
  WhereMatcher(string where_clause);
  bool does_match(Record record);
private:
  vector<Tokenizer::Token> tokens_;
};

#endif