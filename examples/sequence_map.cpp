// Copyright Jacqueline Kay 2017
// Distributed under the MIT License.
// See accompanying LICENSE.md or https://opensource.org/licenses/MIT

#include "petra/sequence_map.hpp"
#include "petra/utilities.hpp"

#include <iostream>

struct callback {

 template<std::size_t... I>
 auto operator()(std::integer_sequence<std::size_t, I...>&&) {
   // Do stuff
   (std::cout << ... << I);
   std::cout << "\n";
 }

 auto operator()(petra::InvalidInputError&&) {
   // Do stuff
   std::cout << "ERROR\n";
 }
};

int main(int argc, char** argv) {
  std::array<std::size_t, 3> test{{0}};
  if (argc > 11) {
    std::cout << "Sorry, we can only take 10 integers from the command line.\n";
    return 255;
  }

  for (int i = 1; i < argc; ++i) {
    test[i] = std::stoi(argv[i]);
  }

  auto m = petra::make_sequence_map<3, 3ul>(callback{});
  m(test);

  return 0;
}
