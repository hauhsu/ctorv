#include "Parser.hpp"


auto Parser::unaryOp() -> Addr {
  auto op = advance();
  auto rhs = parsePrecedence(PREC_UNARY);
  Addr res;
  switch (op->tag) {
    case MINUS:
      res = cu->newTemp();
      cu->emitIR(OP_SUB, res, 0, rhs);
      break;
    case PLUS:
    case LEFT_PAREN:
      res = rhs; // do nothing
      break;
    default:
      error("Invalid unary operator: " + tagstr(op->tag));
      exit(1);
  }
  return res;
}

auto Parser::binaryOp(Addr lhs) -> Addr {
  auto op = advance();
  auto prec = getOpRule(op->tag).precedence;
  auto rhs = parsePrecedence(prec + 1);
  auto dst = cu->newTemp();
  switch (op->tag) {
    case PLUS:
      //cout << dst << " = " << l << " + " << r << endl;
      cu->emitIR(OP_ADD, dst, lhs, rhs);
      break;
    case MINUS:
      //cout << dst << " = " << l << " - " << r << endl;
      cu->emitIR(OP_SUB, dst, lhs, rhs);
      break;
    case ASTERISK:
      //cout << dst << " = " << l << " * " << r << endl;
      cu->emitIR(OP_MUL, dst, lhs, rhs);
      break;
    case EQ:
      cu->emitIR(OP_EQ, dst, lhs, rhs);
      break;
    default:
      error("Invalid binary operator: " + tagstr(op->tag));
  }
  return dst;
}

auto Parser::groupOp() -> Addr {
  auto res = parseExpr();
  match(RIGHT_PAREN);
  return res;
}

auto Parser::callOp(Addr funcName) -> Addr {
  match(LEFT_PAREN);
  vector<Addr> params;
  if (lookahead->tag != RIGHT_PAREN) {
    do {
      params.push_back(parseExpr());
    } while (lookahead->tag == COMMA && match(COMMA));
    match(RIGHT_PAREN);
  }

  auto funcSignature = cu->functions.find(funcName);
  if (funcSignature == cu->functions.end()) {
    error("Implicit declaration of function: " + funcName);
  }
  for (auto &p: params) {
    cu->emitIR(OP_PARAM, p, "", "");
  }
  cu->emitIR(OP_CALL, funcName, "", "");

  return "";
}


Parser::Parser() {
  //              token         precedence    prefix op               infix op
  opRules.insert({SEMICOLON,   {PREC_NONE,    nullptr,                nullptr}});
  opRules.insert({LEFT_PAREN,  {PREC_CALL,    &Parser::groupOp,       &Parser::callOp}});
  opRules.insert({RIGHT_PAREN, {PREC_NONE,    nullptr,                nullptr}});
  opRules.insert({ASTERISK,    {PREC_FACTOR,  nullptr,                &Parser::binaryOp}});
  opRules.insert({PLUS,        {PREC_TERM,    &Parser::unaryOp,       &Parser::binaryOp}});
  opRules.insert({MINUS,       {PREC_TERM,    &Parser::unaryOp,       &Parser::binaryOp}});
  opRules.insert({EQ,          {PREC_EQ,      nullptr,                &Parser::binaryOp}});
  opRules.insert({NUMBER,      {PREC_NONE,    nullptr,                nullptr}});
  opRules.insert({IDENTIFIER,  {PREC_NONE,    nullptr,                nullptr}});
  opRules.insert({EQ,          {PREC_EQ,      nullptr,                &Parser::binaryOp}});
  opRules.insert({RETURN,      {PREC_NONE,    nullptr,                nullptr}});
  opRules.insert({RIGHT_BRACE, {PREC_NONE,    nullptr,                nullptr}});
  opRules.insert({LEFT_BRACE,  {PREC_NONE,    nullptr,                nullptr}});
  curBlock = nullptr;
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
    //cout << lookahead->repr() << endl;
    if (lookahead->tag == Tag::TYPE) {
      parseDecl();
    }
  }
  cu->dumpIRs();
  return cu;
}

