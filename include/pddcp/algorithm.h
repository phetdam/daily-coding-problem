/**
 * @file algorithm.h
 * @author Derek Huang
 * @brief C++ header for some useful algorithms
 * @copyright MIT License
 */

#ifndef PDDCP_ALGORITHM_H_
#define PDDCP_ALGORITHM_H_

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace pddcp {

/**
 * Return sum of the negative values in a range.
 *
 * If the iterator traits value type is unsigned, zero is returned.
 *
 * @tparam InputIt Input iterator type
 *
 * @param begin Iterator to first element
 * @param end Iterator one past last element
 */
template <typename InputIt>
auto negative_sum(InputIt begin, InputIt end)
{
  // helper type aliases
  using iter_traits_type = std::iterator_traits<InputIt>;
  using value_type = typename iter_traits_type::value_type;
  // if type is unsigned, only need to return zero
  if constexpr (std::is_unsigned_v<value_type>)
    return value_type{};
  // otherwise, perform the actual summation
  else
    return std::accumulate(
      begin,
      end,
      value_type{},
      [](const auto& acc, const auto& v)
      {
        return (v < value_type{}) ? acc + v : acc;
      }
    );
}

/**
 * Return sum of the negative values in a container.
 *
 * If the container's value type is unsigned, zero is returned.
 *
 * @tparam Container *Container* with arithmetic `value_type`
 *
 * @param values Container to compute sum of negative values for
 */
template <typename Container>
inline auto negative_sum(const Container& values)
{
  return negative_sum(values.begin(), values.end());
}

/**
 * Return sum of the positive values in a range.
 *
 * If the iterator traits value type is unsigned, its sum is returned.
 *
 * @tparam InputIt Input iterator type
 *
 * @param begin Iterator to first element
 * @param end Iterator one past last element
 */
template <typename InputIt>
auto positive_sum(InputIt begin, InputIt end)
{
  // helper type aliases
  using iter_traits_type = std::iterator_traits<InputIt>;
  using value_type = typename iter_traits_type::value_type;
  // if type is unsigned, just sum everything up
  if constexpr (std::is_unsigned_v<value_type>)
    return std::accumulate(begin, end, value_type{});
  // otherwise, need to check for positive values
  else
    return std::accumulate(
      begin,
      end,
      value_type{},
      [](const auto& acc, const auto& v)
      {
        return (v > value_type{}) ? acc + v : acc;
      }
    );
}

/**
 * Return sum of the positive values in a container.
 *
 * If the container's value type is unsigned, its sum is returned.
 *
 * @tparam Container *Container* with arithmetic `value_type`
 *
 * @param values Container to compute sum of positive values for
 */
template <typename Container>
inline auto positive_sum(const Container& values)
{
  return positive_sum(values.begin(), values.end());
}

}  // namespace pddcp

#endif  // PDDCP_ALGORITHM_H_
