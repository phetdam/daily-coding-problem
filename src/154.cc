/**
 * @file 154.cc
 * @author Derek Huang
 * @brief #154 from Daily Coding Problem
 * @copyright MIT License
 *
 * Implement a stack API using only a heap. A stack implements the following
 * methods:
 *
 * push(item), which adds an element to the stack
 * pop(), which removes and returns the most recently added element (or throws
 * an error if there is nothing on the stack)
 *
 * Recall that a heap has the following operations:
 *
 * push(item), which adds a new key to the heap
 * pop(), which removes and returns the max value of the heap
 */

#include <cstdio>
#include <deque>
#include <iterator>
#include <list>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/type_traits.h"
#include "pddcp/utility.h"

namespace {

/**
 * Stack implemented via max heap based on insertion index.
 *
 * The idea is to use the number of inserted items as a priority and use a max
 * heap sorting based on an index associated with each inserted value that is
 * derived from the existing number of inserted items.
 *
 * @tparam T type
 */
template <typename T>
class heapstack {
public:
  using value_type = T;
  using indexed_type = std::pair<std::size_t, T>;

  /**
   * Comparison functor for `std::priority_queue<T>` to sort by index.
   *
   * @param a First value
   * @param b second value
   */
  struct comparator {
    bool operator()(const indexed_type& a, const indexed_type& b)
    {
      return a.first < b.first;
    }
  };

  using container_type = std::priority_queue<
    indexed_type, std::vector<indexed_type>, comparator
  >;

  /**
   * Default ctor.
   */
  heapstack() : heap_{}, n_pushed_{} {}

  /**
   * Range-initialization ctor.
   *
   * @tparam InputIt Iterator with appropriate `value_type`
   *
   * @param first Iterator to first element
   * @param last Iterator one past the last element
   */
  template <typename InputIt>
  heapstack(InputIt first, InputIt last) : heapstack{} { push(first, last); }

  /**
   * Container-initialization ctor.
   *
   * @tparam Container *Container* with appropriate `value_type`
   *
   * @param values Container of values to initialize from
   */
  template <typename Container>
  heapstack(const Container& values) : heapstack{} { push(values); }

  /**
   * Return number of elements in the stack.
   */
  auto size() const { return heap_.size(); }

  /**
   * Return const reference to the top item in the stack.
   */
  const auto& top() const { return heap_.top(); }

  /**
   * Push an element onto the stack.
   *
   * @param value Value to push
   * @returns Reference to `*this` to allow method chaining
   */
  auto& push(const T& value)
  {
    heap_.emplace(n_pushed_++, value);
    return *this;
  }

  /**
   * Push elements from a range onto the stack.
   *
   * @tparam InputIt Iterator with appropriate `value_type`
   *
   * @param first Iterator to first element
   * @param last Iterator one past the last element
   */
  template <typename InputIt>
  auto& push(InputIt first, InputIt last)
  {
    static_assert(
      std::is_same_v<
        typename std::iterator_traits<InputIt>::value_type, value_type
      >,
      "InputIt value_type must equal heapstack value_type"
    );
    for (auto iter = first; iter < last; iter++)
      push(*iter);
    return *this;
  }

  /**
   * Push elements from a container onto the stack.
   *
   * @tparam Container *Container* with appropriate `value_type`
   *
   * @param values Container of values to push
   * @returns Reference to `*this` to allow method chaining
   */
  template <typename Container>
  auto& push(const Container& values)
  {
    static_assert(
      std::is_same_v<typename Container::value_type, value_type>,
      "Container value_type must equal heapstack value_type"
    );
    for (const auto& value : values)
      push(value);
    return *this;
  }

  /**
   * Pop an element from the stack, returning the popped element.
   */
  auto pop()
  {
    auto top = heap_.top().second;
    heap_.pop();
    return top;
  }

  /**
   * Pop an element from the stack, writing the popped element to an lvalue.
   *
   * @param value Value reference to write popped element to
   * @returns Reference to `*this` to allow method chaining
   */
  auto& pop(T& value)
  {
    value = heap_.top().second;
    heap_.pop();
    return *this;
  }

