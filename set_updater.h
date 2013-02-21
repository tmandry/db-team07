#ifndef SETUPDATER_H_
#define SETUPDATER_H_

#include "exception.h"
#include "record.h"
#include "tokenizer.h"

#include <vector>
#include <string>
using namespace std;

class EXPORT SetUpdater {
public:
  SetUpdater(string set_clause);
  void update(Record record);
private:
  vector<Token> tokens_;
  vector<Token> set_;
  Record record_;

  void parse_update();

  Token stream_get();
  void stream_unget(Token token);
};

#endif