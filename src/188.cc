/**
 * @file 188.cc
 * @author Derek Huang
 * @brief #188 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * What will this code print out?
 *
 *     def make_functions():
 *         flist = []
 *
 *         for i in [1, 2, 3]:
 *             def print_i():
 *                 print(i)
 *             flist.append(print_i)
 *
 *         return flist
 *
 *     functions = make_functions()
 *     for f in functions:
 *         f()
 *
 * How can we make it print out what we apparently want?
 */

#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/common.h"
#include "pddcp/utility.h"

namespace {

/**
 * Class template for a value printer.
 *
 * An instance can be invoked to write to `std::cout` or a specified stream.
 *
 * @tparam T type
 */
template <typename T>
class value_printer {
public:
  using value_type = T;

  /**
   * Ctor initializing by copy.
   *
   * @param value Value to print when invoked
   */
  value_printer(const T& value) : value_{value} {}

  /**
   * Ctor initializing by move.
   *
   * @param value Value to print when invoked
   */
  value_printer(T&& value) : value_{std::move(value)} {}

  /**
   * Return a const reference to the contained value.
   */
  const T& value() const { return value_; }

  /**
   * Write the contained value to a stream.
   *
   * @tparam CharT Char type
   * @tparam Traits Char traits class
   *
   * @param stream Stream to write value to
   */
  template <typename CharT, typename Traits>
  void operator()(std::basic_ostream<CharT, Traits>& stream) const
  {
    stream << value_;
  }

  /**
   * Write the contained value to `std::cout`.
   */
  void operator()() const { std::cout << value_; }

private:
  T value_;
};

/**
 * Class template for a faulty value printer.
 *
 * This mimics the behavior of the incorrect nested function creation done in
 * the original Python code by having each created printer instance modify a
 * static class variable, which is in fact the value that gets printed.
 *
 * An instance can be invoked to write to `std::cout` or a specified stream.
 * Although unnecessary in this case, we follow good practice and make all uses
 * of the shared value require mutex acquisition.
 *
 * @tparam T type
 */
template <typename T>
class bad_value_printer {
public:
  using value_type = T;

  /**
   * Ctor initializing by copy.
   *
   * @param value Value to update the shared value with
   */
  bad_value_printer(const T& value)
  {
    std::lock_guard<std::mutex> lock{shared_mutex()};
    shared_value() = value;
  }

  /**
   * Write the shared value to a stream.
   *
   * @tparam CharT Char type
   * @tparam Traits Char traits class
   *
   * @param stream Stream to write the shared value to
   */
  template <typename CharT, typename Traits>
  void operator()(std::basic_ostream<CharT, Traits>& stream) const
  {
    std::lock_guard<std::mutex> lock{shared_mutex()};
    stream << shared_value();
  }

  /**
   * Write the shared value to `std::cout`.
   */
  void operator()() const
  {
    std::lock_guard<std::mutex> lock{shared_mutex()};
    std::cout << shared_value();
  }

private:
  /**
   * Shared mutex for the `bad_value_printer<T>`.
   *
   * @note Since C++11, initialization of block scope statics is thread-safe.
   */
  static auto& shared_mutex()
  {
    static std::mutex m;
    return m;
  }

