#include "AST.hpp"

FunctionNode::FunctionNode(const string& t, const string& n, const Params& p):
  returnType(t),  name(n), params(p)
{
  ;
}

ostream &operator<< (ostream &os, const Variable& var) {
  os << var.typeLexeme << " " << var.name;
  return os;
}

ostream &operator<< (ostream & os, const FunctionNode & f) {
  os << f.returnType << " " << f.name << "(";
  printVector(f.params);
  os << ")";
  return os;
}

