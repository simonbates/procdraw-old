#include "procdraw/test/function_spec_checker.h"
#include "gtest/gtest.h"
#include <string>

class FunctionSpecCheckerTest : public ::testing::Test
{
protected:
  void ExpectFailure(const std::string& filename, int expectedNumTests,
                     const std::string& expectedMessage,
                     bool resolveFilename = true)
  {
    std::string fullpath;
    if (resolveFilename) {
      fullpath = ResolveTestFile(filename);
    } else {
      fullpath = filename;
    }
    EXPECT_FALSE(checker_.Check(fullpath.c_str(), expectedNumTests));
    auto messages = checker_.GetMessages();
    EXPECT_EQ(1, messages.size());
    if (messages.size() > 0) {
      EXPECT_EQ(expectedMessage, messages[0]);
    }
  }

  static std::string ResolveTestFile(const std::string& filename)
  {
    return PROCDRAW_FUNCTION_SPEC_CHECKER_TESTDATA_DIR + std::string("/") +
           filename;
  }

  procdraw_test::FunctionSpecChecker checker_;
};

TEST_F(FunctionSpecCheckerTest, NonExistingFile)
{
  ExpectFailure("NON_EXISTING_FILE", 0, "Error loading file: NON_EXISTING_FILE",
                false);
}

TEST_F(FunctionSpecCheckerTest, Testcase1)
{
  ExpectFailure("testcase1.xml", 1, "EXPR: (+ 1) EXPECTED: 0 ACTUAL: 1");
}

TEST_F(FunctionSpecCheckerTest, Testcase2)
{
  ExpectFailure("testcase2.xml", 2, "EXPR: (+ 2) EXPECTED: 0 ACTUAL: 2");
}

TEST_F(FunctionSpecCheckerTest, Testcase3)
{
  ExpectFailure("testcase3.xml", 2, "EXPR: (+ 3) EXPECTED: 0 ACTUAL: 3");
}

TEST_F(FunctionSpecCheckerTest, TooFewTests)
{
  ExpectFailure("passing.xml", 3, "EXPECTED 3 TESTS BUT 2 WERE RUN");
}

TEST_F(FunctionSpecCheckerTest, TooManyTests)
{
  ExpectFailure("passing.xml", 1, "EXPECTED 1 TESTS BUT 2 WERE RUN");
}

TEST_F(FunctionSpecCheckerTest, Passing)
{
  EXPECT_TRUE(checker_.Check(ResolveTestFile("passing.xml").c_str(), 2));
  EXPECT_EQ(0, checker_.GetMessages().size());
}
