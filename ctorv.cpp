#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

bool _DEBUG = true;
//bool _DEBUG = false;

enum Tag {
  // Other ASCII code occupies 0 ~ 127
    NUMBER   = 128,
    IDENTIFYER    ,
    TRUE          ,
    FALSE         ,
    INT           ,
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
    RETURN        ,
};


struct Token {
  int tag;

  Token(int t): tag(t) {;}
  virtual ~Token(){;}

  virtual string to_string() const {
    return "Tag:" + ::to_string(tag);
  }
  friend ostream &operator<< (ostream &os, const Token& token);
};

ostream &operator<< (ostream &os, const Token& token) {
  os << token.to_string();
  return os;
}

struct Num: Token {
  Num(const int v): Token(Tag::NUMBER), val(v) {;}
  int val;
  virtual string to_string() const {
    return "Number: " + ::to_string(val) + " Tag: " + ::to_string(tag);
  }
};

struct Id: Token {
  Id(const string s): Token(Tag::IDENTIFYER), lexeme(s) {;}
  const string lexeme;
  virtual string to_string() const {
    return "Identifyer: '" + lexeme + "'" + " Tag:" + ::to_string(tag);
  }
};

struct Word: Token {
  Word(const int t, const string s): Token(t), lexeme(s) {;}
  string lexeme;
  virtual string to_string() const {
    return "Keyword: '" + lexeme + "'" + " Tag:" + ::to_string(tag);
  }
};

struct Punctuator: Token {
  Punctuator(const int tag, const string s): Token(tag), punc(s) {;}
  string punc;
  virtual string to_string() const {
    return "Punctuator: '" + punc + "'" + " Tag:" + ::to_string(tag);
  }
};



typedef shared_ptr<Token> TokenPtr;
typedef shared_ptr<Word> WordPtr;

class Lexer {
public:
  Lexer(): line(1) {
    init_reserved();
    init_punctuators();
  }
  Lexer(string input): fin(input), line(1) {
    init_reserved();
    init_punctuators();
  }

  auto getNextToken() -> TokenPtr {
    char peek = fin.peek();
    for (;peek != EOF;peek = fin.peek()) {
      if (peek == '\n') {
        line++;
        /* if (_DEBUG) { */
        /*   cout << "Reading line " << line << endl; */
        /* } */
        fin.get(); // swallow newline and continue
        continue;
      }
      else if (isspace(peek)) {
        fin.get(); // swallow space and continue
        continue;
      }

      // Parse number
      if (isdigit(peek)) {
        int num = 0;
        do {
          num = num * 10 + (peek - '0');
          fin.get(); // swallow it
          peek = fin.peek();
        } while (isdigit(peek));
        return make_shared<Num>(num);
      }

      // Parse identifyer
      if (isalpha(peek) or peek == '_') {
        string id = "";
        do {
          id += peek;
          fin.get(); // swallow it
          peek = fin.peek();
        } while (isValidIdChar(peek));

        auto reserved = reserved_keywords.find(id);
        if ( reserved == reserved_keywords.end()) {
          return make_shared<Id>(id);
        } else {
          return reserved->second;
        }
      }

      // Parse punctuators
      Tag t;
      switch (peek) {
        case '(':
          t = Tag::LEFT_PAREN;
          fin.get(); // swallow
          break;
        case ')':
          t = Tag::RIGHT_PAREN;
          fin.get(); // swallow
          break;
        case '{':
          t = Tag::LEFT_CURRY;
          fin.get(); // swallow
          break;
        case '}':
          t =  Tag::RIGHT_CURRY;
          fin.get(); // swallow
          break;
        case ';':
          t = Tag::SEMICOLON;
          fin.get(); // swallow
          break;
        case '+':
          t = Tag::SEMICOLON;
          fin.get(); // swallow
          break;
        case '-':
          t = Tag::SEMICOLON;
          fin.get(); // swallow
          break;
        case '=':
          fin.get();
          if (fin.peek() == '=') 
            t = Tag::EQ;
          else
            t =  Tag::RIGHT_CURRY;
          break;
        case '|':
          fin.get();
          if (fin.peek() == '|') 
            t = Tag::EQ;
          else
            t =  Tag::RIGHT_CURRY;
          break;
      }
      auto punc = punctuators.find(t);
      if (punc != punctuators.end()) {
        return punc->second;
      } else {
        cout << "Cannot parse '" << peek << "' (line " << line << ")" << endl;
        exit(1);
      }
    }
    
    return nullptr;
  }

  auto isValidIdChar(const char c) -> bool {
    return isalpha(c) or isdigit(c) or c == '_';
  }
private:
  auto init_reserved() -> void {
    reserve(Tag::TRUE, "true");
    reserve(Tag::FALSE, "false");
    reserve(Tag::RETURN, "return");
    reserve(Tag::INT, "int");
  }

  auto reserve(Tag t, string word) -> void {
    reserved_keywords.insert({word, make_shared<Word>(t, word)});
  }

  auto init_punctuators() -> void {
    add_punctuator(Tag::LEFT_PAREN, "(");
    add_punctuator(Tag::RIGHT_PAREN, ")");
    add_punctuator(Tag::LEFT_CURRY, "{");
    add_punctuator(Tag::RIGHT_CURRY, "}");
    add_punctuator(Tag::EQ, "==");
    add_punctuator(Tag::ASSIGN, "=");
    add_punctuator(Tag::BITWISE_OR, "|");
    add_punctuator(Tag::LOGIC_OR, "||");
    add_punctuator(Tag::SEMICOLON, ";");
    add_punctuator(Tag::MINUS, "-");
  }

  auto add_punctuator(Tag t, string punc) -> void {
    punctuators.insert({t, make_shared<Punctuator>(t, punc)});
  }

  static unordered_map<Tag, shared_ptr<Punctuator>, std::hash<int>> punctuators;
private:
  ifstream fin;
  unsigned line;
  unordered_map<string, shared_ptr<Word>> reserved_keywords;
};

unordered_map<Tag, shared_ptr<Punctuator>, std::hash<int>> Lexer::punctuators;

class Parser {
};


class Compiler {
public:
  auto compile(const string& input,
               const string& output="") -> void {
    lexer = Lexer(input);
    while(1) {
      shared_ptr<Token> t = lexer.getNextToken();
      if (!t) break;
      if (_DEBUG) {
        cout << t->to_string() << endl;
      }
    }
  }
private:
  Lexer lexer;
  Parser parser;
};

int main(int argc, char *argv[])
{
  auto compiler = Compiler();
  for (int i = 1; i < argc; i++) {
    compiler.compile(argv[i]);
  }

  return 0;
}
