#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "exception.h"

#include <string>
#include <vector>

using namespace std;

/** An enum specifying the type of the token */
enum TokenType {
  bool_or               = 0,
  bool_and              = 1,

  parenthesis_left      = 2,
  parenthesis_right     = 3,

  conditional_eq        = 4,
  conditional_neq       = 5,
  conditional_lt        = 6,
  conditional_gt        = 7,
  conditional_lte       = 8,
  conditional_gte       = 9,

  value_undefined_type  = 10,
  value_numeral         = 11,
  value_varchar         = 12,
  value_date            = 13,
  value_time            = 14,

  attribute_name        = 15
};

enum TokenizerType {
  where                 = 0,
  update                = 1
};

/**
 * A token is used in parsing to split a string into easy to use blocks of logic
 * associated with its purpose.
 */
typedef pair<TokenType, string> Token;

class Tokenizer {
public:

  /** Constructors takes the source string to be parsed */
  Tokenizer(TokenizerType type, string source);

  /** Returns a vector of tokens parsed from a where clause */
  vector<Token> tokenize();
private:
  char stream_get(bool include_space = true);
  void stream_unget(char c);
  static bool string_contains(string source, char target);
  void handle_attribute_name(char first);

  vector<Token> tokens_;
  string stream_;
  TokenizerType type_;
};


#endif