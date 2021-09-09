#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>
#include <sstream>
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
    IF            ,
    ELSE          ,
    //   type
    TYPE          ,
    // punctuators
    LEFT_PAREN    , // (
    RIGHT_PAREN   , // )
    LEFT_BRACE    , // {
    RIGHT_BRACE   , // } 139
    ASSIGN        , // =
    EQ            , // ==
    BITWISE_OR    , // |
    LOGIC_OR      , // ||
    SEMICOLON     , // ; 144
    MINUS         , // -
    PLUS          , // +
    ASTERISK      , // *
    COMMA         , // ,
};

string tagstr(Tag);

struct Token {
  Token(Tag t, unsigned line): tag(t), line(line) {;}
  virtual ~Token(){;}
  virtual string repr() const;
  virtual string str() const;
  friend ostream &operator<< (ostream &os, const Token& token);

  Tag tag;
  unsigned line;
};

struct Num: Token {
  Num(const int v, const unsigned line): Token(Tag::NUMBER, line), val(v) {;}
  virtual string repr() const;
  virtual string str() const;
  int val;
};

struct Id: Token {
  Id(const string s, const unsigned line): Token(Tag::IDENTIFIER, line), lexeme(s) {;}
  virtual string repr() const;
  virtual string str() const;
  string lexeme;
};

struct Word: Token {
  Word(const Tag t, const string s): Token(t, 0), lexeme(s) {;}
  virtual string repr() const;
  virtual string str() const;
  string lexeme;
};

struct Punctuator: Token {
  Punctuator(const Tag tag, const string s): Token(tag, 0), punc(s) {;}
  virtual string repr() const;
  virtual string str() const;
  string punc;
};

typedef shared_ptr<Token> TokenPtr;
class Lexer {
public:
  Lexer();

  auto setInput(ifstream& input) -> void {
    ins = &input;
  }

  auto setInput(stringstream& input) -> void {
    ins = &input;
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
  static unordered_map<string, shared_ptr<Punctuator>> punctuators;

private:
  istream *ins;  // input stream
  unsigned line;
};
#endif /* ifndef LEXER_H */
