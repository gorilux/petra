// Copyright Jacqueline Kay 2017
// Distributed under the MIT License.
// See accompanying LICENSE.md or https://opensource.org/licenses/MIT

#pragma once

#include <array>
#include <utility>

#include "petra/sequential_table.hpp"
#include "petra/utilities.hpp"

namespace petra {

  /* Given a sequence size known at compile time, map a std::array to a std::integer_sequence
   * */

  // do we need a "recursive sequential table?"
  template<auto SeqSize, decltype(SeqSize) UpperBound, decltype(SeqSize) CurrentIndex, decltype(SeqSize)... Sequence>
  struct helper {
    using Integral = decltype(SeqSize);
    static constexpr Integral Size = utilities::abs(SeqSize);

    // why do these have to be const anyway?
    template<Integral I, typename F>
    constexpr auto operator()(
        std::integral_constant<Integral, I>&&, const std::array<Integral, Size>& input, F& callback) const {
      if constexpr (CurrentIndex == Size - 1) {
        return callback(std::integer_sequence<Integral, Sequence..., I>{});
      } else {
        static_assert(CurrentIndex < Size - 1);
        return make_sequential_table<UpperBound>(
            helper<SeqSize, UpperBound, CurrentIndex + static_cast<Integral>(1), Sequence..., I>{})
        (input[CurrentIndex + static_cast<Integral>(1)], input, callback);
      }
    }

    template<typename F>
    constexpr auto operator()(InvalidInputError&& e, const std::array<Integral, Size>&, F& callback) const {
      return callback(std::forward<InvalidInputError>(e));
    }
  };

  template<typename F, auto SeqSize, decltype(SeqSize) UpperBound>
  struct SequenceMap {
    constexpr SequenceMap(F&& f) : callback(f) {}
    using Integral = decltype(SeqSize);

    constexpr auto operator()(const std::array<Integral, utilities::abs(SeqSize)>& input) {
      return seq_map(input[static_cast<Integral>(0)], input, callback);
    }

  private:
    static constexpr auto seq_map = make_sequential_table<UpperBound>(
        helper<SeqSize, UpperBound, static_cast<Integral>(0)>{});

    F callback;
  };

  template<auto SeqSize, decltype(SeqSize) UpperBound, typename F>
  constexpr decltype(auto) make_sequence_map(F&& f) {
    return SequenceMap<F, SeqSize, UpperBound>(std::forward<F>(f));
  }



}  // namespace petra
