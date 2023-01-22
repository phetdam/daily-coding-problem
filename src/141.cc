/**
 * @file 141.cc
 * @author Derek Huang
 * @brief #141 from Daily Coding Problem
 *
 * This problem was asked by Microsoft.
 *
 * Implement 3 stacks using a single list:
 *
 * class Stack:
 *
 *    def __init__(self):
 *        self.list = []
 *
 *    def pop(self, stack_number):
 *        pass
 *
 *    def push(self, item, stack_number):
 *        pass
 */

#include <array>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * A class managing `n` separate stacks using a single `std::vector<T>`.
 *
 * @tparam T value type
 */
template <typename T>
class multi_stack {
public:
  using value_type = T;
  using size_type = typename std::vector<T>::size_type;
  using difference_type = typename std::vector<T>::difference_type;

  /**
   * Ctor.
   *
   * @param n_stacks number of stacks
   */
  multi_stack(size_type n_stacks = 1) : ends_(n_stacks) {}

  /**
   * Ctor template to construct initial state from multiple *Container* types.
   *
   * @tparam Cs... *Container* types
   *
   * @param init_values variable list of *Container* objects to get values
   */
  template <typename... Cs>
  multi_stack(const Cs&... init_values)
  {
    size_type total_offset = 0;
    // order matters here, so unary left fold must be done
    (
      ...
      ,
      [&]
      {
        // GCC 9.3 bug -- internal compiler error. Godbolt reveals that GCC
        // 11.3 is minimum required version for this static_assert to compile
        // correctly, while any C++17 supporting Clang has no issues.
#if !defined(__clang__) && defined(__GNUG__) && \
  (__GNUG__ < 11 || (__GNUG__ == 11 && __GNUC_MINOR__ < 3))
#error "GCC >=11.3 required for correct compilation of line 76"
#else
        static_assert(std::is_same_v<T, typename Cs::value_type>);
#endif
        values_.insert(values_.cend(), init_values.cbegin(), init_values.cend());
        total_offset += init_values.size();
        // track where each stack ends
        ends_.push_back(total_offset);
      }()
    );
  }

  /**
   * Return number of values held in the multi stack.
   */
  auto size() const { return values_.size(); }

  /**
   * Returns number of values held in the multi stack.
   */
  auto n_values() const { return size(); }

  /**
   * Return number of stacks managed by the multi stack.
   */
  auto n_stacks() const { return ends_.size(); }

  /**
   * Return const ref to underlying vector managing the stack values.
   */
  const auto& values() const { return values_; }

  /**
   * Return const ref to vector holding indices indicating the stacks' ends.
   */
  const auto& ends() const { return ends_; }

  /**
   * Peek the value at the top of specified stack.
   *
   * @param stack_number stack to peek at
   */
  const auto& peek(size_type stack_number)
  {
    check_stack_nonempty();
    check_stack_number(stack_number);
    return values_[ends_[stack_number] - 1];
  }

  /**
   * Pop a value from a specified stack 0-index.
   *
   * @param stack_number stack to pop a value from
   * @returns value popped off of stack `stack_number`
   */
  auto pop(size_type stack_number)
  {
    check_stack_nonempty();
    check_stack_number(stack_number);
    auto value = values_.erase(values_.cbegin() + (ends_[stack_number] - 1));
    for (size_type i = stack_number; i < ends_.size(); i++)
      ends_[i]--;
    return value;
  }

  /**
   * Push a value onto a stack specified by its 0-index.
   *
   * @param value value to push
   * @param stack_number stack to push a value onto
   * @returns reference to `*this` to allow method chaining
   */
  const auto& push(T value, size_type stack_number)
  {
    check_stack_number(stack_number);
    values_.insert(values_.cbegin() + ends_[stack_number], value);
    for (size_type i = stack_number; i < ends_.size(); i++)
      ends_[i]++;
    return *this;
  }

private:
  std::vector<T> values_;
  std::vector<size_type> ends_;

  /**
   * Check that the specified stack number is valid.
   *
   * @param stack_number stack index
   */
  void check_stack_number(size_type stack_number)
  {
    if (stack_number >= ends_.size())
      throw std::runtime_error("stack number must be < " + ends_.size());
  }

  /**
   * Check that the stack is not empty.
   */
  void check_stack_nonempty()
  {
    if (!values_.size())
      throw std::runtime_error("multi_stack is empty");
  }
};

/**
 * Test fixture template for testing the multi stack.
 *
 * @tparam T value type
 */
template <typename T>
class DailyTest141 : public ::testing::Test {
protected:
  // three different Container objects to populate 3 different stacks
  static inline const std::vector<T> first_values_{1, 4, 23, 5, 777};
  static inline const std::set<T> second_values_{6, 7, 8};
  static inline const std::array<T, 4> third_values_{0, 8, 16, 32};
};

using DailyTest141Types = ::testing::Types<unsigned long, double, int>;
TYPED_TEST_SUITE(DailyTest141, DailyTest141Types);

/**
 * Test that default and multi-container ctors work as expected.
 */
TYPED_TEST(DailyTest141, CtorTest)
{
  // default has no elements, and only one stack
  multi_stack<TypeParam> empty_stack;
  EXPECT_EQ(1, empty_stack.n_stacks());
  EXPECT_EQ(0, empty_stack.n_values());
  // multi-container will have 3 stacks
  multi_stack<TypeParam> full_stack{
    this->first_values_, this->second_values_, this->third_values_
  };
  EXPECT_EQ(3, full_stack.n_stacks());
  EXPECT_EQ(
    this->first_values_.size() +
    this->second_values_.size() +
    this->third_values_.size(),
    full_stack.n_values()
  );
}

}  // namespace
