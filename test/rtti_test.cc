/**
 * @file rtti_test.cc
 * @author Derek Huang
 * @brief rtti.h unit tests
 * @copyright MIT License
 */

#include "pddcp/rtti.h"

#include <regex>
#include <string>
#include <typeinfo>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Test fixture class for RTTI tests.
 */
class RttiTest : public ::testing::Test {};

/**
 * Test that demangling of a type works as expected.
 */
TEST_F(RttiTest, DemangleTypeTest)
{
  auto type_name = PDDCP_DEMANGLED_NAME(std::vector<unsigned int>);
  // inexact, but this avoids us needing to memorize the exact type strings
  EXPECT_NE(std::string::npos, type_name.find("std::vector"));
  EXPECT_NE(std::string::npos, type_name.find("unsigned"));
}

/**
 * Test that demangling of an expression works as expected.
 */
TEST_F(RttiTest, DemangleExprTest)
{
  auto type_name = PDDCP_DEMANGLED_NAME(2 * 2.4 + 2 / static_cast<double>(4));
  // inexact. not sure what compiler vendor would not return "double" here
  EXPECT_NE(std::string::npos, type_name.find("double"));
}

}  // namespace
