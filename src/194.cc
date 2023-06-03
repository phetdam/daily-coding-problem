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

#include "pddcp/common.h"
#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

/**
 * Return number of intersecting pairs line segments.
 *
 * Let p(1), ... p(n) and q(1), ... q(n) be two sets of x-coordinates, where
 * each p(i) has a y-coordinate of 0 and each q(i) has a y-coordinate of 1, and
 * let L(i) represent the line segment with endpoints (p(i), 0), (q(i), 1). The
 * function returns for all i, j the number of intersecting L(i), L(j) pairs.
 *
 * Time complexity is O(pow(n, 2)) with n(n - 1) / 2 actual comparisons.
 *
 * @tparam FirstContainer *SequenceContainer* with arithmetic `value_type`
 * @tparam SecondContainer *SequenceContainer* with arithmetic `value_type`
 *
 * @param ps Set of x-coordinates for points with y-coordinate 0
 * @param qs Set of y-coordinates for points with y-coordinate 1
 */
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
 * Helper for a pair whose `first_type` and `second_type` are the same.
 *
 * @param T type
 */
template <typename T>
using mono_pair = std::pair<T, T>;

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, `std::pair<U, V>` for
 *  `element_type`, where `U`, `V`, are containers that support random access
 */
template <typename IndexedType>
class DailyTest194 : public ::testing::Test {};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, mono_pair<std::vector<double>>>;
using InputType2 = pddcp::indexed_type<1, mono_pair<std::vector<float>>>;
using InputType3 = pddcp::indexed_type<2, mono_pair<std::vector<double>>>;
using InputType4 = pddcp::indexed_type<
  3,
  std::pair<std::deque<double>, std::vector<double>>
>;
using InputType5 = pddcp::indexed_type<4, mono_pair<std::vector<int>>>;

/**
 * Helper macro defining each `DailyTest194` specialization.
 *
 * Each specialization has `ps_`, `qs_`, and `res_` static members for the
 * y = 0 points' x-coordinates, the y = 1 points' x-coordinates, and expected
 * number of intersecting line segments, respectively.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param ps_init `PDDCP_INIT_LIST(...)` of x-coordinates for y = 0 points
 * @param qs_init `PDDCP_INIT_LIST(...)` of x-coordinates for y = 1 points
 * @param res Expected number of intersecting line segments
 */
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

/**
 * Specialization for the first input/output pair.
 *
 * Models a single pair of intersecting segments.
 */
DAILY_TEST_194(
  InputType1, PDDCP_INIT_LIST(0.3, 0.2), PDDCP_INIT_LIST(0.2, 0.3), 1
);

/**
 * Specialization for the second input/output pair.
 *
 * Models a single pair of parallel line segments.
 *
 * @note We explicitly use the `f` suffix to please MSVC, other C4305 emitted
 */
DAILY_TEST_194(
  InputType2, PDDCP_INIT_LIST(0.1f, 0.2f), PDDCP_INIT_LIST(0.1f, 0.2f), 0
);

/**
 * Specialization for the third input/output pair.
 *
 * Models the following arrangement of points (5 intersecting pairs):
 *
 *  q(1)  q(2)  q(3)  q(4)  q(5)
 *
 *  p(5)  p(2)  p(1)  p(3)  p(4)
 */
DAILY_TEST_194(
  InputType3,
  PDDCP_INIT_LIST(0.3, 0.2, 0.4, 0.5, 0.1),
  PDDCP_INIT_LIST(0.1, 0.2, 0.3, 0.4, 0.5),
  5
);

/**
 * Specialization for the fourth input/output pair.
 *
 * Models the following arrangement of points (2 intersecting pairs):
 *
 *  q(1)  q(2)  q(3)
 *
 *  p(3)  p(1)  p(2)
 *
 * The first and second *SequenceContainer* types also differ here.
 */
DAILY_TEST_194(
  InputType4, PDDCP_INIT_LIST(0.2, 0.3, 0.1), PDDCP_INIT_LIST(0.1, 0.2, 0.3), 2
);

/**
 * Specializaton for the fifth input/output pair.
 *
 * Models the following arrangement of points (5 intersecting pairs):
 *
 *  q(1)  q(2)  q(3)  q(4)  q(5)
 *
 *  p(5)  p(2)  p(1)  p(3)  p(4)
 *
 * Here we use a *SequenceContainer* with `int` `value_type` for the input type.
 */
DAILY_TEST_194(
  InputType5,
  PDDCP_INIT_LIST(0, -1, 1, 2, -2),
  PDDCP_INIT_LIST(-2, -1, 0, 1, 2),
  5
);

using DailyTest194Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4, InputType5
>;
TYPED_TEST_SUITE(DailyTest194, DailyTest194Types);

/**
 * Test that `intersecting_pairs` works as expected.
 */
TYPED_TEST(DailyTest194, TypedTest)
{
  EXPECT_EQ(
    TestFixture::res_, intersecting_pairs(TestFixture::ps_, TestFixture::qs_)
  );
}

}  // namespace