auto Parser::parseDecl() -> void {
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
    if (curBlock == nullptr) {
      // global
      cu->symbolTables[0]->add(Variable(type->lexeme, id->lexeme));
    } else {
      curBlock->symtable->add(Variable(type->lexeme, id->lexeme));
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

    auto f = make_shared<Function>(type->lexeme, id->lexeme, params);
    cu->addFunc(f);

    if (lookahead->tag == Tag::SEMICOLON) {
      match(Tag::SEMICOLON);
      cout << "Parsed function declaration: " << endl;
      cout << "  " << *f << endl;
      return;
    }
    if (lookahead->tag == Tag::LEFT_BRACE) {
      cout << "Parsed function definition: " << endl;
      cout << "  " << *f << endl;

      auto funcLabel = cu->newLabel(f->name);
      f->body = parseBlock();
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
    auto type = match(Tag::TYPE);
    auto id = match(Tag::IDENTIFIER);
    params.push_back(Variable(type->str(), id->str()));
    if (lookahead->tag == Tag::COMMA) {
      match(Tag::COMMA);
      auto tmp = parseParams();
      params.insert(params.end(), tmp.begin(), tmp.end());
    }
  }
  return params;
}

auto Parser::parseBlock() -> shared_ptr<Block> {
  match(Tag::LEFT_BRACE);
  auto blk = make_shared<Block>();
  blk->parent = curBlock;
  curBlock = blk;

  while (lookahead->tag != Tag::RIGHT_BRACE) {
    parseStatm();
  }
  curBlock = blk->parent;
  match(Tag::RIGHT_BRACE);
  return blk;
}

auto Parser::parseStatm() -> void {
  switch (lookahead->tag) {
    case Tag::IF:
      parseIf();
      break;
    case Tag::TYPE:
      parseDecl();
      break;
    case Tag::RETURN:
      parseReturn();
      break;
    case Tag::LEFT_BRACE:
      parseBlock();
      break;
    default:
      parseExprStatm();
  }
}

auto Parser::parseIf() -> void {
    match(Tag::IF);
    match(Tag::LEFT_PAREN);  // (
    auto result = parseExpr();
    match(Tag::RIGHT_PAREN); // )
    //jump over "if block" if condition evaluates 0 (false)
    auto ifBranchIR = cu->emitIR(OP_BEQ, result, "0", "patchme");
    parseStatm();

    auto jumpBeforeElseStart = cu->emitJump("patchme"); // 'if block' should jump over 'else block'.
    if (lookahead->tag == Tag::ELSE) {
      match(Tag::ELSE);
      auto elseStart = cu->newLabel();
      cu->patchBranch(ifBranchIR, elseStart);
      parseStatm();
    }
    //until new we know where is the end of "if", backpatching.
    auto labelAfterElse = cu->newLabel();
    cu->patchJump(jumpBeforeElseStart, labelAfterElse);

}

auto Parser::parsePrecedence(int precedence) -> Addr {

  // Handle prefix
  auto prefixFunc = getOpRule(lookahead->tag).prefixFunc;
  if (prefixFunc) prefixFunc(*this);

  // Handle Infix
  auto lhs = advance()->str();
  while (getOpRule(lookahead->tag).precedence >= precedence) {
    auto rule = getOpRule(lookahead->tag);
    lhs = rule.infixFunc(*this, lhs);
  }
  return lhs;
}

auto Parser::parseExpr() -> Addr {
  return parsePrecedence(PREC_ASSIGNMENT);
}

auto Parser::parseExprStatm() -> Addr {
  Addr result = parseExpr();
  match(Tag::SEMICOLON, "Expect ';' after expression.");
  return result;
}

auto Parser::parseReturn() -> void {
  match(Tag::RETURN);
  auto retAddr = parseExprStatm();
  cu->emitReturn(retAddr);
}


/*
 * Auxilary methods
 *
 */

auto Parser::match(Tag t, string msg) -> TokenPtr {
  if (lookahead->tag == t) {
    return advance();
  }
  if (msg == "")
    msg = "Syntax error: expecting '" + tagstr(t) +
          "', got '" + tagstr(lookahead->tag) + "'" + " (line " +
          to_string(lookahead->line) + ")";

  cout << msg << endl;
  exit(1);
}


// Get next token without check (shortcut of `match(lookahead->tag)`)
auto Parser::advance() -> TokenPtr {
    cout << "Read token " << lookahead->str() << endl;
    auto prev = lookahead;
    lookahead = lexer.getNextToken();
    return prev;
}

auto Parser::getOpRule(Tag opTag) -> OpRule {
    auto rule = opRules.find(opTag);
    if ( rule == opRules.end()) {
      error("Invalid operator: '" + tagstr(opTag) + "'");
    }
    return rule->second;
}


auto Parser::error(string msg) -> void {
  cerr << msg << endl;
  exit(1);
}
