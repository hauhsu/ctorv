#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Lexer.hpp"

using namespace std;

// Token
string tagstr(Tag tag) {
  switch (tag) {
    case NUMBER:
      return "NUMBER";
    case IDENTIFIER:
      return "IDENTIFIER";
    case TRUE:
      return "TRUE";
    case FALSE:
      return "FALSE";
    case RETURN:
      return "RETUREN";
    case IF:
      return "IF";
    case ELSE:
      return "ELSE";
    case TYPE:
      return "TYPE";
    case LEFT_BRACE:
      return "{";
    case RIGHT_BRACE:
      return "}";
    case LEFT_PAREN:
      return "(";
    case RIGHT_PAREN:
      return ")";
    case ASSIGN:
      return "=";
    case EQ:
      return "==";
    case BITWISE_OR:
      return "||";
    case LOGIC_OR:
      return "|";
    case SEMICOLON:
      return ";";
    case MINUS:
      return "-";
    case PLUS:
      return "+";
    case ASTERISK:
      return "*";
    case COMMA:
      return ",";
    default:
      cerr << "Unknown tag: " << tag << endl;
      exit(1);
  }
}


ostream &operator<< (ostream &os, const Token& token) {
  os << token.repr();
  return os;
}

string Token::repr() const {
  return "Tag: " + tagstr(tag) + " Line: " + to_string(line);
}
string Token::str() const {
  return  tagstr(tag);
}

string Num::repr() const {
  return "Number: " + to_string(val) + " Tag: " + tagstr(tag);
}

string Num::str() const {
  return to_string(val);
}

string Id::repr() const {
  return "Identifier: '" + lexeme + "'" + " Tag: " + tagstr(tag);
}

string Id::str() const {
  return lexeme;
}

string Word::repr() const {
  return "Keyword: '" + lexeme + "'" + " Tag: " + tagstr(tag);
}

string Word::str() const {
  return lexeme;
}

string Punctuator::repr() const {
  return "Punctuator: '" + punc + "'" + " Tag: " + tagstr(tag);
}

string Punctuator::str() const {
  return punc;
}

// Lexer

Lexer::Lexer(): line(1) {
  init_reserved();
  init_punctuators();
}

unordered_map<string, shared_ptr<Word>> Lexer::reserved_keywords;
unordered_map<string, shared_ptr<Punctuator>> Lexer::punctuators;


auto Lexer::getNextToken() -> TokenPtr {
  char peek = ins->peek();
  for (;peek != EOF;peek = ins->peek()) {
    if (peek == '\n') {
      line++;
      /* if (_DEBUG) { */
      /*   cout << "Reading line " << line << endl; */
      /* } */
      ins->get(); // swallow newline and continue
      continue;
    }
    else if (isspace(peek)) {
      ins->get(); // swallow space and continue
      continue;
    }

    // Parse number
    if (isdigit(peek)) {
      int num = 0;
      do {
        num = num * 10 + (peek - '0');
        ins->get(); // swallow it
        peek = ins->peek();
      } while (isdigit(peek));
      return make_shared<Num>(num, line);
    }

    // Parse identifyer
    if (isalpha(peek) or peek == '_') {
      string id = "";
      do {
        id += peek;
        ins->get(); // swallow it
        peek = ins->peek();
      } while (isValidIdChar(peek));

      auto reserved = reserved_keywords.find(id);
      if ( reserved == reserved_keywords.end()) {
        return make_shared<Id>(id, line);
      } else {
        reserved->second->line = line;
        return reserved->second;
      }
    }

    // Parse punctuators
    string punc_str(1, peek);
    ins->get();
    switch (punc_str[0]) {
      case '=':
        if (ins->peek() == '=') {
          punc_str = "==";
          ins->get();
        }
        break;
      case '|':
        if (ins->peek() == '|') {
          punc_str = "||";
          ins->get();
        }
        break;
    }
    auto punc = punctuators.find(punc_str);
    if (punc != punctuators.end()) {
      punc->second->line = line;
      return punc->second;
    } else {
      cout << "Lexer: Cannot parse '" << punc_str << "' (line " << line << ")" << endl;
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
  reserve(Tag::IF, "if");
  reserve(Tag::ELSE, "else");
}

auto Lexer::reserve(Tag t, string word) -> void {
  reserved_keywords.insert({word, make_shared<Word>(t, word)});
}

auto Lexer::init_punctuators() -> void {
  add_punctuator(Tag::LEFT_PAREN, "(");
  add_punctuator(Tag::RIGHT_PAREN, ")");
  add_punctuator(Tag::LEFT_BRACE, "{");
  add_punctuator(Tag::RIGHT_BRACE, "}");
  add_punctuator(Tag::EQ, "==");
  add_punctuator(Tag::ASSIGN, "=");
  add_punctuator(Tag::BITWISE_OR, "|");
  add_punctuator(Tag::LOGIC_OR, "||");
  add_punctuator(Tag::SEMICOLON, ";");
  add_punctuator(Tag::PLUS, "+");
  add_punctuator(Tag::MINUS, "-");
  add_punctuator(Tag::ASTERISK, "*");
  add_punctuator(Tag::COMMA, ",");
}

auto Lexer::add_punctuator(Tag t, string punc) -> void {
  punctuators.insert({punc, make_shared<Punctuator>(t, punc)});
}

