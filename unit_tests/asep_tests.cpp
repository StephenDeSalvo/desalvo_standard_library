/** @file asep_tests.cpp
  @brief Tests for asep.h.

  These run the process rather than only instantiating it: the templates can
  compile and still be unable to move a particle, which is the one thing the
  process is for.
 */

#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "../asep.h"
#include "../polymorphic/graph.h"
#include "../polymorphic/particle.h"
#include "Catch2/catch.hpp"

namespace dsl = desalvo_standard_library;

typedef dsl::polymorphic::Node Position;
typedef dsl::polymorphic::Particle<dsl::polymorphic::Graph, Position> Particle;
typedef dsl::ASEP::ASEP<dsl::polymorphic::Graph, Particle, dsl::timer<> > Process;


/** The classical rule: a move is blocked if somebody is already there. */
static bool exclusion(const Position* proposed, std::vector<Particle*>& particles) {
    for (auto& p : particles)
        if (p->get_position() == proposed) return true;
    return false;
}

/** Nothing is ever blocked. Used to separate "exclusion works" from
    "nothing moved at all". */
static bool no_exclusion(const Position*, std::vector<Particle*>&) {
    return false;
}


TEST_CASE( "Running the process moves the particles", "[asep]" ) {
    std::default_random_engine gen(12345);
    auto graph = dsl::polymorphic::
        random_weighted_complete_directed_graph_with_source_sink(20, gen);
    Process asep(&graph, 5, exclusion, gen);

    std::ostringstream before;
    asep.print(before);

    asep.run(100);

    std::ostringstream after;
    asep.print(after);

    REQUIRE( before.str() != after.str() );
    REQUIRE( asep.get_positions().size() == 5 );
}


TEST_CASE( "Exclusion keeps one particle per node away from the source", "[asep]" ) {
    // Every particle is born at initial_state(), so they all start on the same
    // node: the source is a birth state and is expected to hold a queue. What
    // exclusion promises is narrower -- a particle never moves ONTO an
    // occupied node -- so every node but the source holds at most one.
    //
    // A line with a source and a sink is the tight case: particles queue up,
    // so a broken rule shows as a collision almost at once.
    int nodes_checked = 0;

    for (int trial = 0; trial < 20; ++trial) {
        std::default_random_engine gen(trial + 1);
        auto graph = dsl::polymorphic::random_weighted_line_with_source_sink(10, gen);
        const Position* source = graph.initial_state();
        Process asep(&graph, 4, exclusion, gen);

        asep.run(50);

        std::map<const Position*, int> occupancy;
        for (auto* p : asep.get_positions())
            ++occupancy[p->get_position()];

        for (auto& node_and_count : occupancy) {
            if (node_and_count.first == source) continue;
            REQUIRE( node_and_count.second == 1 );
            ++nodes_checked;
        }
    }

    // the test above is vacuous if nobody ever left the source
    REQUIRE( nodes_checked > 0 );
}


TEST_CASE( "An explicit Timer type instantiates and runs", "[asep]" ) {
    std::default_random_engine gen(99);
    auto graph = dsl::polymorphic::
        random_weighted_complete_directed_graph_with_source_sink(12, gen);
    typedef dsl::timer<std::exponential_distribution<double> > Timer;

    dsl::ASEP::ASEP<dsl::polymorphic::Graph, Particle, Timer>
        asep(&graph, 3, no_exclusion, gen);
    asep.run(200);

    REQUIRE( asep.get_positions().size() == 3 );
}


TEST_CASE( "The process runs on an unweighted graph", "[asep]" ) {
    // complete_graph and random_graph build edges that carry no weight, so
    // every transition weight here is zero. No preference among the
    // neighbours means a uniform choice: the ordinary random walk.
    std::default_random_engine gen(7);
    auto graph = dsl::polymorphic::complete_graph(6);
    Process asep(&graph, 2, exclusion, gen);

    asep.run(20);

    REQUIRE( asep.get_positions().size() == 2 );
    for (const Particle* p : asep.get_positions())
        REQUIRE( p->get_position() != nullptr );
}


TEST_CASE( "Positions can be read and printed after a run", "[asep]" ) {
    std::default_random_engine gen(7);
    auto graph = dsl::polymorphic::
        random_weighted_complete_directed_graph_with_source_sink(8, gen);
    Process asep(&graph, 2, exclusion, gen);

    asep.run(20);

    std::ostringstream out;
    for (const Particle* p : asep.get_positions()) {
        REQUIRE( p->get_position() != nullptr );
        p->print(out);          // requires print() to be const
    }
    REQUIRE_FALSE( out.str().empty() );
}