  /**
   * Shared value for the `bad_value_printer<T>`.
   *
   * @note Since C++11, initialization of block scope statics is thread-safe.
   */
  static auto& shared_value()
  {
    static T v;
    return v;
  }

};

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`
 */
template <typename IndexedType>
class DailyTest188 : public ::testing::Test {};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, unsigned int>;
using InputType2 = pddcp::indexed_type<1, std::string>;
using InputType3 = pddcp::indexed_type<2, long>;

/**
 * Helper macro defining each `DailyTest188` specialization.
 *
 * Each specialization has `values_`, `res_` and `bad_res_` static members for
 * the values to create printers for, the expected contents written to a string
 * stream by `value_printer<T>` instances, and the expected contents written to
 * a string stream by `bad_value_printer<T>` instances.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param init `PDDCP_INIT_LIST(...)` of values to create `T` printers for
 * @param res Expected value written to the output string stream
 */
#define DAILY_TEST_188(input_type, init, res) \
  template <> \
  class DailyTest188<input_type> : public ::testing::Test { \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
  protected: \
    static inline const std::vector<element_type> values_ init; \
    static inline const std::string res_{res}; \
    static inline const std::string bad_res_{ \
      [] \
      { \
        std::stringstream stream; \
        for ( \
          typename std::decay_t<decltype(values_)>::size_type i = 0; \
          i < values_.size(); \
          i++ \
        ) \
          stream << values_.back(); \
        return stream.str(); \
      }() \
    }; \
  }

/**
 * Specializations for each input/output pair.
 */

DAILY_TEST_188(InputType1, PDDCP_INIT_LIST(1, 2, 3), "123");
DAILY_TEST_188(InputType2, PDDCP_INIT_LIST("hi", "hey", "hm"), "hiheyhm");
DAILY_TEST_188(InputType3, PDDCP_INIT_LIST(2, 4, 6, 8, 10), "246810");

using DailyTest188Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest188, DailyTest188Types);

/**
 * Test that the `value_printer<T>` class template works as expected.
 *
 * The unit test code is the C++ analogue of the following Python code:
 *
 * @code{.py}
 * from typing import Any, Callable, List, Iterable
 *
 *
 * def make_printers(values: Iterable[Any]) -> List[Callable[[], None]]:
 *
 *     def printer_factory(v: Any) -> Callable[[], None]:
 *         return lambda: print(v)
 *
 *     return [printer_factory(value) for value in values]
 *
 *
 * # in the unit test, DAILY_TEST_188 specifies the input
 * for printer in make_printers([1, 2, 3]):
 *     printer()
 * @endcode
 *
 * The above code a cleaned up version of what the Python code presented in
 * the problem should be if it was correct. Here the use of `printer_factory`
 * ensures that each printer function is associated with the address of each
 * object in `values`, as Python passes values by reference. If we simply used
 * `lambda: print(value)` in the generator expression, we would have the same
 * problem as the original Python code, i.e. only the last value is printed, as
 * each lambda is maintaining a reference to `value`, which at the end of the
 * loop refers to the last value in `values` and does not get garbage collected.
 *
 * Note that in C++, this is more or less a non-issue, as arguments are passed
 * by value unless we are explicitly passing by reference.
 */
TYPED_TEST(DailyTest188, PrinterTest)
{
  // populate vector of printers
  std::vector<value_printer<typename TestFixture::element_type>> printers;
  for (const auto& value : TestFixture::values_)
    printers.emplace_back(value);
  // send to stream and compare to expected result
  std::stringstream stream;
  for (const auto& printer : printers)
    printer(stream);
  EXPECT_EQ(TestFixture::res_, stream.str());
}

/**
 * Test that `bad_value_printer<T>` class template works as expected.
 *
 * The unit test code is the C++ analogue of the following Python code:
 *
 * @code{.py}
 * from typing import Any, Callable, List
 *
 *
 * def make_bad_printers(values: Iterable[Any]) -> List[Callable[[], None]]:
 *     return [lambda: print(value) for value in values]
 *
 *
 * # in the unit test, DAILY_TEST_188 specifies the input
 * for printer in make_bad_printers([1, 2, 3]):
 *     printer()
 * @endcode
 *
 * The above code is a cleaned up version of the original Python code presented
 * in problem, containing the same exact problem. Instead of printing 1, 2, 3
 * on separate lines, the above code prints 3, 3, 3 on separate lines. The
 * reason for this is due to how Python passes values by reference. Each lambda
 * refers to `value`, so it is not garbage collected, but its value after the
 * end of the generator expression is `3`, hence each lambda will print `3`.
 *
 * `PrinterTest` shows the correct Python code, which uses a factory function
 * to return a function object to break the reference chain.
 */
TYPED_TEST(DailyTest188, BadPrinterTest)
{
  // populate vector of bad printers
  std::vector<bad_value_printer<typename TestFixture::element_type>> printers;
  for (const auto& value : TestFixture::values_)
    printers.emplace_back(value);
  // send to stream and compare to expected bad result
  std::stringstream stream;
  for (const auto& printer : printers)
    printer(stream);
  EXPECT_EQ(TestFixture::bad_res_, stream.str());
}

}  // namespace
