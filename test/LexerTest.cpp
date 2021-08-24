#include <gtest/gtest.h>

#include "Lexer.hpp"

// Demonstrate some basic assertions.
TEST(ParsePunctuator, BasicPunctuators) {
  auto lexer = Lexer();
  lexer.set_input_string("(){}==+-=;||,|");

  auto punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::LEFT_PAREN);
  EXPECT_EQ(punc->punc, "(");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::RIGHT_PAREN);
  EXPECT_EQ(punc->punc, ")");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::LEFT_BRACE);
  EXPECT_EQ(punc->punc, "{");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::RIGHT_BRACE);
  EXPECT_EQ(punc->punc, "}");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::EQ);
  EXPECT_EQ(punc->punc, "==");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::PLUS);
  EXPECT_EQ(punc->punc, "+");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::MINUS);
  EXPECT_EQ(punc->punc, "-");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::ASSIGN);
  EXPECT_EQ(punc->punc, "=");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::SEMICOLON);
  EXPECT_EQ(punc->punc, ";");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::LOGIC_OR);
  EXPECT_EQ(punc->punc, "||");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::COMMA);
  EXPECT_EQ(punc->punc, ",");

  punc = dynamic_pointer_cast<Punctuator>(lexer.getNextToken());
  EXPECT_EQ(punc->tag, Tag::BITWISE_OR);
  EXPECT_EQ(punc->punc, "|");
}

TEST(ParseKeyWords, Basic) {
  auto lexer = Lexer();
  lexer.set_input_string("true false return int if else");

  auto word = dynamic_pointer_cast<Word>(lexer.getNextToken());
  EXPECT_EQ(word->tag, Tag::TRUE);
  EXPECT_EQ(word->lexeme, "true");

  word = dynamic_pointer_cast<Word>(lexer.getNextToken());
  EXPECT_EQ(word->tag, Tag::FALSE);
  EXPECT_EQ(word->lexeme, "false");

  word = dynamic_pointer_cast<Word>(lexer.getNextToken());
  EXPECT_EQ(word->tag, Tag::RETURN);
  EXPECT_EQ(word->lexeme, "return");

  word = dynamic_pointer_cast<Word>(lexer.getNextToken());
  EXPECT_EQ(word->tag, Tag::TYPE);
  EXPECT_EQ(word->lexeme, "int");

  word = dynamic_pointer_cast<Word>(lexer.getNextToken());
  EXPECT_EQ(word->tag, Tag::IF);
  EXPECT_EQ(word->lexeme, "if");

  word = dynamic_pointer_cast<Word>(lexer.getNextToken());
  EXPECT_EQ(word->tag, Tag::ELSE);
  EXPECT_EQ(word->lexeme, "else");
}

TEST(ParseId, Basic) {
  auto lexer = Lexer();
  lexer.set_input_string("variable var_name _var");

  auto id = dynamic_pointer_cast<Id>(lexer.getNextToken());
  EXPECT_EQ(id->tag, Tag::IDENTIFIER);
  EXPECT_EQ(id->lexeme, "variable");

  id = dynamic_pointer_cast<Id>(lexer.getNextToken());
  EXPECT_EQ(id->tag, Tag::IDENTIFIER);
  EXPECT_EQ(id->lexeme, "var_name");

  id = dynamic_pointer_cast<Id>(lexer.getNextToken());
  EXPECT_EQ(id->tag, Tag::IDENTIFIER);
  EXPECT_EQ(id->lexeme, "_var");
}