  /**
   * Pop up to `n` elements from the stack into a container.
   *
   * If `n` exceeds the size of the stack, an exception is raised. This
   * overload is selected if the container implements `emplace_back`.
   *
   * @tparam Container *Container* with appropriate `value_type`
   *
   * @param values Container to emplace popped values into
   * @param n Number of values to pop
   * @returns Reference to `*this` to allow method chaining
   */
  template <
    typename Container,
    std::enable_if_t<
      pddcp::is_emplace_back_container_v<Container>, bool> = true>
  auto& pop(Container& values, std::size_t n)
  {
    if (n > size())
      throw std::runtime_error{"n cannot exceed size of stack"};
    for (decltype(n) i = 0; i < n; i++)
      values.emplace_back(pop());
    return *this;
  }

  /**
   * Pop up to `n` elements from the stack into a container.
   *
   * If `n` exceeds the size of the stack, an exception is raised. This
   * overload is selected if the container only implements `push_back`. If the
   * container implements `emplace_back`, the above overload is selected.
   *
   * @tparam Container *Container* with appropriate `value_type`
   *
   * @param values Container to push popped values into
   * @param n Number of values to pop
   * @returns Reference to `*this` to allow method chaining
   */
  template <
    typename Container,
    std::enable_if_t<
      pddcp::is_push_back_container_v<Container> &&
      !pddcp::is_emplace_back_container_v<Container>, bool> = true>
  auto& pop(Container& values, std::size_t n)
  {
    if (n > size())
      throw std::runtime_error{"n cannot exceed size of stack"};
    for (decltype(n) i = 0; i < n; i++)
      values.push_back(pop());
    return *this;
  }

  /**
   * Pop all elements from the stack into a container.
   *
   * Values will be emplaced using `emplace_back` if possible, but will use
   * `push_back` if the container does not have an `emplace_back` method.
   *
   * @tparam Container *Container* with appropriate `value_type`
   *
   * @param values Container to emplace or push popped values into
   * @returns Reference to `*this` to allow method chaining
   */
  template <typename Container>
  auto& pop(Container& values)
  {
    return pop(values, size());
  }

private:
  container_type heap_;
  std::size_t n_pushed_;
};

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with *Container* `value_type`
 */
template <typename IndexedType>
class DailyTest154 : public ::testing::Test {};

// input types used in specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, std::vector<int>>;
using InputType2 = pddcp::indexed_type<1, std::deque<double>>;
using InputType3 = pddcp::indexed_type<2, std::list<unsigned int>>;

/**
 * Helper macro defining each `DailyTest154` specialization.
 *
 * Each specialization has `input_` and `res_` and `bad_res_` static members
 * for the input container values and the expected vector of reversed values
 * obtained by popping all the stack values into a container.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param input_init `PDDCP_INIT_LIST(...)` of input values
 */
#define DAILY_TEST_154(input_type, input_init) \
  template <> \
  class DailyTest154<input_type> : public ::testing::Test { \
  public: \
    PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(input_type); \
  protected: \
    static inline const element_type input_ input_init; \
    static inline const auto res_{ \
      [] \
      { \
        std::vector<value_type> out; \
        out.reserve(input_.size()); \
        /* not all reverse iterators support operator<, so we use != */ \
        for (auto riter = input_.rbegin(); riter != input_.rend(); riter++) \
          out.emplace_back(*riter); \
        return out; \
      }() \
    }; \
  }

// specializations for each input/output pair
DAILY_TEST_154(InputType1, PDDCP_INIT_LIST(-1, 3, 2, 5));
DAILY_TEST_154(InputType2, PDDCP_INIT_LIST(-4, 1.3, 3.2, 6));
DAILY_TEST_154(InputType3, PDDCP_INIT_LIST(6, 1, 7, 14, 20, 51, 26));

using DailyTest154Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest154, DailyTest154Types);

/**
 * Test that `heapstack<T>` works as expected.
 */
TYPED_TEST(DailyTest154, TypedTest)
{
  heapstack<typename TestFixture::value_type> stack{TestFixture::input_};
  std::decay_t<decltype(TestFixture::res_)> popped;
  stack.pop(popped);
  EXPECT_EQ(TestFixture::res_, popped);
}

}  // namespace
