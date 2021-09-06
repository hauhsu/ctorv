#include "Parser.hpp"

shared_ptr<IR> IR_nop() {
  return mkIR(OP_NOP, "0", "0", "0");
}

auto unaryOp(Addr dst, Addr addr) -> void {
}

auto binaryOp(Addr dst, Addr l, Addr r) -> void {

}

Parser::Parser(): tmpCnt(0) {
  opRules.insert({PLUS,  {PREC_TERM, unaryOp, binaryOp}});
  opRules.insert({MINUS, {PREC_TERM, unaryOp, binaryOp}});
}

auto Parser::parse(ifstream& input) -> shared_ptr<CompileUnit> {
  lexer.setInput(input);
  return parse();
}

auto Parser::parse(stringstream& input) -> shared_ptr<CompileUnit> {
  lexer.setInput(input);
  return parse();
}

//Recursive decent parser
auto Parser::parse() -> shared_ptr<CompileUnit> {
  cu = make_shared<CompileUnit>();
  while(1) {
    lookahead = lexer.getNextToken();
    if (!lookahead) break;
    cout << lookahead->repr() << endl;
    if (lookahead->tag == Tag::TYPE) {
      parseDecl(nullptr);
    }
  }
  return cu;
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

auto Parser::parseDecl(shared_ptr<BlockNode> curBlk) -> void {
  /*
   * grammer:
   *     declaration -> TYPE ID '(' parameters ')';  //function definition
   *                 |  TYPE ID '(' parameters ')' block  //function declaration
   *                 |  TYPE ID;  //variable
   */
  auto type = dynamic_pointer_cast<Word>(match(Tag::TYPE));
  auto id = dynamic_pointer_cast<Id>(match(Tag::IDENTIFIER));
  if (lookahead->tag == Tag::SEMICOLON) {
    cout << "Parsed variable declaration: " << endl;
    cout << "  type: " << type->lexeme << endl;
    cout << "  name: " << id->lexeme << endl;
    if (curBlk == nullptr) {
      // global
      cu->symbolTables[0]->add(Variable(type->lexeme, id->lexeme));
    } else {
      curBlk->symtable->add(Variable(type->lexeme, id->lexeme));
    }
    match(Tag::SEMICOLON);
    return;
  }

  if (lookahead->tag == Tag::ASSIGN) {
    match(Tag::ASSIGN);
    TokenPtr initVal;

    if (type->lexeme == "int") {
      initVal = match(Tag::NUMBER);
    }
    cout << "Parsed variable declaration: " << endl;
    cout << "  type: " << type->lexeme << endl;
    cout << "  name: " << id->lexeme << endl;
    cout << "  init: " << initVal->repr() << endl;
    match(Tag::SEMICOLON);
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
      cout << "  name: " << f->name << endl;
      cout << "  parameters: " << endl;
      printVector(f->params);
      return;
    }
    if (lookahead->tag == Tag::LEFT_BRACE) {
      f->body = parseBlock(curBlk);
      cout << "Parsed function definition: " << endl;
      cout << "  name: " << f->name << endl;
      cout << "  parameters: " << endl;
      printVector(f->params);
      return;
    }
    cout << "Syntax error." << endl;
    exit(1);
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

auto Parser::parseBlock(shared_ptr<BlockNode> parent=nullptr) -> shared_ptr<BlockNode> {
  auto blk = make_shared<BlockNode>();
  blk->parent = parent;

  parseExpr();
  return blk;
}


auto Parser::parseRHS(Addr lhs, int precedence) -> Addr {
  while (true) {
    auto rule = opRules.find(lookahead->tag);
    if (rule == opRules.end()) {
      cerr << "Invalid operator: " << lookahead << endl;
    }
    if (precedence > rule->second.precedence) {
      return lhs;
    }

    auto tmp = mkTemp();
    rule->second.infixFunc(tmp, rhs, lhs);
    auto rhs = parseRHS(lookahead->str(), rule->second.precedence+1);
}
}

/*
 *
 */
auto Parser::parseExpr() -> shared_ptr<ASTNode> {
  if (lookahead->tag == NUMBER) {
    auto lhs = dynamic_pointer_cast<Num>(match(lookahead->tag));
    parseRHS(repr(lhs->val), PREC_ASSIGNMENT);
  }
  else if (lookahead->tag == IDENTIFIER) {
    auto lhs = dynamic_pointer_cast<Id>(match(lookahead->tag));
    parseRHS(lhs->lexeme, PREC_ASSIGNMENT);
  }


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

  cout << "Syntax error: expecting " << t << endl;
  exit(1);
}


auto Parser::addFunc(shared_ptr<FunctionNode> f) -> void {
  assert(cu);
  cu->functions.insert({f->name, f});
}
