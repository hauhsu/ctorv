#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;
enum Tag {
  // Other ASCII code occupies 0 ~ 127
    NUMBER   = 128,
    IDENTIFIER    ,
    // keywords
    TRUE          ,
    FALSE         ,
    RETURN        ,
    //   type
    TYPE    ,
    // punctuators
    LEFT_PAREN    , // (
    RIGHT_PAREN   , // )
    LEFT_CURRY    , // {
    RIGHT_CURRY   , // }
    ASSIGN        , // =
    EQ            , // ==
    BITWISE_OR    , // |
    LOGIC_OR      , // ||
    SEMICOLON     , // ;
    MINUS         , // -
    PLUS          , // +
};

struct Token {
  Token(int t): tag(t) {;}
  virtual ~Token(){;}
  virtual string to_string() const;
  friend ostream &operator<< (ostream &os, const Token& token);

  int tag;
};

struct Num: Token {
  Num(const int v): Token(Tag::NUMBER), val(v) {;}
  virtual string to_string() const;
  int val;
};

struct Id: Token {
  Id(const string s): Token(Tag::IDENTIFIER), lexeme(s) {;}
  virtual string to_string() const;
  string lexeme;
};

struct Word: Token {
  Word(const int t, const string s): Token(t), lexeme(s) {;}
  virtual string to_string() const;
  string lexeme;
};

struct Punctuator: Token {
  Punctuator(const int tag, const string s): Token(tag), punc(s) {;}
  virtual string to_string() const;
  string punc;
};

typedef shared_ptr<Token> TokenPtr;
class Lexer {
public:
  Lexer();
  Lexer(const string& input);

  auto set_input(const string& input) -> void {
    fin.open(input);
  }
  auto getNextToken() -> TokenPtr;

  auto isValidIdChar(const char c) -> bool {
    return isalpha(c) or isdigit(c) or c == '_';
  }

private:
  auto init_reserved() -> void;
  auto reserve(Tag t, string word) -> void;
  auto init_punctuators() -> void;
  auto add_punctuator(Tag t, string punc) -> void;
  static unordered_map<string, shared_ptr<Word>> reserved_keywords;
  static unordered_map<Tag, shared_ptr<Punctuator>, std::hash<int>> punctuators;

private:
  ifstream fin;
  unsigned line;
};
#endif /* ifndef LEXER_H */
