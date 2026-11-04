# desalvo_standard_library

This is my C++11 library of code that I can't live without.  I've implemented
most of the modern C++11 paradigms, along with curiously recurring template
patterns, and the documentation is extensive (though not 100% complete).

Header-only.  `dsl.h` pulls in the stable headers; every header also declares
the short alias `dsl` for `desalvo_standard_library`.

```cpp
#include "dsl.h"
namespace dsl = desalvo_standard_library;
```

## Random integer partitions

`integer_partition.h` generates uniformly random integer partitions of a given
size, optionally restricted to a set of allowed part sizes.  It absorbs the
standalone **IntegerPartitionClass** repository (2026-11-04), which is now
retired — this is the only copy.

```cpp
#include "integer_partition.h"
typedef unsigned long long ull;

ull n = 100;
IP::UnrestrictedPartition ip;

ip(n);                             // a uniform partition of exactly n
ip.RejectionSampling(n);           // the textbook method
ip.PDCDeterministicSecondHalf(n);  // probabilistic divide-and-conquer, faster
ip.RandomSize(n);                  // faster still, size only approximately n

std::cout << ip << "\n";           // the parts
ip.Ferrer();                       // a Ferrers diagram
auto parts = ip.AsMultiset();      // parts in descending order
```

PDC deterministic second half is the method from *Improving probabilistic
divide-and-conquer: deterministic second half* (DeSalvo, arXiv).

Restrictions are policy classes with `operator()(IndexType i)` returning the
i-th allowed part size.  `IP::Unrestricted`, `Even`, `Odd`, `Triangular` and
`JmodM` are supplied; anything with that call signature works:

```cpp
struct PerfectCubes { ull operator()(ull i) { return i*i*i; } };
IP::IntegerPartition<PerfectCubes> cubes;
cubes(1000);                       // a partition of 1000 into cubes
```

Tests: `unit_tests/integer_partition_tests.cpp`, which builds and runs on its
own:

```
cd unit_tests && g++ -std=c++11 -O2 -iquote .. -o ip_tests integer_partition_tests.cpp && ./ip_tests
```

## The asymmetric exclusion process

`asep.h` runs particles on a graph: each jumps at its own exponential times,
and an exclusion function decides which proposed moves are legal.  It absorbs
the standalone **ASEP** repository (2026-11-04), which is now retired — the
`Graph` and `Particle` it runs on had already been brought here in 2020 as
`polymorphic/graph.h` and `polymorphic/particle.h`; this was the piece left
outside.

```cpp
#include "asep.h"
#include "polymorphic/graph.h"
#include "polymorphic/particle.h"

namespace dsl = desalvo_standard_library;
typedef dsl::polymorphic::Node Position;
typedef dsl::polymorphic::Particle<dsl::polymorphic::Graph, Position> Particle;

// the classical rule: a move is blocked if somebody is already there
bool exclusion(const Position* proposed, std::vector<Particle*>& particles) {
    for (auto& p : particles)
        if (p->get_position() == proposed) return true;
    return false;
}

std::default_random_engine generator(seed);
auto graph = dsl::polymorphic::
    random_weighted_complete_directed_graph_with_source_sink(100, generator);

dsl::ASEP::ASEP<dsl::polymorphic::Graph, Particle, dsl::timer<> >
    asep(&graph, 20, exclusion, generator);

asep.print(std::cout);     // initial states
asep.run(10000);
asep.print(std::cout);     // final states
```

`ASEP` is templated on `Universe` and `Particle`, so it does not include the
polymorphic graph itself — bring your own, or those.  `Universe` needs
`initial_state()` and `neighbors_and_weights()`; `Particle` needs
`jump_times`, `update` and `print`.

Graphs to run it on, all in `polymorphic/graph.h`: `complete_graph`,
`random_graph`, `random_weighted_complete_directed_graph`,
`..._with_source_sink`, `random_weighted_line_with_source_sink`, and
`graph_from_file` (incidence matrix; see `polymorphic/input/graph.txt`).

**Note on what exclusion promises.**  Every particle is born at
`initial_state()`, so they all start on the same node and the source is
expected to hold a queue.  Exclusion stops a particle moving *onto* an
occupied node; it does not mean no two particles are ever co-located.

Tests: `unit_tests/asep_tests.cpp`, standalone like the integer partition
ones.

## Known rough edges

Honest notes so nobody loses an afternoon to these:

- **`time.h` shadows the system header.**  Compiling with `-I .` puts this
  directory ahead of the system includes, so anything that reaches `<ctime>`
  fails.  Use **`-iquote .`** instead, which only affects `"quoted"` includes.
- **`dsl_usings.h` does not compile** — 26 errors in `sequence.h` and
  `permutation.h` (a `finite_sequence` member `v` that isn't there, and a use
  of `std::atomic` without including `<atomic>`).  `dsl.h` is clean; prefer it.
- **`unit_tests/unit_tests.cpp` does not compile.**  Commit 041b878 (2020)
  moved `polymorphic_graph.h` to `polymorphic/graph.h` and renamed the classes
  without updating the test.  The vendored Catch2 is also too old for current
  glibc (`catch.hpp` calls `sysconf` in a constexpr context).  The integer
  partition and ASEP tests avoid both by standing alone.
- **`polymorphic::Particle::print` is not `const`**, so the `const Particle*`
  values that `ASEP::get_positions()` hands back cannot be printed through.
  Use `get_position()`, which is `const`.
