#include "procdraw/test_support/function_spec_checker.h"
#include "gtest/gtest.h"
#include <string>

class FunctionSpecsTest : public ::testing::Test
{
protected:
  void CheckSpec(const std::string& filename, int expectedNumTests)
  {
    bool passed =
      checker_.Check(ResolveSpecFile(filename).c_str(), expectedNumTests);
    EXPECT_TRUE(passed);
    if (!passed) {
      for (auto message : checker_.GetMessages()) {
        ADD_FAILURE() << message;
      }
    }
  }

  static std::string ResolveSpecFile(const std::string& filename)
  {
    return PROCDRAW_FUNCTION_SPECS_DIR + std::string("/") + filename;
  }

  procdraw_test::FunctionSpecChecker checker_;
};

TEST_F(FunctionSpecsTest, Clear)
{
  CheckSpec("clear.xml", 5);
}

TEST_F(FunctionSpecsTest, Cons)
{
  CheckSpec("cons.xml", 3);
}

TEST_F(FunctionSpecsTest, Difference)
{
  CheckSpec("difference.xml", 5);
}

TEST_F(FunctionSpecsTest, Eq)
{
  CheckSpec("eq.xml", 7);
}

TEST_F(FunctionSpecsTest, Functionp)
{
  CheckSpec("functionp.xml", 4);
}

TEST_F(FunctionSpecsTest, Get)
{
  CheckSpec("get.xml", 5);
}

TEST_F(FunctionSpecsTest, Keys)
{
  CheckSpec("keys.xml", 4);
}

TEST_F(FunctionSpecsTest, MakeDict)
{
  CheckSpec("make-dict.xml", 5);
}

TEST_F(FunctionSpecsTest, Memb)
{
  CheckSpec("memb.xml", 6);
}

TEST_F(FunctionSpecsTest, Product)
{
  CheckSpec("product.xml", 5);
}

TEST_F(FunctionSpecsTest, Put)
{
  CheckSpec("put.xml", 5);
}

TEST_F(FunctionSpecsTest, Quotient)
{
  CheckSpec("quotient.xml", 4);
}

TEST_F(FunctionSpecsTest, Sum)
{
  CheckSpec("sum.xml", 5);
}
