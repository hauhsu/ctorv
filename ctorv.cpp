#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

//bool _DEBUG = true;
bool _DEBUG = false;

enum Tag {
  // Other ASCII code occupies 0 ~ 127
    NUM        = 128,
    ID         ,
    TRUE       ,
    FALSE      ,
    PUNCTUATOR ,
    RETURN     ,
    TYPE
};


struct Token {
  const int tag;

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
  Num(const int v): Token(Tag::NUM), val(v) {;}
  const int val;
  virtual string to_string() const {
    return "Tag:" + ::to_string(tag) + " Val: " + ::to_string(val);
  }
};

struct Id: Token {
  Id(const string s): Token(Tag::ID), lexeme(s) {;}
  const string lexeme;
  virtual string to_string() const {
    return "Tag:" + ::to_string(tag) + " Lexeme: " + lexeme;
  }
};

struct Word: Token {
  Word(const int t, const string s): Token(t), lexeme(s) {;}
  const string lexeme;
  virtual string to_string() const {
    return "Tag:" + ::to_string(tag) + " Lexeme: " + lexeme;
  }
};

struct Punctuator: Token {
  Punctuator(const string s): Token(Tag::PUNCTUATOR), lexeme(s) {;}
  const string lexeme;
  virtual string to_string() const {
    return "Tag:" + ::to_string(tag) + " Lexeme: " + lexeme;
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
        if (_DEBUG) {
          cout << "Reading line " << line << endl;
        }
        line++;
        fin.get(); // swallow newline and continue
      }
      else if (isspace(peek)) {
        fin.get(); // swallow space and continue
        continue;
      }

      if (isdigit(peek)) {
        int num = 0;
        do {
          num = num * 10 + (peek - '0');
          fin.get(); // swallow it
          peek = fin.peek();
        } while (isdigit(peek));
        return TokenPtr(new Num(num));
      }

      if (isalpha(peek) or peek == '_') {
        string id = "";
        do {
          id += peek;
          fin.get(); // swallow it
          peek = fin.peek();
        } while (isValidIdChar(peek));

        auto reserved = reserved_keywords.find(id);
        if ( reserved == reserved_keywords.end()) {
          return TokenPtr(new Id(id));
        } else {
          return TokenPtr(reserved->second);
        }
        return TokenPtr(new Id(id));
      }

      if (punctuators.find(peek) != string::npos) {
        fin.get();
        return TokenPtr(new Punctuator(string({peek})));
      }
    }

    return nullptr;
  }

  auto isValidIdChar(const char c) -> bool {
    return isalpha(c) or isdigit(c) or c == '_';
  }
private:
  auto reserve(WordPtr t) -> void {
    reserved_keywords.insert({t->lexeme, t});
  }
  auto init_reserved() -> void {
    reserve(WordPtr(new Word(Tag::TRUE, "true")));
    reserve(WordPtr(new Word(Tag::FALSE, "false")));
    reserve(WordPtr(new Word(Tag::RETURN, "return")));
    reserve(WordPtr(new Word(Tag::TYPE, "int")));
  }
  auto init_punctuators() -> void {
    punctuators = "()[]{}.+-*%/~!=&|;:?";
  }

private:
  ifstream fin;
  unsigned line;
  unordered_map<string, WordPtr> reserved_keywords;
  string punctuators;
};

class Parser {
};


class Compiler {
public:
  auto compile(const string& input,
               const string& output="") -> void {
    lexer = Lexer(input);
    auto t = lexer.getNextToken();
    while (t) {
      cout << t->to_string() << endl;
      t = lexer.getNextToken();
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
