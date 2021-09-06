#include <gtest/gtest.h>

#include "Parser.hpp"

TEST(ParsePunctuator, BasicPunctuators) {

  stringstream input ("int test = 1;\n"
                      "if (test == 3) return 0;");
  Parser parser;
  parser.parse(input);
}
