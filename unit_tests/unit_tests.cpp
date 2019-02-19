/** @file unit_tests.cpp
  @author Stephen DeSalvo
  @date February, 2019
  @brief Trying out Catch2 for unit tests

  As of right now, there are no unit tests anywhere.  In some sense, however, 
  the comments contains lots of examples which help you test whether it works.
  This is my attempt to formalize these comments into unit tests as painlessly
  as possible.
 */

#include <string>
#include "../fraction.h"
#include "../polymorphic_graph.h"


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "Catch2/catch.hpp"


namespace dsl = desalvo_standard_library;


// Tests for fraction.h.
TEST_CASE( "Fractions", "[fractions]" ) {
    REQUIRE( dsl::Fraction<int>(0,6) == dsl::Fraction<int>(0,1) );
    REQUIRE( dsl::Fraction<int>(1,-2) == dsl::Fraction<int>(-1,2) );
    REQUIRE( dsl::Fraction<int>(4,6)+dsl::Fraction<int>(2,3) == dsl::Fraction<int>(4,3) );
    REQUIRE( dsl::Fraction<int>(5,13)+dsl::Fraction<int>(7,8) == dsl::Fraction<int>(131,104) );
    REQUIRE( dsl::Fraction<int>(6) == dsl::Fraction<int>(6,1) );
    REQUIRE( dsl::Fraction<int>(6) == dsl::Fraction<int>(12,2) );

    dsl::Fraction<int> c(0,1);
    REQUIRE( (double)(c) == 0 );

    dsl::Fraction<int> a(1,2);
    dsl::Fraction<int> b(2,3);
    REQUIRE(a != b);
    REQUIRE( !(a == b) );
    REQUIRE(a <= b);
    REQUIRE( !(a > b) );
    REQUIRE(b >= a);
    REQUIRE( !(b < a) );

    // Now apply some operations to a and b
    REQUIRE (++a == dsl::Fraction<int>(3,2));
    REQUIRE (--a == dsl::Fraction<int>(1,2));
    REQUIRE (a++ == dsl::Fraction<int>(1,2));
    REQUIRE (a == dsl::Fraction<int>(3,2));
    REQUIRE (a-- == dsl::Fraction<int>(3,2));
    REQUIRE (a == dsl::Fraction<int>(1,2));

}

// Fun idea but not really needed in this case.
// SCENARIO("Fraction Class", "[fractions]") {

//   GIVEN("Two fractions 1/2 and 2/3") {
//     dsl::Fraction<int> a(1,2);
//     dsl::Fraction<int> b(2,3);
//     WHEN("We test for equality") {
//       THEN("We get false") {
//        REQUIRE(a != b);
//        REQUIRE( !(a == b) );
//      }
//    }
//     WHEN("We test for inequality") {
//       THEN("We get true") {
//        REQUIRE(a <= b);
//        REQUIRE( !(a > b) );
//        REQUIRE(b >= a);
//        REQUIRE( !(b < a) );
//      }
//    }
//  }
// }

// Tests for polymorhpic_graph.h.
TEST_CASE( "Polymorphic Graph", "[polymorphic_graph]" ) {
    // REQUIRE( dsl::Fraction<int>(0,6) == dsl::Fraction<int>(0,1) );
    // REQUIRE( dsl::Fraction<int>(1,-2) == dsl::Fraction<int>(-1,2) );
    unsigned int seed = 42;
    std::default_random_engine generator(seed);

    dsl::PolymorphicGraph CompleteGraph = dsl::complete_graph(6);
    auto initial_state = CompleteGraph.initial_state();

    REQUIRE (initial_state->get_label() == std::string("0"));

    auto neighbors = CompleteGraph.neighbors(initial_state, false);
    size_t n = neighbors.size();

    REQUIRE(n == 5);

    std::vector<std::string> labels(neighbors.size());
    for (int i=0;i<n; ++i) {
      labels[i] = neighbors[i]->get_label();
    }
    REQUIRE (labels == std::vector<std::string>{"1", "2", "3", "4", "5"});

    size_t counter = 1;
    for (auto& neighbor : neighbors) {
      std::vector<std::string> full_set {"0", "1", "2", "3", "4", "5"};

      auto local_neighbors = CompleteGraph.neighbors(neighbor, false);
      size_t local_n = local_neighbors.size();
      REQUIRE(local_n == 5);

      std::vector<std::string> local_labels(local_neighbors.size());
      for (int i=0;i<n; ++i) {
        local_labels[i] = local_neighbors[i]->get_label();
      }
      full_set.erase(std::begin(full_set)+counter);
      REQUIRE (local_labels == full_set);
      ++counter;

    }



    //dsl::PolymorphicGraph graph = dsl::random_weighted_graph(6, .8, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_line_with_source_sink(10, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph(6, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph_with_source_sink(100, generator);

    //std::string filename = "input/graph.txt"; //argv[1];
    //dsl::PolymorphicGraph graph = dsl::graph_from_file(filename);
    //graph.print(std::cout);



}


