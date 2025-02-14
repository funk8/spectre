// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <ostream>
#include <utility>

#include "Utilities/Requires.hpp"
#include "Utilities/TMPL.hpp"
#include "Utilities/TypeTraits/IsA.hpp"

namespace cpp20 {
namespace detail {
template <typename T, size_t Size, size_t... Is>
constexpr std::array<T, Size> convert_to_array(
    // NOLINTNEXTLINE(modernize-avoid-c-arrays)
    const T (&t)[Size], std::index_sequence<Is...> /*meta*/) {
  return {{t[Is]...}};
}
}  // namespace detail

/// \brief A `std::array` implementation with partial C++20 support
///
/// \warning This is not a standard-compliant C++20 `std::array` implementation.
/// We provide this implementation because we need the `constexpr operator==`
/// from C++20. Note that other C++20 changes, such as the replacement of the
/// other comparison operators in favor of `operator<=>`, are not implemented.
/// This class can be removed when we support C++20.
template <typename T, size_t Size>
struct array {
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = value_type*;
  using const_iterator = const value_type*;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  // clang-tidy: mark explicit. We want implicit conversion
  constexpr operator std::array<T, Size>() const {  // NOLINT
    return detail::convert_to_array(data_, std::make_index_sequence<Size>{});
  }

  constexpr iterator begin() {
    return iterator(data_);  // NOLINT
  }
  constexpr const_iterator begin() const {
    return const_iterator(data_);  // NOLINT
  }
  constexpr iterator end() {
    return iterator(data_ + Size);  // NOLINT
  }
  constexpr const_iterator end() const {
    return const_iterator(data_ + Size);  // NOLINT
  }

  constexpr const_iterator cbegin() const { return begin(); }
  constexpr const_iterator cend() const { return end(); }

  constexpr size_type size() const { return Size; }
  constexpr size_type max_size() const { return Size; }
  constexpr bool empty() const { return Size == 0; }

  constexpr reference operator[](const size_type i) {
    return data_[i];  // NOLINT
  }
  constexpr const_reference operator[](const size_type i) const {
    return data_[i];  // NOLINT
  }

  constexpr reference at(const size_type i) {
    return data_[i];  // NOLINT
  }
  constexpr const_reference at(const size_type i) const {
    return data_[i];  // NOLINT
  }

  constexpr reference front() { return data_[0]; }
  constexpr const_reference front() const { return data_[0]; }
  constexpr reference back() { return data_[Size > 0 ? Size - 1 : 0]; }
  constexpr const_reference back() const {
    return data_[Size > 0 ? Size - 1 : 0];
  }

  constexpr value_type* data() { return data_; }
  constexpr const value_type* data() const { return data_; }

  // NOLINTNEXTLINE(modernize-avoid-c-arrays)
  value_type data_[Size > 0 ? Size : 1];
};
namespace detail {
template <typename T = void>
struct Equal : std::binary_function<T, T, bool> {
  constexpr bool inline operator()(const T& lhs, const T& rhs) const {
    return lhs == rhs;
  }
};

template <>
struct Equal<void> {
  template <class T0, class T1>
  constexpr bool inline operator()(T0&& lhs, T1&& rhs) const {
    return std::forward<T0>(lhs) == std::forward<T1>(rhs);
  }
};
}  // namespace detail

template <typename InputIter1, typename InputIter2,
          typename BinaryPred = detail::Equal<>>
constexpr bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                     BinaryPred pred = detail::Equal<>{}) {
  for (; first1 != last1; ++first1, ++first2) {
    if (not pred(*first1, *first2)) {
      return false;
    }
  }
  return true;
}

template <typename InputIter1, typename InputIter2,
          typename BinaryPred = std::less_equal<>>
constexpr bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                       InputIter2 first2, InputIter2 last2,
                                       BinaryPred pred = std::less_equal<>{}) {
  for (; first2 != last2; ++first1, ++first2) {
    if (first1 == last1 or pred(*first1, *first2)) {
      return true;
    } else if (pred(*first2, *first1)) {
      return false;
    }
  }
  return false;
}

