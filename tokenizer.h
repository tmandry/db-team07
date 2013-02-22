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
  bool_not              = 2,

  parenthesis_left      = 3,
  parenthesis_right     = 4,

  conditional_eq        = 5,
  conditional_neq       = 6,
  conditional_lt        = 7,
  conditional_gt        = 8,
  conditional_lte       = 9,
  conditional_gte       = 10,

  value_undefined_type  = 11,
  value_numeral         = 12,
  value_varchar         = 13,
  value_date            = 14,
  value_time            = 15,

  attribute_name        = 16,

  operand_add           = 17,
  operand_subtract      = 18,
  operand_multiply      = 19,
  operand_divide        = 20
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

  static bool is_valid(TokenType type, string str);

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