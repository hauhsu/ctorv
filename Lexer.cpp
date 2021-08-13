#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Lexer.hpp"

using namespace std;

// Token

ostream &operator<< (ostream &os, const Token& token) {
  os << token.to_string();
  return os;
}

string Token::to_string() const {
  return "Tag:" + ::to_string(tag);
}

string Num::to_string() const {
  return "Number: " + ::to_string(val) + " Tag: " + ::to_string(tag);
}

string Id::to_string() const {
  return "Identifier: '" + lexeme + "'" + " Tag:" + ::to_string(tag);
}

string Word::to_string() const {
  return "Keyword: '" + lexeme + "'" + " Tag:" + ::to_string(tag);
}

string Punctuator::to_string() const {
  return "Punctuator: '" + punc + "'" + " Tag:" + ::to_string(tag);
}

// Lexer

Lexer::Lexer(): line(1) {
  init_reserved();
  init_punctuators();
}

Lexer::Lexer(const string& input): fin(input), line(1) {
  init_reserved();
  init_punctuators();
}

unordered_map<string, shared_ptr<Word>> Lexer::reserved_keywords;
unordered_map<Tag, shared_ptr<Punctuator>, std::hash<int>> Lexer::punctuators;


auto Lexer::getNextToken() -> TokenPtr {
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
        break;
      case ')':
        t = Tag::RIGHT_PAREN;
        break;
      case '{':
        t = Tag::LEFT_CURRY;
        break;
      case '}':
        t =  Tag::RIGHT_CURRY;
        break;
      case ';':
        t = Tag::SEMICOLON;
        break;
      case '+':
        t = Tag::PLUS;
        break;
      case '-':
        t = Tag::MINUS;
        break;
      case ',':
        t = Tag::COMMA;
        break;
      case '=':
        if (fin.peek() == '=') {
          fin.get();
          t = Tag::EQ;
        }
        else
          t =  Tag::ASSIGN;
        break;
      case '|':
        if (fin.peek() == '|') {
          fin.get();
          t = Tag::LOGIC_OR;
        }
        else
          t =  Tag::BITWISE_OR;
        break;
    }
    fin.get(); // swallow
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

auto Lexer::init_reserved() -> void {
  reserve(Tag::TRUE, "true");
  reserve(Tag::FALSE, "false");
  reserve(Tag::RETURN, "return");
  reserve(Tag::TYPE, "int");
}

auto Lexer::reserve(Tag t, string word) -> void {
  reserved_keywords.insert({word, make_shared<Word>(t, word)});
}

auto Lexer::init_punctuators() -> void {
  add_punctuator(Tag::LEFT_PAREN, "(");
  add_punctuator(Tag::RIGHT_PAREN, ")");
  add_punctuator(Tag::LEFT_CURRY, "{");
  add_punctuator(Tag::RIGHT_CURRY, "}");
  add_punctuator(Tag::EQ, "==");
  add_punctuator(Tag::ASSIGN, "=");
  add_punctuator(Tag::BITWISE_OR, "|");
  add_punctuator(Tag::LOGIC_OR, "||");
  add_punctuator(Tag::SEMICOLON, ";");
  add_punctuator(Tag::PLUS, "+");
  add_punctuator(Tag::MINUS, "-");
  add_punctuator(Tag::COMMA, ",");
}

auto Lexer::add_punctuator(Tag t, string punc) -> void {
  punctuators.insert({t, make_shared<Punctuator>(t, punc)});
}