template <class T, size_t Size>
inline constexpr bool operator==(const array<T, Size>& x,
                                 const array<T, Size>& y) {
  return equal(x.data_, x.data_ + Size, y.data_);  // NOLINT
}

template <class T, size_t Size>
inline constexpr bool operator!=(const array<T, Size>& lhs,
                                 const array<T, Size>& rhs) {
  return not(lhs == rhs);
}

template <class T, size_t Size>
inline constexpr bool operator<(const array<T, Size>& lhs,
                                const array<T, Size>& rhs) {
  return lexicographical_compare(lhs.__elems_, lhs.__elems_ + Size,
                                 rhs.__elems_, rhs.__elems_ + Size);
}

template <class T, size_t Size>
inline constexpr bool operator>(const array<T, Size>& lhs,
                                const array<T, Size>& rhs) {
  return rhs < lhs;
}

template <class T, size_t Size>
inline constexpr bool operator<=(const array<T, Size>& lhs,
                                 const array<T, Size>& rhs) {
  return not(rhs < lhs);
}

template <class T, size_t Size>
inline constexpr bool operator>=(const array<T, Size>& lhs,
                                 const array<T, Size>& rhs) {
  return not(lhs < rhs);
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const array<T, 0>& /*a*/) {
  return os << "()";
}

template <typename T, size_t N>
inline std::ostream& operator<<(std::ostream& os, const array<T, N>& a) {
  os << '(';
  for (size_t i = 0; i < N - 1; ++i) {
    os << a[i] << ',';
  }
  if (N > 0) {
    os << a[N - 1];
  }
  os << ')';
  return os;
}
}  // namespace cpp20

namespace detail {
template <typename List, size_t... indices,
          Requires<not tt::is_a_v<tmpl::list, tmpl::front<List>>> = nullptr>
inline constexpr auto make_cpp20_array_from_list_helper(
    std::integer_sequence<size_t, indices...> /*meta*/)
    -> cpp20::array<std::decay_t<decltype(tmpl::front<List>::value)>,
                    tmpl::size<List>::value> {
  return cpp20::array<std::decay_t<decltype(tmpl::front<List>::value)>,
                      tmpl::size<List>::value>{
      {tmpl::at<List, tmpl::size_t<indices>>::value...}};
}
}  // namespace detail

/// \ingroup ConstantExpressionsGroup
/// Make an array from a typelist that holds std::integral_constant's all of
/// which have the same `value_type`
///
/// \tparam List the typelist of std::integral_constant's
/// \return array of integral values from the typelist
template <typename List,
          Requires<not tt::is_a_v<tmpl::list, tmpl::front<List>>> = nullptr>
inline constexpr auto make_cpp20_array_from_list()
    -> cpp20::array<std::decay_t<decltype(tmpl::front<List>::value)>,
                    tmpl::size<List>::value> {
  return detail::make_cpp20_array_from_list_helper<List>(
      std::make_integer_sequence<size_t, tmpl::size<List>::value>{});
}

template <typename TypeForZero,
          Requires<not tt::is_a_v<tmpl::list, TypeForZero>> = nullptr>
inline constexpr cpp20::array<std::decay_t<TypeForZero>, 0>
make_cpp20_array_from_list() {
  return cpp20::array<std::decay_t<TypeForZero>, 0>{{}};
}

namespace detail {
template <typename T, size_t Size, size_t... Is>
inline constexpr cpp20::array<T, Size> convert_to_cpp20_array_impl(
    const std::array<T, Size>& t,
    std::index_sequence<
        Is...> /*meta*/) {
  return {{t[Is]...}};
}
}  // namespace detail

template <typename T, size_t Size, size_t... Is>
inline constexpr cpp20::array<T, Size>
convert_to_cpp20_array(const std::array<T, Size>& t) {
  return detail::convert_to_cpp20_array_impl(t,
                                             std::make_index_sequence<Size>{});
}
