/** @file integer_partition_tests.cpp

  @brief Tests for integer_partition.h.

  Random partitions are checked by property rather than by expected value:
  that a sampler promising an exact size delivers it, that a restriction on
  the part sizes is obeyed, and that every partition of a small n is actually
  reachable with roughly its share of the mass. The last of those is the one
  that matters -- a sampler can look fine and still never emit some
  partitions, and only a frequency check notices.
 */

#include <cmath>
#include <map>
#include <set>
#include <string>

#include "../integer_partition.h"
#include "Catch2/catch.hpp"

namespace dsl = desalvo_standard_library;
typedef unsigned long long ull;


TEST_CASE( "An exact-size sampler returns exactly n", "[integer_partition]" ) {
    dsl::IP::UnrestrictedPartition ip;
    const ull sizes[] = {1, 2, 3, 4, 17, 100, 500};

    for (ull n : sizes) {
        ip.RejectionSampling(n);
        REQUIRE( ip.n() == n );

        ip.PDCDeterministicSecondHalf(n);
        REQUIRE( ip.n() == n );

        ip(n);
        REQUIRE( ip.n() == n );
    }
}


TEST_CASE( "A restriction on the part sizes is obeyed", "[integer_partition]" ) {
    SECTION( "odd parts only" ) {
        dsl::IP::OddPartition odd;
        odd(200);
        REQUIRE( odd.n() == 200 );
        for (auto part : odd.AsMultiset())
            REQUIRE( part % 2 == 1 );
    }

    SECTION( "even parts only" ) {
        dsl::IP::EvenPartition even;
        even(200);
        REQUIRE( even.n() == 200 );
        for (auto part : even.AsMultiset())
            REQUIRE( part % 2 == 0 );
    }

    SECTION( "a restriction supplied by the caller" ) {
        struct PerfectCubes { ull operator()(ull i) { return i*i*i; } };
        dsl::IP::IntegerPartition<PerfectCubes> cubes;
        cubes(1000);
        REQUIRE( cubes.n() == 1000 );
        for (auto part : cubes.AsMultiset()) {
            ull root = static_cast<ull>(std::llround(std::cbrt(double(part))));
            REQUIRE( root * root * root == part );
        }
    }
}


TEST_CASE( "Every partition of 4 is reachable", "[integer_partition]" ) {
    // 4 / 3+1 / 2+2 / 2+1+1 / 1+1+1+1 -- five of them, each a fifth of the
    // mass. A sampler that cannot emit one of them is still a sampler; it is
    // just not sampling what it claims to.
    dsl::IP::UnrestrictedPartition ip;
    std::map<std::multiset<ull, std::greater<ull> >, int> seen;
    const int draws = 4000;

    for (int t = 0; t < draws; ++t) {
        ip.RejectionSampling(4);
        ++seen[ip.AsMultiset()];
    }

    REQUIRE( seen.size() == 5 );

    // and each at roughly its share: a fifth of 4000 is 800, so a count
    // below 400 would mean that partition is being suppressed
    for (auto& partition_and_count : seen)
        REQUIRE( partition_and_count.second > 400 );
}
