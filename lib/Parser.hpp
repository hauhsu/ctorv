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
  auto parseAssign() -> shared_ptr<ASTNode>; // =
  auto parseLogicOr() -> shared_ptr<ASTNode>;   // ||
  auto parseLogicAnd() -> shared_ptr<ASTNode>;   // &&
  auto parseEq() -> shared_ptr<ASTNode>;     // ==, !=
  auto parseRel() -> shared_ptr<ASTNode>;    // >, <, <=, >=
  auto parseExpr() -> Addr;   // +, -
  auto parseExprStatm() -> Addr;
  auto parseRHS(Addr lhs, int precedence) -> Addr;
  auto parseFuncCall() -> Addr;

  auto parseTerm() -> shared_ptr<ASTNode>;   // *, /
  auto parseUnary() -> shared_ptr<ASTNode>;  // -
  auto parseFactor() -> shared_ptr<ASTNode>; // ID, true, false, NUMBER
  auto patchJump(unsigned jumpIR, Addr addr) -> void {
    cu->IRList[jumpIR]->addr2 = addr;
  }


  auto unaryOp(Tag tag, Addr dst, Addr addr) -> Addr;
  auto binaryOp(Tag tag, Addr dst, Addr l, Addr r) -> Addr;
  auto groupOp(Tag, Addr, Addr) -> Addr;
  auto callOp(Tag, Addr, Addr, Addr) -> Addr;
  auto emptyPrefixOp(Tag, Addr, Addr) -> Addr {
    cerr << "Shouldn't be called." << endl;
    exit(1);
  }

  auto emptyInfixOp(Tag, Addr, Addr, Addr) -> Addr {
    cerr << "Shouldn't be called." << endl;
    exit(1);
  }



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
  typedef function<Addr(Parser&, Tag, Addr, Addr)> PrefixFunc;
  typedef function<Addr(Parser&, Tag, Addr, Addr, Addr)> InfixFunc;
  struct OpRule {
    Precedence precedence;
    PrefixFunc prefixFunc;
    InfixFunc infixFunc;
  };
  unordered_map<int, OpRule> opRules;

 /* Auxilary methods */
  auto match(Tag t, string msg="") -> TokenPtr;
  auto getOpRule(shared_ptr<Token> op) -> OpRule;
};
#endif /* end of include guard: PARSER_CPP_UDVXJZBP */
