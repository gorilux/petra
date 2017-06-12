// Copyright Jacqueline Kay 2017
// Distributed under the MIT License.
// See accompanying LICENSE.md or https://opensource.org/licenses/MIT

#include "petra/sequence_map.hpp"
#include "petra/utilities/sequence.hpp"
#include "utilities.hpp"

#include <iostream>

static constexpr std::size_t sequence_size = 3;
static constexpr std::size_t upper_bound = 4;
using Array = std::array<std::size_t, sequence_size>;

struct minimal {
  template<std::size_t... Sequence>
  auto operator()(std::index_sequence<Sequence...>&&) noexcept {
  }

  auto operator()(petra::InvalidInputError&&) noexcept {
  }
};



struct callback {
  template<std::size_t... Sequence, std::size_t... Indices>
  auto operator()(std::index_sequence<Sequence...>&& seq, const Array& input,
                  std::index_sequence<Indices...>&&) noexcept {
    (PETRA_ASSERT(petra::access_sequence<Indices>(seq) == input[Indices]), ...);
  }

  template<typename... Args>
  auto operator()(petra::InvalidInputError&&, Args&&...) noexcept {
    PETRA_ASSERT(false);
  }
};

int main() {
  {
    Array test{{1, 3, 2}};
    auto m = petra::make_sequence_map<sequence_size, upper_bound>(minimal{});
    static_assert(noexcept(m(test)));
  }
  {
    Array test{{1, 3, 2}};

    auto m = petra::make_sequence_map<sequence_size, upper_bound>(callback{});
    // static_assert(noexcept(m(test, test, std::make_index_sequence<sequence_size>{})));
    m(test, test, std::make_index_sequence<sequence_size>{});
  }
  {
    /*
    // Error case
    std::array<std::size_t, sequence_size> test{{1, 3, 2}};

    auto m = petra::make_sequence_map<sequence_size, upper_bound>(callback{});
    m(test, test, std::make_index_sequence<sequence_size>{});
    */
  }

  return 0;
}
