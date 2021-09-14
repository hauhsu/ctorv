#ifndef PARSER_CPP_UDVXJZBP
#define PARSER_CPP_UDVXJZBP

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "AST.hpp"
#include "CompileUnit.hpp"
using namespace std;

extern bool _DEBUG;


// Recursive decent parser
class Parser {
public:

  Parser();
  auto parse(stringstream& input) -> shared_ptr<CompileUnit>;
  auto parse(ifstream& input) -> shared_ptr<CompileUnit>;


private:
  auto parse() -> shared_ptr<CompileUnit>;
  auto parseDecl() -> void;  // add to **functions** or **symTables**
  auto parseParams() -> Params;
  auto parseBlock() -> shared_ptr<BlockNode>;
  auto parseStatm() -> void;
  auto parseIf() -> void;
  auto parseExpr() -> Addr;   // +, -
  auto parseExprStatm() -> Addr;
  auto parsePrecedence(int precedence) -> Addr;
  auto parseFuncCall() -> Addr;

  auto patchJump(unsigned jumpIR, Addr addr) -> void {
    cu->IRList[jumpIR]->addr2 = addr;
  }


  auto unaryOp() -> Addr;
  auto binaryOp(Addr) -> Addr;
  auto groupOp() -> Addr;
  auto callOp(Addr) -> Addr;



private:
  Lexer lexer;
  shared_ptr<Token> lookahead;
  shared_ptr<CompileUnit> cu;
  shared_ptr<Environment> curSymTable;
  shared_ptr<BlockNode> curBlock;

  // for Pratt expression parser
  enum Precedence {
    PREC_NONE,        
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQ,          // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
  };
  typedef function<Addr(Parser&)> PrefixFunc;
  typedef function<Addr(Parser&, Addr lhs)> InfixFunc;
  struct OpRule {
    Precedence precedence;
    PrefixFunc prefixFunc;
    InfixFunc infixFunc;
  };
  unordered_map<int, OpRule> opRules;

 /* Auxilary methods */
  auto match(Tag t, string msg="") -> TokenPtr;
  auto advance() -> TokenPtr;
  auto getOpRule(Tag opTag) -> OpRule;
};
#endif /* end of include guard: PARSER_CPP_UDVXJZBP */
