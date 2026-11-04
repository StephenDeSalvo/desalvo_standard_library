/** @file integer_partition_tests.cpp
  @author Stephen DeSalvo
  @date November, 2026
  @brief Tests for integer_partition.h.

  Deliberately NOT using the Catch2 in this directory. That copy predates
  modern glibc and no longer compiles (catch.hpp calls sysconf in a constexpr
  context), and unit_tests.cpp has not built since April 2020 anyway: commit
  041b878 moved polymorphic_graph.h to polymorphic/graph.h and renamed the
  classes without updating the test that uses them. Rather than make this file
  depend on fixing all of that, it stands alone:

      g++ -std=c++11 -O2 -iquote .. -o ip_tests integer_partition_tests.cpp && ./ip_tests

  What these guard against. The integer_partition.h that was here before was
  an unfinished Boltzmann sampler whose part loop ran `i < n`, so a part of
  size n could never occur. At n = 4 that silently deleted the partition {4} --
  a fifth of the sample space -- and nothing complained, because nothing was
  checking. The last test below is that specific regression.
 */

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>

#include "../integer_partition.h"

namespace dsl = desalvo_standard_library;
typedef unsigned long long ull;

static int failures = 0;

static void check(bool ok, const std::string& what) {
    std::cout << (ok ? "  ok    " : "  FAIL  ") << what << "\n";
    if (!ok) ++failures;
}

int main() {
    std::cout << "integer_partition.h\n";

    // ── the samplers that promise an exact size deliver it ──────────────────
    {
        dsl::IP::UnrestrictedPartition ip;
        bool rejection = true, pdc = true, call = true;
        const ull sizes[] = {1, 2, 3, 4, 17, 100, 500};
        for (ull n : sizes) {
            ip.RejectionSampling(n);          rejection &= (ip.n() == n);
            ip.PDCDeterministicSecondHalf(n); pdc       &= (ip.n() == n);
            ip(n);                            call      &= (ip.n() == n);
        }
        check(rejection, "RejectionSampling returns a partition of exactly n");
        check(pdc,       "PDCDeterministicSecondHalf returns exactly n");
        check(call,      "operator() returns exactly n");
    }

    // ── a restriction on the part sizes is actually respected ───────────────
    {
        dsl::IP::OddPartition odd;
        odd(200);
        bool all_odd = (odd.n() == 200);
        for (auto part : odd.AsMultiset()) all_odd &= (part % 2 == 1);
        check(all_odd, "OddPartition uses only odd parts, and still sums to n");

        dsl::IP::EvenPartition even;
        even(200);
        bool all_even = (even.n() == 200);
        for (auto part : even.AsMultiset()) all_even &= (part % 2 == 0);
        check(all_even, "EvenPartition uses only even parts, and still sums to n");
    }

    // ── the regression: every partition of 4 must be reachable ──────────────
    {
        // 4 / 3+1 / 2+2 / 2+1+1 / 1+1+1+1 -- five of them, each 1/5 of the
        // mass. {4} is the one the previous implementation could never produce.
        dsl::IP::UnrestrictedPartition ip;
        std::map<std::multiset<ull, std::greater<ull> >, int> seen;
        const int draws = 4000;
        for (int t = 0; t < draws; ++t) {
            ip.RejectionSampling(4);
            ++seen[ip.AsMultiset()];
        }
        check(seen.size() == 5, "all five partitions of 4 occur");

        std::multiset<ull, std::greater<ull> > single;
        single.insert(4);
        const int got = seen[single];
        // 1/5 of 4000 is 800; a tenth of that would still mean suppression
        check(got > 400,
              "the single part {4} occurs about its share (" +
              std::to_string(got) + " of " + std::to_string(draws) +
              ", uniform ~" + std::to_string(draws / 5) + ")");
    }

    std::cout << (failures ? "FAILED" : "all passed") << "\n";
    return failures ? 1 : 0;
}
