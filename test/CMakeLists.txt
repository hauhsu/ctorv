include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
FetchContent_MakeAvailable(googletest)




add_executable(
  LexerTest
  LexerTest.cpp
)

add_executable(
  ParserTest
  ParserTest.cpp
)
target_link_libraries(
  ParserTest
  lctorv
  gtest_main
)
target_link_libraries(
  LexerTest
  lctorv
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(LexerTest)
gtest_discover_tests(ParserTest)
