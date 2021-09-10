#include "AST.hpp"

Type strToType(const string &s) {
  if (s == "int") return TYPE_INT;
  if (s == "double") return TYPE_DOUBLE;
  if (s == "void") return TYPE_VOID;
  cerr << "Unknown type: " << s << endl;
  exit(1);
}

FunctionNode::FunctionNode(const string& t, const string& n, const Params& p):
  name(n), params(p)
{
  returnType = strToType(t);
}

ostream &operator<< (ostream &os, const Variable& var) {
  os <<  var.name << ":" << var.typeLexeme;
  return os;
}

ostream &operator<< (ostream & os, const FunctionNode & f) {
  os << f.returnType << " " << f.name << "(";
  printVector(f.params);
  os << ")";
  return os;
}

