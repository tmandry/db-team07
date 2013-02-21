#include "tokenizer.h"

#include <sstream>

Tokenizer::Tokenizer(TokenizerType type, string source) {
  type_ = type;
  stream_ = string ( source.rbegin(), source.rend() );
}

// This looks disgusting, if you have a creative way of splitting it up, go
// for it.
vector<Token> Tokenizer::tokenize() {
  while (stream_.size() > 0) {
    char c = stream_get();
    char second;
    switch (c) {
      // parenthesis_left
      case '(':
        tokens_.push_back( Token(parenthesis_left, "(") );
        break;

      // parenthesis_right
      case ')':
        tokens_.push_back( Token(parenthesis_right, ")") );
        break;

      // bool_or
      case 'O':
        c = stream_get();
        if (c == 'R') {
          tokens_.push_back( Token(bool_or, "OR") );
          break;
        } else {
          stream_unget(c);
          handle_attribute_name('O');
        }

      // bool_and
      case 'A':
        c = stream_get();
        second = stream_get();
        if (c == 'N' && second == 'D') {
          tokens_.push_back( Token(bool_and, "AND") );
          break;
        } else {
          stream_unget(second);
          stream_unget(c);
          handle_attribute_name('A');
        }

      // condtional_lt / condtional_lte
      case '<':
        c = stream_get();
        if (c == '=') {
          tokens_.push_back( Token(conditional_lte, "<=") );
        } else {
          stream_unget(c);
          tokens_.push_back( Token(conditional_lt, "<") );
        }
        break;

      // condtional_gt / condtional_gte
      case '>':
        c = stream_get();
        if (c == '=') {
          tokens_.push_back( Token(conditional_gte, ">=") );
        } else {
          stream_unget(c);
          tokens_.push_back( Token(conditional_gt, ">") );
        }
        break;

      // condtional_neq
      case '!':
        c = stream_get();
        if (c == '=') {
          tokens_.push_back( Token(conditional_neq, "!=") );
        } else {
          stream_unget(c);
          throw QuerySyntaxError("Invalid syntax in: " + stream_);
        }
        break;

      // condtional_eq
      case '=':
        tokens_.push_back( Token(conditional_eq, "=") );
        break;

      // catch all for value_* and attribute_name
      default:

        // looks at the first character in the next token:
        //   if single quote: varchar
        //   if numeral: float, date, time, or int
        //   else: attribute name
        if (c == 39) {
          string value;

          while (true) {
            c = stream_get(false);

            // if single quote, we are done
            if (c == 39) {
              break;
            }

            value.push_back(c);
          }

          tokens_.push_back( Token(value_varchar, value) );
        } else if (c >= 48 && c <= 57) {
          string value(1, c);

          while (c = stream_get(false)) {
            if (c >= 48 && c <= 57 || c == 46 || c == 47 || c == 58) {
              value.push_back(c);
            } else {
              stream_unget(c);
              break;
            }
          }

          // discern between floats, dates, times, and ints
          if (string_contains(value, 47)) {
            tokens_.push_back( Token(value_date, value) );
          } else if (string_contains(value, 58)) {
            tokens_.push_back( Token(value_time, value) );
          } else {
            tokens_.push_back( Token(value_numeral, value) );
          }
        } else {
          handle_attribute_name(c);
        }
        break;
    }
  }

  return tokens_;
}

void Tokenizer::handle_attribute_name(char first) {
  string attribute(1,first);
  while (true) {
    first = stream_get(false);
    if (first != 32 && first != ')') {
      attribute.push_back(first);
    } else {
      tokens_.push_back( Token(attribute_name, attribute) );
      break;
    }
  }
}

char Tokenizer::stream_get(bool skip_space) {
  // return NULL character if end of stream was reached
  if (stream_.size() == 0) {
    return 0;
  }

  char c;
  // find the next, nonspace (and if update, noncomma) character
  do {
    if (skip_space) {
      c = 32;
      while (c == 32) {
        c = stream_[stream_.size() - 1];
        stream_.pop_back();
      }
    } else {
      c = stream_[stream_.size() - 1];
      stream_.pop_back();
    }
  } while (c == ',' && type_ == update);

  return c;
}

void Tokenizer::stream_unget(char c) {
  stream_.push_back(c);
}

/** Searches through a string for a specific character */
bool Tokenizer::string_contains(string source, char target) {
  for (unsigned i = 0; i < source.size(); i++) {
    if (source[i] == target) {
      return true;
    }
  }
  return false;
}