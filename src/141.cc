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
        static_assert(std::is_same_v<T, typename Cs::value_type>);
        values_.insert(values_.cend(), init_values.cbegin(), init_values.cend());
        total_offset += init_values.size();
        // track where each stack ends
        ends_.push_back(total_offset);
      }()
    );
  }

  auto size() const { return values_.size(); }
  auto n_values() const { return size(); }
  auto n_stacks() const { return ends_.size(); }
  const auto& values() const { return values_; }
  const auto& ends() const { return ends_; }

  /**
   * Pop a value from a specified stack 0-index.
   *
   * @param stack_number stack to pop a value from
   * @returns value popped off of stack `stack_number`
   */
  auto pop(size_type stack_number)
  {
    if (stack_number >= ends_.size())
      throw std::runtime_error("stack number must be < " + ends_.size());
    if (!values_.size())
      throw std::runtime_error("multi_stack is empty");
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
    if (stack_number >= ends_.size())
      throw std::runtime_error("stack number must be < " + ends_.size());
    values_.insert(values_.cbegin() + ends_[stack_number], value);
    for (size_type i = stack_number; i < ends_.size(); i++)
      ends_[i]++;
    return *this;
  }

private:
  std::vector<T> values_;
  std::vector<size_type> ends_;
};

}  // namespace
