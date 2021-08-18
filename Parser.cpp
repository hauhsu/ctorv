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
      parseDecl(nullptr);
    }
  }
}

ostream &operator<< (ostream &os, const Variable& var) {
  os <<  var.name << ":" << var.typeLexeme;
  return os;
}

template<typename T>
void printVector(vector<T>& v) {
  for (auto &c: v) {
    cout << c << endl;
  }
}

auto Parser::parseDecl(shared_ptr<BlockNode> scope) -> void {
  /*
   * grammer:
   *     declaration -> TYPE ID '(' parameters ')';  //function definition
   *                 |  TYPE ID '(' parameters ')' block  //function declaration
   *                 |  TYPE ID; // variable
   */
  auto type = dynamic_pointer_cast<Word>(match(Tag::TYPE));
  auto id = dynamic_pointer_cast<Id>(match(Tag::IDENTIFIER));
  if (lookahead->tag == Tag::SEMICOLON) {
    cout << "Parsed variable declaration: " << endl;
    cout << "type: " << type->lexeme << endl;
    cout << "name: " << id->lexeme << endl;
    if (scope == nullptr) {
      // global
      symTables[0]->add(Variable(type->lexeme, id->lexeme));
    }
    return;
  }

  if (lookahead->tag == Tag::LEFT_PAREN) {
    match(Tag::LEFT_PAREN);
    auto params = parseParams();
    match(Tag::RIGHT_PAREN);

    auto f = make_shared<FunctionNode>();
    f->name = dynamic_pointer_cast<Id>(id)->lexeme;
    f->params = params;
    addFunc(f);

    if (lookahead->tag == Tag::SEMICOLON) {
      cout << "Parsed function declaration: " << endl;
      cout << "Name: " << f->name << endl;
      cout << "Parameters: " << endl;
      printVector(f->params);
      return;
    }
    if (lookahead->tag == Tag::LEFT_BRACE) {
      cout << "Parsed function definition: " << endl;
      cout << "Name: " << f->name << endl;
      cout << "Parameters: " << endl;
      printVector(f->params);
      return;
    }
    cout << "Syntax error.";
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
    params.push_back(Variable(type->lexeme, id->lexeme));
    if (lookahead->tag == Tag::COMMA) {
      match(Tag::COMMA);
      auto tmp = parseParams();
      params.insert(params.end(), tmp.begin(), tmp.end());
    }
  }
  return params;
}

auto Parser::parseBlock(shared_ptr<BlockNode> parent=nullptr) -> BlockNode {
  auto blk = make_shared<BlockNode>();
  blk->parent = parent;

  if (lookahead->tag == Tag::IF) {
    /* auto ifblk = make_shared<IFNode>(); */
    /* if (lookahead->tag == Tag::ELSE) { */
    /* } */

  }


  return *blk;
}

auto parseStatm() -> void {

}

/*
 * Auxilary methods
 *
 */

auto Parser::match(Tag t) -> TokenPtr {
  if (lookahead->tag == t) {
    auto prevLookahead = lookahead;
    lookahead = lexer.getNextToken();
    return prevLookahead;
  }

  cout << "Syntax error";
  exit(1);
}


auto Parser::addFunc(shared_ptr<FunctionNode> f) -> void {
  functions.insert({f->name, f});
}
