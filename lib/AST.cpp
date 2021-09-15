#include "AST.hpp"

Function::Function(const string& t, const string& n, const Params& p):
  returnType(t),  name(n), params(p)
{
  ;
}

ostream &operator<< (ostream &os, const Variable& var) {
  os << var.typeLexeme << " " << var.name;
  return os;
}

ostream &operator<< (ostream & os, const Function & f) {
  os << f.returnType << " " << f.name << "(";
  printVector(f.params);
  os << ")";
  return os;
}

