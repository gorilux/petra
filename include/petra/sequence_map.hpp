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
   *
   * struct callback {
   *
   * template<int... I>
   *  auto operator()(<std::integer_sequence<int, I...>&&) {
   *    // Do stuff
   *  }
   * };
   *
   * auto m = make_sequence_map<int, 10, 100>(callback{});
   *
   * */

  // do we need a "recursive sequential table?"
  template<typename Integral, auto SeqSize, Integral UpperBound, auto CurrentIndex, Integral... Sequence>
  struct helper {
    // why do these have to be const anyway?
    template<Integral I, typename F>
    constexpr auto operator()(
        std::integral_constant<Integral, I>&&, const std::array<Integral, SeqSize>& input, F& callback) const {
      if constexpr (CurrentIndex == SeqSize) {
        return callback(std::integer_sequence<Integral, Sequence...>{});
      } else {
        return make_sequential_table<UpperBound>(
            helper<Integral, SeqSize, UpperBound, CurrentIndex + 1, Sequence..., I>{})
        (input[CurrentIndex + 1], input, callback);
      }
    }

    template<typename F>
    constexpr auto operator()(InvalidInputError&& e, const std::array<Integral, SeqSize>&, F& callback) const {
      return callback(std::forward<InvalidInputError>(e));
    }
  };

  template<typename F, typename Integral, auto SeqSize, Integral UpperBound>
  struct SequenceMap {
    constexpr SequenceMap(F&& f) : callback(f) {}

    constexpr auto operator()(const std::array<Integral, SeqSize>& input) {
      return seq_map(input[0], input, callback);
    }

  private:
    static constexpr auto seq_map = make_sequential_table<UpperBound>(
        helper<Integral, SeqSize, UpperBound, 0>{});

    F callback;
  };

  template<auto SeqSize, auto UpperBound, typename F>
  constexpr decltype(auto) make_sequence_map(F&& f) {
    return SequenceMap<F, decltype(UpperBound), SeqSize, UpperBound>(std::forward<F>(f));
  }



}  // namespace petra
