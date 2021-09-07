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
  auto parseDecl(shared_ptr<BlockNode> scope) -> void;  // add to **functions** or **symTables**
  auto parseParams() -> Params;
  auto parseBlock(shared_ptr<BlockNode> parent) -> shared_ptr<BlockNode>;
  auto parseStatm() -> shared_ptr<ASTNode>;
  auto parseIfStatm() -> shared_ptr<ASTNode>;
  auto parseAssign() -> shared_ptr<ASTNode>; // =
  auto parseLogicOr() -> shared_ptr<ASTNode>;   // ||
  auto parseLogicAnd() -> shared_ptr<ASTNode>;   // &&
  auto parseEq() -> shared_ptr<ASTNode>;     // ==, !=
  auto parseRel() -> shared_ptr<ASTNode>;    // >, <, <=, >=
  auto parseExpr() -> shared_ptr<ASTNode>;   // +, -
  auto parseRHS(Addr lhs, int precedence) -> Addr;

  auto parseTerm() -> shared_ptr<ASTNode>;   // *, /
  auto parseUnary() -> shared_ptr<ASTNode>;  // -
  auto parseFactor() -> shared_ptr<ASTNode>; // ID, true, false, NUMBER
  auto mkTemp() -> string {
    return "t" + to_string(tmpCnt++);
  }


  auto match(Tag t) -> TokenPtr;
  auto addFunc(shared_ptr<FunctionNode>) -> void;

private:
  Lexer lexer;
  shared_ptr<Token> lookahead;
  shared_ptr<CompileUnit> cu;
  shared_ptr<Environment> curSymTable;
  shared_ptr<BlockNode> curBlock;

  // for Pratt expression parser
  enum Precedence {
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
  };
  typedef function<void(Tag, Addr, Addr)> PrefixFunc;
  typedef function<void(Tag, Addr, Addr, Addr)> InfixFunc;
  struct OpRule {
    Precedence precedence;
    PrefixFunc prefixFunc;
    InfixFunc infixFunc;
  };
  unordered_map<int, OpRule> opRules;
  int tmpCnt;
};
#endif /* end of include guard: PARSER_CPP_UDVXJZBP */
