/** @file asep_tests.cpp
  @author Stephen DeSalvo
  @date November, 2026
  @brief Tests for asep.h.

  Standalone for the same reason as integer_partition_tests.cpp: the Catch2
  vendored here is too old for current glibc, and unit_tests.cpp has not
  compiled since 2020.

      g++ -std=c++11 -O2 -iquote .. -o asep_tests asep_tests.cpp && ./asep_tests

  What these guard against. Absorbing the standalone ASEP repository turned up
  two bugs that had kept the process from running at all on a current
  compiler:

    - asep.h used std::function, std::sort and std::default_random_engine
      without including <functional>, <algorithm> or <random>. It had only
      ever compiled through someone else's transitive includes.
    - polymorphic/particle.h declared std::discrete_distribution<double>.
      That distribution returns an index, so its type argument must be
      integral; the standard has never allowed double. Older libstdc++ let it
      pass, gcc 15 rejects it, and update() could not be instantiated -- which
      is the one function that moves a particle.

  Both were invisible without a test that actually ran the process, so these
  run it.
 */

#include <iostream>
#include <random>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#include "../asep.h"
#include "../polymorphic/graph.h"
#include "../polymorphic/particle.h"

namespace dsl = desalvo_standard_library;

typedef dsl::polymorphic::Node Position;
typedef dsl::polymorphic::Particle<dsl::polymorphic::Graph, Position> Particle;

static int failures = 0;

static void check(bool ok, const std::string& what) {
    std::cout << (ok ? "  ok    " : "  FAIL  ") << what << "\n";
    if (!ok) ++failures;
}

/** The classical exclusion rule: a move is blocked if somebody is already there. */
static bool exclusion(const Position* proposed, std::vector<Particle*>& particles) {
    for (auto& p : particles)
        if (p->get_position() == proposed) return true;
    return false;
}

/** Nothing is ever blocked -- used to tell "exclusion works" apart from
    "nothing moved at all". */
static bool no_exclusion(const Position*, std::vector<Particle*>&) {
    return false;
}

int main() {
    std::cout << "asep.h\n";

    // ── it runs, and the particles actually move ────────────────────────────
    {
        std::default_random_engine gen(12345);
        auto graph = dsl::polymorphic::
            random_weighted_complete_directed_graph_with_source_sink(20, gen);
        dsl::ASEP::ASEP<dsl::polymorphic::Graph, Particle, dsl::timer<> >
            asep(&graph, 5, exclusion, gen);

        std::ostringstream before;
        asep.print(before);
        asep.run(100);
        std::ostringstream after;
        asep.print(after);

        check(before.str() != after.str(),
              "run() moves the particles off their initial state");
        check(asep.get_positions().size() == 5,
              "the particle count is preserved (5 in, 5 out)");
    }

    // ── the exclusion rule is respected away from the source ───────────────
    {
        // Every particle is BORN at universe->initial_state(), so they all
        // share that node to begin with and exclusion cannot be read as "no
        // two particles are ever co-located" -- the source is a birth state
        // and is expected to hold a queue. What exclusion does promise is
        // that a particle never moves ONTO an occupied node, so every node
        // other than the source holds at most one.
        //
        // A line with a source and a sink is the tight case: particles queue
        // up, so a broken rule shows as a collision almost at once.
        bool violated = false;
        int trials_that_moved = 0;
        for (int trial = 0; trial < 20; ++trial) {
            std::default_random_engine gen(trial + 1);
            auto graph = dsl::polymorphic::
                random_weighted_line_with_source_sink(10, gen);
            const Position* source = graph.initial_state();
            dsl::ASEP::ASEP<dsl::polymorphic::Graph, Particle, dsl::timer<> >
                asep(&graph, 4, exclusion, gen);
            asep.run(50);

            std::map<const Position*, int> occupancy;
            for (auto* p : asep.get_positions())
                ++occupancy[p->get_position()];

            for (auto& node_count : occupancy) {
                if (node_count.first == source) continue;
                if (node_count.second > 1) violated = true;
                ++trials_that_moved;
            }
        }
        check(!violated,
              "exclusion holds: no node but the source holds two particles");
        check(trials_that_moved > 0,
              "particles did leave the source, so the test above had work to do");
    }

    // ── a timer with a different rate still drives the process ──────────────
    {
        std::default_random_engine gen(99);
        auto graph = dsl::polymorphic::
            random_weighted_complete_directed_graph_with_source_sink(12, gen);
        typedef dsl::timer<std::exponential_distribution<double> > Timer;
        dsl::ASEP::ASEP<dsl::polymorphic::Graph, Particle, Timer>
            asep(&graph, 3, no_exclusion, gen);
        asep.run(200);
        check(asep.get_positions().size() == 3,
              "an explicit Timer type instantiates and runs");
    }

    // ── the documented top-level spelling still compiles ────────────────────
    {
        std::default_random_engine gen(7);
        auto graph = dsl::polymorphic::complete_graph(6);
        // ASEP:: rather than dsl::ASEP:: -- how the retired repository's
        // demos spell it, kept working by the alias in asep.h
        ASEP::ASEP<dsl::polymorphic::Graph, Particle, dsl::timer<> >
            asep(&graph, 2, exclusion, gen);
        check(asep.get_positions().size() == 2,
              "the top-level ASEP:: alias still resolves");
    }

    std::cout << (failures ? "FAILED" : "all passed") << "\n";
    return failures ? 1 : 0;
}
