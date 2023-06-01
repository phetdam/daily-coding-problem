/**
 * @file 194.cc
 * @author Derek Huang
 * @brief #194 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * Suppose you are given two lists of n points, one list p1, p2, ..., pn on the
 * line y = 0 and the other list q1, q2, ..., qn on the line y = 1. Imagine a
 * set of n line segments connecting each point pi to qi. Write an algorithm to
 * determine how many pairs of the line segments intersect.
 */

#include <cstdint>
#include <deque>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

template <typename FirstContainer, typename SecondContainer>
auto intersecting_pairs(const FirstContainer& ps, const SecondContainer& qs)
{
  // value_types need to be arithmetic
  static_assert(
    std::is_arithmetic_v<typename FirstContainer::value_type>,
    "FirstContainer value_type must be arithmetic"
  );
  static_assert(
    std::is_arithmetic_v<typename SecondContainer::value_type>,
    "SecondContainer value_type must be arithmetic"
  );
  // containers must be the same size
  if (ps.size() != qs.size())
    throw std::runtime_error{"ps and qs must have same size"};
  // number of points. if zero, then return zero. there is an unused lambda
  // capture warning due to compile-time selection of one of the branches
PDDCP_GNU_WARNING_PUSH()
PDDCP_GNU_WARNING_DISABLE(unused-lambda-capture)
  auto n_points = [&ps, &qs]
  {
    // the larger size_type type is used here
    if constexpr (
      sizeof(typename FirstContainer::size_type) >
      sizeof(typename SecondContainer::size_type)
    )
      return ps.size();
    else
      return qs.size();
  }();
PDDCP_GNU_WARNING_POP()
  if (!n_points)
    return n_points;
  // check all possible intersections
  decltype(n_points) n_isects = 0;
  for (decltype(n_points) i = 0; i < n_points; i++)
    for (decltype(n_points) j = i + 1; j < n_points; j++)
      // MSVC complains about signed/unsigned mismatch with int value_type.
      // unsigned i, j are not part of the evaluated value, so spurious warning
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4365)
      if (
        (*(ps.begin() + i) - *(ps.begin() + j)) *
        (*(qs.begin() + i) - *(qs.begin() + j)) <= 0
      )
PDDCP_MSVC_WARNING_POP()
        n_isects++;
  return n_isects;
}

/**
 * Helper macro for creating an initializer list out of varargs.
 *
 * @param ... Arguments for the initializer list
 */
#define INIT_LIST(...) {__VA_ARGS__}

/**
 * Helper for a pair whose `first_type` and `second_type` are the same.
 *
 * @param T type
 */
template <typename T>
using mono_pair = std::pair<T, T>;

template <typename IndexedType>
class DailyTest194 : public ::testing::Test {};

using InputType1 = pddcp::indexed_type<0, mono_pair<std::vector<double>>>;
using InputType2 = pddcp::indexed_type<1, mono_pair<std::vector<float>>>;
using InputType3 = pddcp::indexed_type<2, mono_pair<std::vector<double>>>;
using InputType4 = pddcp::indexed_type<
  3,
  std::pair<std::deque<double>, std::vector<double>>
>;
using InputType5 = pddcp::indexed_type<4, mono_pair<std::vector<int>>>;

#define DAILY_TEST_194(input_type, ps_init, qs_init, res) \
  template <> \
  class DailyTest194<input_type> : public ::testing::Test { \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
    using first_type = typename element_type::first_type; \
    using second_type = typename element_type::second_type; \
    /* size_type will be the larger of first_type, second_type size_types */ \
    using size_type = std::conditional_t< \
      /* parentheses disambiguate > as ending the template definition */ \
      ( \
        sizeof(typename first_type::size_type) > \
        sizeof(typename second_type::size_type) \
      ), \
      typename first_type::size_type, \
      typename second_type::size_type \
    >; \
  protected: \
    static inline const first_type ps_ ps_init; \
    static inline const second_type qs_ qs_init; \
    static inline const size_type res_ = res; \
  }

DAILY_TEST_194(InputType1, INIT_LIST(0.3, 0.2), INIT_LIST(0.2, 0.3), 1);

// explicitly use f suffix to please MSVC, otherwise emits C4305
DAILY_TEST_194(InputType2, INIT_LIST(0.1f, 0.2f), INIT_LIST(0.1f, 0.2f), 0);

DAILY_TEST_194(
  InputType3,
  INIT_LIST(0.3, 0.2, 0.4, 0.5, 0.1),
  INIT_LIST(0.1, 0.2, 0.3, 0.4, 0.5),
  5
);
DAILY_TEST_194(InputType4, INIT_LIST(0.2, 0.3, 0.1), INIT_LIST(0.1, 0.2, 0.3), 2);
DAILY_TEST_194(
  InputType5,
  INIT_LIST(0, -1, 1, 2, -2),
  INIT_LIST(-2, -1, 0, 1, 2),
  5
);

using DailyTest194Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4, InputType5
>;
TYPED_TEST_SUITE(DailyTest194, DailyTest194Types);

TYPED_TEST(DailyTest194, TypedTest)
{
  EXPECT_EQ(
    TestFixture::res_, intersecting_pairs(TestFixture::ps_, TestFixture::qs_)
  );
}

}  // namespace
