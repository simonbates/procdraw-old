#include "procdraw_core_tests/function_spec_checker.h"
#include "gtest/gtest.h"
#include <string>
#include <utility>

typedef std::pair<std::string, int> SpecInfo;

// *********************************************************
// BEGIN FUNCTION SPECS
// *********************************************************

static SpecInfo functionSpecs[] = {
  SpecInfo("clear.xml", 5),      SpecInfo("cons.xml", 3),
  SpecInfo("difference.xml", 5), SpecInfo("eq.xml", 7),
  SpecInfo("functionp.xml", 4),  SpecInfo("get.xml", 5),
  SpecInfo("keys.xml", 4),       SpecInfo("make-dict.xml", 5),
  SpecInfo("memb.xml", 6),       SpecInfo("product.xml", 5),
  SpecInfo("put.xml", 5),        SpecInfo("quotient.xml", 4),
  SpecInfo("sum.xml", 5)
};

// *********************************************************
// END FUNCTION SPECS
// *********************************************************

class FunctionSpecsTest : public ::testing::TestWithParam<SpecInfo>
{
protected:
  static std::string ResolveSpecFile(const std::string& filename)
  {
    return PROCDRAW_FUNCTION_SPECS_DIR + std::string("/") + filename;
  }

  procdraw_test::FunctionSpecChecker checker_;
};

TEST_P(FunctionSpecsTest, Check)
{
  bool passed = checker_.Check(ResolveSpecFile(GetParam().first).c_str(),
                               GetParam().second);
  EXPECT_TRUE(passed);
  if (!passed) {
    for (auto message : checker_.GetMessages()) {
      ADD_FAILURE() << message;
    }
  }
}

INSTANTIATE_TEST_CASE_P(CheckFunctionSpecs, FunctionSpecsTest,
                        ::testing::ValuesIn(functionSpecs));
