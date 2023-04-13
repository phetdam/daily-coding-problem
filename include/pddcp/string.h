/**
 * @file string.h
 * @author Derek Huang
 * @brief C++ header for string operations
 * @copyright MIT License
 */

#ifndef PDDCP_STRING_H_
#define PDDCP_STRING_H_

#include <cstdint>
#include <sstream>
#include <string>

namespace pddcp {

/**
 * Join a container of values into a string.
 *
 * @tparam Container *Container* with `value_type` compatible with `operator<<`
 * @tparam CharT Char type
 * @tparam Traits Char traits
 * @tparam Alloc Allocator
 *
 * @param values Values to join into string
 * @param delim Delimiter to use when joining values
 */
template <typename Container, typename CharT, typename Traits, typename Alloc>
auto string_join(
  const Container& values,
  const std::basic_string<CharT, Traits, Alloc>& delim)
{
  std::basic_stringstream<CharT, Traits, Alloc> stream;
  auto n_values = values.size();
  for (decltype(n_values) i = 0; i < n_values; i++) {
    stream << values[i];
    if (i == n_values - 1)
      break;
    stream << delim;
  }
  return stream.str();
}

/**
 * Join a container of values into a string.
 *
 * @tparam Container *Container* with `value_type` compatible with `operator<<`
 * @tparam CharT Char type
 *
 * @param values Values to join into string
 * @param delim Delimiter to use when joining values
 */
template <typename Container, typename CharT>
inline auto string_join(const Container& values, const CharT* delim)
{
  return string_join(values, std::basic_string<CharT>{delim});
}

/**
 * Join a container of values into a `std::string` with no delimiter.
 *
 * @tparam Container *Container* with `value_type` compatible with `operator<<`
 *
 * @param values Values to join into string
 */
template <typename Container>
inline auto string_join(const Container& values)
{
  return string_join(values, "");
}

/**
 * Check that a string is a palindrome.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits
 * @tparam Alloc Allocator
 *
 * @param str String to check
 */
template <typename CharT, typename Traits, typename Alloc>
bool is_palindrome(const std::basic_string<CharT, Traits, Alloc>& str)
{
  // empty string is not considered palindrome
  if (str.empty())
    return false;
  // right and left endpoints of string
  auto right_i = str.size() - 1;
  decltype(right_i) left_i = 0;
  // move towards center to check
  while (left_i < right_i)
    if (str[left_i++] != str[right_i--])
      return false;
  return true;
}


}  // namespace pddcp

#endif  // PDDCP_STRING_H_
