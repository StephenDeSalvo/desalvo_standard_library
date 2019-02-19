/** @file unit_tests.cpp
  @author Stephen DeSalvo
  @date February, 2019
  @brief Trying out Catch2 for unit tests

  As of right now, there are no unit tests anywhere.  In some sense, however, 
  the comments contains lots of examples which help you test whether it works.
  This is my attempt to formalize these comments into unit tests as painlessly
  as possible.
 */

#include "fraction.h"
#include <iostream>

namespace dsl = desalvo_standard_library;

int main() {

    dsl::Fraction<int> a(1,2);
    dsl::Fraction<int> b(2,3);

    std::cout << a << std::endl;

    if (a == b) {
      std::cout << "they are equal" << std::endl;
    }
    return 0;
}



