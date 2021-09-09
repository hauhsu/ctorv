#include "Parser.hpp"

auto emptyPrefixOp(Tag, Addr, Addr) -> void {
  cerr << "Shouldn't be called." << endl;
  exit(1);
}

auto emptyInfixOp(Tag, Addr, Addr, Addr) -> void {
  cerr << "Shouldn't be called." << endl;
  exit(1);
}

auto Parser::unaryOp(Tag tag, Addr dst, Addr addr) -> void {
  switch (tag) {
    case PLUS:
      cout << dst << " = " << "+" << addr << endl;
      break;
    default:
      cerr  << "Invalid Op"<< endl;
  }
}

auto Parser::binaryOp(Tag tag, Addr dst, Addr l, Addr r) -> void {
  switch (tag) {
    case PLUS:
      cout << dst << " = " << l << " + " << r << endl;
      cu->emitIR(OP_ADD, dst, l, r);
      break;
    case ASTERISK:
      cout << dst << " = " << l << " * " << r << endl;
      cu->emitIR(OP_MUL, dst, l, r);
      break;
    default:
      cerr  << "Invalid Op"<< endl;
  }
}


Parser::Parser() {
  //              token         precedence    prefix op      infix op
  opRules.insert({SEMICOLON,   {PREC_NONE,    &Parser::emptyPrefixOp, &Parser::emptyInfixOp}});
  opRules.insert({LEFT_PAREN,  {PREC_CALL,    &Parser::unaryOp,       &Parser::emptyInfixOp}});
  opRules.insert({RIGHT_PAREN, {PREC_NONE,    &Parser::emptyPrefixOp, &Parser::emptyInfixOp}});
  opRules.insert({ASTERISK,    {PREC_FACTOR,  &Parser::emptyPrefixOp, &Parser::binaryOp}});
  opRules.insert({PLUS,        {PREC_TERM,    &Parser::unaryOp,       &Parser::binaryOp}});
  opRules.insert({MINUS,       {PREC_TERM,    &Parser::unaryOp,       &Parser::binaryOp}});
  opRules.insert({EQ,          {PREC_EQ,      &Parser::emptyPrefixOp, &Parser::binaryOp}});
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
  cu->dumpIRs();
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
      match(Tag::SEMICOLON);
      cout << "Parsed function declaration: " << endl;
      cout << "  name: " << f->name << endl;
      cout << "  parameters: " << endl;
      printVector(f->params);
      return;
    }
    if (lookahead->tag == Tag::LEFT_BRACE) {
      match(Tag::LEFT_BRACE);
      cout << "Parsed function definition: " << endl;
      cout << "  name: " << f->name << endl;
      cout << "  parameters: " << endl;
      printVector(f->params);
      f->body = parseBlock(curBlk);
      match(Tag::RIGHT_BRACE);
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

  while (lookahead->tag != Tag::RIGHT_BRACE) {
    parseStatm();
  }
  return blk;
}

auto Parser::parseStatm() -> void {
  if (lookahead->tag == Tag::IF) {
    cout << "Parsing if" << endl;
    parseIf();
  }

  else {
    cout << "Parsing expr" << endl;
    parseExprStatm();
  }
}

auto Parser::parseIf() -> void {
    match(Tag::IF);
    match(Tag::LEFT_PAREN);  // (
    auto result = parseExpr();
    match(Tag::RIGHT_PAREN); // )
    match(Tag::LEFT_BRACE);  // {
    auto ifBranchIR = cu->emitIR(OP_BEQ, result, "0", "patchme"); //jump over if expression evaluates 0 (false)
    parseStatm();
    match(Tag::RIGHT_BRACE);  // }
    auto afterIfLabel = cu->newLabel();
    patchJump(ifBranchIR, afterIfLabel);
}

auto Parser::parseRHS(Addr lhs, int precedence) -> Addr {
  //Handle Infix
  while (true) {
    auto op = lookahead;
    cout << op->repr() << endl;
    auto rule = getOpRule(op);
    Addr rhs;
    if (precedence >= rule.precedence) {
      return lhs;
    } else {
      match(lookahead->tag); // swallow op
      rhs = parseRHS(match(lookahead->tag)->str(), rule.precedence+1);
    }

    auto tmp = cu->newTemp();
    rule.infixFunc(*this, op->tag, tmp, lhs, rhs);
    lhs = tmp;
  }
}

/*
 *
 */
auto Parser::parseExpr() -> Addr {
  switch (lookahead->tag) {
    case Tag::NUMBER:
    case Tag::IDENTIFIER:
      return parseRHS(match(lookahead->tag)->str(), PREC_ASSIGNMENT);

    default:
      cerr << "Unexpected token " << lookahead->repr() << " for expression" << endl;
      exit(1);
  }
}

auto Parser::parseExprStatm() -> Addr {
  Addr result = parseExpr();
  match(Tag::SEMICOLON);
  return result;
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

  cout << "Syntax error: expecting " << t << ", got " << lookahead->tag << endl;
  exit(1);
}


auto Parser::addFunc(shared_ptr<FunctionNode> f) -> void {
  assert(cu);
  cu->functions.insert({f->name, f});
}

auto Parser::getOpRule(shared_ptr<Token> op) -> OpRule {
    auto rule = opRules.find(op->tag);
    if ( rule == opRules.end()) {
      cerr << "Invalid operator: " << op->repr() << endl;
      exit(1);
    }
    return rule->second;
}

