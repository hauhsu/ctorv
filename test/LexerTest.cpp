#include <gtest/gtest.h>

#include "Lexer.hpp"

// Demonstrate some basic assertions.
TEST(ParsePunctuator, BasicPunctuators) {
  auto lexer = Lexer();
  lexer.set_input_string("(");
  auto punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::LEFT_PAREN);
  EXPECT_EQ(punc->punc, "(");
}
