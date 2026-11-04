/** @file finished_components_tests.cpp
  @brief Tests for binary_integer.h and subsets.h.

  Both were unusable until they were finished, and in both cases the reason
  was structural rather than mathematical: binary_integer kept its definitions
  in a .cpp that nothing built and that did not compile, and subsets had every
  member private with no constructor, so the enumeration logic inside it could
  not be reached. These tests exist so that neither can quietly stop being
  reachable again.
 */

#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../binary_integer.h"
#include "../std_cout.h"
#include "../subsets.h"
#include "Catch2/catch.hpp"

namespace dsl = desalvo_standard_library;


TEST_CASE( "binary_integer carries values past 64 bits", "[binary_integer]" ) {
    SECTION( "round-trips a value through its decimal form" ) {
        dsl::binary_integer a(1234567890123LL);
        std::ostringstream out;
        out << a;
        REQUIRE( out.str() == "1234567890123" );
    }

    SECTION( "adds" ) {
        dsl::binary_integer a(1234567890123LL);
        dsl::binary_integer b("1011");
        a += b;
        std::ostringstream out;
        out << a;
        REQUIRE( out.str() == "1234567891134" );
    }

    SECTION( "carries across a digit boundary" ) {
        dsl::binary_integer a(999LL), one(1LL);
        a += one;
        std::ostringstream out;
        out << a;
        REQUIRE( out.str() == "1000" );
    }
}


TEST_CASE( "subsets enumerates every subset exactly once", "[subsets]" ) {
    typedef std::vector<size_t> V;

    // The sequence is documented as emptyset, 1, 2, ..., N, 12, 13, ... so for
    // n it must visit all 2^n subsets and then stop. Counting distinct ones is
    // what says next_in_sequence is right, rather than merely that it compiles.
    for (size_t n = 1; n <= 5; ++n) {
        dsl::subsets<dsl::store::random_access, size_t, V, std::vector<V> > s(n);
        std::set<V> seen;
        seen.insert(s.get());                       // the empty set it starts on

        size_t steps = 0;
        const size_t cap = 1u << 10;                // so a broken loop fails, not hangs
        while (s.next_in_sequence() && ++steps < cap)
            seen.insert(s.get());

        REQUIRE( steps < cap );                     // it terminated on its own
        REQUIRE( seen.size() == (size_t(1) << n) );
        REQUIRE( s.n() == n );
    }
}


TEST_CASE( "subsets produces sets that are sorted and in range", "[subsets]" ) {
    typedef std::vector<size_t> V;
    dsl::subsets<dsl::store::random_access, size_t, V, std::vector<V> > s(6);

    size_t steps = 0;
    do {
        V cur = s.get();
        for (size_t i = 0; i < cur.size(); ++i) {
            REQUIRE( cur[i] >= 1 );
            REQUIRE( cur[i] <= 6 );
            if (i) REQUIRE( cur[i-1] < cur[i] );    // strictly increasing: a set
        }
    } while (s.next_in_sequence() && ++steps < 1024);
    REQUIRE( steps > 0 );
}
