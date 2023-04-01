/**
 * @file 27.cc
 * @author Derek Huang
 * @brief #27 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * Given a string of round, curly, and square open and closing brackets, return
 * whether the brackets are balanced (well-formed). For example, given the
 * string "([])[]({})", you should return true. Given the string "([)]" or
 * "((()", you should return false.
 */

#include <istream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/common.h"

namespace {

/**
 * Macro for defining the type aliases in a `(open|close)_bracket_map`.
 *
 * @param type Char type
 */
#define PDDCP_BRACKET_MAP_TYPES(type) \
  using char_type = type; \
  using map_type = std::unordered_map<type, type>

/**
 * Macro for defining the available operators in a `(open|close)_bracket_map`.
 *
 * The `operator->` overload is for working with `const` member functions of
 * the underlying unordered map while the `operator[]` overload simply wraps a
 * call to the `at` member function of the unordered map.
 *
 * @note The `static_assert(true)` is used to force a semicolon.
 */
#define PDDCP_BRACKET_MAP_OPERATORS() \
  const map_type* operator->() const { return &map_; } \
  char_type operator[](char_type key) const { return map_.at(key); } \
  static_assert(true)

/**
 * Macro for beginning the body of an `open_bracket_map`.
 *
 * @param type Char type
 */
#define PDDCP_BEGIN_OPEN_BRACKET_MAP(type) \
public: \
  PDDCP_BRACKET_MAP_TYPES(type); \
  open_bracket_map() : map_

/**
 * Macro for beginning the body of a `close_bracket_map`.
 *
 * @param type Char type
 */
#define PDDCP_BEGIN_CLOSE_BRACKET_MAP(type) \
public: \
  PDDCP_BRACKET_MAP_TYPES(type); \
  close_bracket_map() : map_

/**
 * Macro for ending the body of a `(open|close)_bracket_map`.
 */
#define PDDCP_END_BRACKET_MAP \
   {} \
  PDDCP_BRACKET_MAP_OPERATORS(); \
private: \
  map_type map_;

/**
 * Main template for the open bracket map.
 *
 * Maps open brackets, e.g. '[', '(', etc. to the associated close brackets.
 *
 * @tparam CharT char type
 */
template <typename CharT>
struct open_bracket_map {
  // static_assert(false);
};

/**
 * Main template for the close bracket map.
 *
 * Maps close brackets, e.g. ']', ')', etc. to the associated open brackets.
 *
 * @tparam CharT char type
 */
template <typename CharT>
struct close_bracket_map {  };

/**
 * `open_bracket_map<CharT>` specializations.
 *
 * Currently available for `char`, `wchar_t`, `char16_t`, `char32_t`.
 */

template <>
struct open_bracket_map<char> {
  PDDCP_BEGIN_OPEN_BRACKET_MAP(char)
  {{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}}
  PDDCP_END_BRACKET_MAP
};

template <>
struct open_bracket_map<wchar_t> {
  PDDCP_BEGIN_OPEN_BRACKET_MAP(wchar_t)
  {{L'(', L')'}, {L'[', L']'}, {L'{', L'}'}, {L'<', L'>'}}
  PDDCP_END_BRACKET_MAP
};

template <>
struct open_bracket_map<char16_t> {
  PDDCP_BEGIN_OPEN_BRACKET_MAP(char16_t)
  {{u'(', u')'}, {u'[', u']'}, {u'{', u'}'}, {u'<', u'>'}}
  PDDCP_END_BRACKET_MAP
};

template <>
struct open_bracket_map<char32_t> {
  PDDCP_BEGIN_OPEN_BRACKET_MAP(char32_t)
  {{U'(', U')'}, {U'[', U']'}, {U'{', U'}'}, {U'<', U'>'}}
  PDDCP_END_BRACKET_MAP
};

/**
 * `close_bracket_map<CharT>` specializations.
 *
 * Currently available for `char`, `wchar_t`, `char16_t`, `char32_t`.
 */

template <>
struct close_bracket_map<char> {
  PDDCP_BEGIN_CLOSE_BRACKET_MAP(char)
  {{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}}
  PDDCP_END_BRACKET_MAP
};

template <>
struct close_bracket_map<wchar_t> {
  PDDCP_BEGIN_CLOSE_BRACKET_MAP(wchar_t)
  {{L')', L'('}, {L']', L'['}, {L'}', L'{'}, {L'>', L'<'}}
  PDDCP_END_BRACKET_MAP
};

template <>
struct close_bracket_map<char16_t> {
  PDDCP_BEGIN_CLOSE_BRACKET_MAP(char16_t)
  {{u')', u'('}, {u']', u'['}, {u'}', u'{'}, {u'>', u'<'}}
  PDDCP_END_BRACKET_MAP
};

template <>
struct close_bracket_map<char32_t> {
  PDDCP_BEGIN_CLOSE_BRACKET_MAP(char32_t)
  {{U')', U'('}, {U']', U'['}, {U'}', U'{'}, {U'>', U'<'}}
  PDDCP_END_BRACKET_MAP
};

template <typename CharT, typename Container>
bool update_bracket_stack(std::stack<CharT, Container>& stack, CharT c)
{
  // maps opening to closing brackets and vice versa
  static open_bracket_map<CharT> open_map;
  static close_bracket_map<CharT> close_map;
  // push open brackets onto the stack
  if (open_map->find(c) != open_map->cend())
    stack.push(c);
  // if it's a close bracket, check that last pushed bracket is open bracket
  else if (close_map->find(c) != close_map->cend()) {
    // if it's not the open bracket, mismatch, so return early, otherwise pop
    if (stack.top() != close_map[c])
      return false;
    stack.pop();
  }
  // otherwise just don't do anything
  return true;
}

template <typename CharT, typename Traits = std::char_traits<CharT>>
bool brackets_balanced(std::basic_istream<CharT, Traits>& stream)
{
  std::stack<CharT> stack;
  CharT c;
  while (!stream.eof()) {
    stream.get(c);
    // if expression is false, brackets are unbalanced
    if (!update_bracket_stack(stack, c))
      return false;
  }
  // if stack is nonempty, return false, otherwise we succeeded
  if (stack.size())
    return false;
  return true;
}

template <typename CharT, typename Traits, typename Allocator>
bool brackets_balanced(const std::basic_string<CharT, Traits, Allocator>& input)
{
  std::stack<CharT> stack;
  for (const auto& c : input)
    if (!update_bracket_stack(stack, c))
      return false;
  if (stack.size())
    return false;
  return true;
}

template <typename CharT>
inline bool brackets_balanced(const CharT* input)
{
  return brackets_balanced(std::basic_string<CharT>{input});
}

// convenience type aliases
using input_type = std::tuple<
  std::string, std::wstring, std::u16string, std::u32string
>;
using result_type = bool;
using pair_type = std::pair<input_type, result_type>;

/**
 * Macro for populating `input_type` using a single string literal.
 *
 * @param str Unprefixed string literal
 */
#define INPUT_TYPE_FROM(str) \
  input_type{ \
    str, \
    PDDCP_CONCAT(L, str), \
    PDDCP_CONCAT(u, str), \
    PDDCP_CONCAT(U, str) \
  }

// macros for the different string literals we want to check.
#define LITERAL_1 "([])[]({})"
#define LITERAL_2 "([)]"
#define LITERAL_3 "((()"

/**
 * Test fixture class for parametrized tests.
 */
class DailyTest27 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest27,
  ::testing::Values(
    pair_type{INPUT_TYPE_FROM("([])[]({})"), true},
    pair_type{INPUT_TYPE_FROM("([)]"), false},
    pair_type{INPUT_TYPE_FROM("((()"), false}
  )
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest27,
  ::testing::Values(
    pair_type{INPUT_TYPE_FROM("(([[]{{}}])){[]}()[[]]"), true},
    pair_type{INPUT_TYPE_FROM("sfdf(([ssdf[]{{}}])){[hello]}()[[oo]]"), true},
    pair_type{INPUT_TYPE_FROM("{{([]))}}uhwf{[]}[()sdfsdf"), false}
  )
);

TEST_P(DailyTest27, CharParamTest)
{
  EXPECT_EQ(
    GetParam().second,
    brackets_balanced(std::get<std::string>(GetParam().first))
  );
}

TEST_P(DailyTest27, WideCharParamTest)
{
  EXPECT_EQ(
    GetParam().second,
    brackets_balanced(std::get<std::wstring>(GetParam().first))
  );
}

TEST_P(DailyTest27, Char16ParamTest)
{
  EXPECT_EQ(
    GetParam().second,
    brackets_balanced(std::get<std::u16string>(GetParam().first))
  );
}

TEST_P(DailyTest27, Char32ParamTest)
{
  EXPECT_EQ(
    GetParam().second,
    brackets_balanced(std::get<std::u32string>(GetParam().first))
  );
}

}  // namespace
