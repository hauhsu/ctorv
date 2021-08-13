#include "Parser.hpp"


//Recursive decent parser
auto Parser::parse(const string& input) -> void {
  lexer.set_input(input);
  while(1) {
    lookahead = lexer.getNextToken();
    if (!lookahead) break;
    if (_DEBUG) {
      cout << lookahead->to_string() << endl;
    }
    if (lookahead->tag == Tag::TYPE) {
      parseDecl();
    }
  }
}

ostream &operator<< (ostream &os, const Variable& var) {
  os << "\"" << var.name.lexeme << "\": " << var.type.lexeme;
  return os;
}

template<typename T>
void printVector(vector<T>& v) {
  for (auto &c: v) {
    cout << c << endl;
  }
}

auto Parser::parseDecl() -> void {
  /*
   * grammer:
   *     declaration -> TYPE ID '(' parameters ')';  //function definition
   *                 |  TYPE ID '(' parameters ')' block  //function declaration
   *                 |  TYPE ID; // variable
   */
  auto type = match(Tag::TYPE);
  auto id = match(Tag::IDENTIFIER);
  if (lookahead->tag == Tag::SEMICOLON) {
    cout << "Parsed variable declaration: " << endl;
    cout << "type: " << dynamic_pointer_cast<Word>(type)->lexeme << endl;
    cout << "name: " << dynamic_pointer_cast<Id>(id)->lexeme << endl;
  } else if (lookahead->tag == Tag::LEFT_PAREN) {
    cout << "Parsed function declaration: " << endl;
    match(Tag::LEFT_PAREN);
    auto params = parseParams();
    printVector(params);
    match(Tag::RIGHT_PAREN);
  }
}

auto Parser::parseParams() -> Params {
  /*
   * grammer:
   *     params -> params ',' param | param | epsilon
   *     param  -> TYPE ID
   */
  Params params;
  if (lookahead->tag == Tag::TYPE) {
    auto type = dynamic_pointer_cast<Word>(match(Tag::TYPE));
    auto id = dynamic_pointer_cast<Id>(match(Tag::IDENTIFIER));
    params.push_back(Variable(*id, *type));
    if (lookahead->tag == Tag::COMMA) {
      match(Tag::COMMA);
      auto tmp = parseParams();
      params.insert(params.end(), tmp.begin(), tmp.end());
    }
  }
  return params;
}

auto Parser::match(Tag t) -> TokenPtr {
  if (lookahead->tag == t) {
    auto prevLookahead = lookahead;
    lookahead = lexer.getNextToken();
    return prevLookahead;
  }

  cout << "Syntax error";
  exit(1);
}

