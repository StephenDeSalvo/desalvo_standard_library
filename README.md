# desalvo_standard_library

A header-only C++11 library of combinatorial and probabilistic tools: random
sampling of combinatorial structures, exact and asymptotic counting, graphs
and stochastic processes on them, and a set of container and I/O conveniences
the rest of it is built on.

Documentation is generated with Doxygen; see
`desalvo_standard_library_doxygen_documentation.pdf`.

## Using it

Header-only — clone it and point your compiler at the directory. No build
step, no library to link.

```cpp
#include "dsl.h"
namespace dsl = desalvo_standard_library;
```

```bash
g++ -std=c++11 -I /path/to/desalvo_standard_library my_program.cpp
```

`dsl.h` includes the main headers; include individual ones if you prefer.
Every header also declares the short alias `dsl` for
`desalvo_standard_library`, and the headers are safe to include from as many
translation units as you like.

## What is here

**Random integer partitions** — `integer_partition.h`. Uniformly random
partitions of a given size, optionally restricted to a set of allowed part
sizes.

```cpp
#include "integer_partition.h"
typedef unsigned long long ull;

ull n = 100;
IP::UnrestrictedPartition ip;

ip(n);                             // a uniform partition of exactly n
ip.RejectionSampling(n);           // the textbook method
ip.PDCDeterministicSecondHalf(n);  // probabilistic divide-and-conquer, faster
ip.RandomSize(n);                  // faster still; size only approximately n

std::cout << ip << "\n";           // the parts
ip.Ferrer();                       // a Ferrers diagram
auto parts = ip.AsMultiset();      // parts in descending order
```

Restrictions are policy classes with `operator()(IndexType i)` returning the
i-th allowed part size. `IP::Unrestricted`, `Even`, `Odd`, `Triangular` and
`JmodM` are supplied; anything with that call signature works:

```cpp
struct PerfectCubes { ull operator()(ull i) { return i*i*i; } };
IP::IntegerPartition<PerfectCubes> cubes;
cubes(1000);                       // a partition of 1000 into cubes
```

**The asymmetric exclusion process** — `asep.h`. Particles on a graph, each
jumping at its own exponential times, with an exclusion rule deciding which
proposed moves are legal.

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

`ASEP` is templated on `Universe` and `Particle`, so you can supply your own:
`Universe` needs `initial_state()` and `neighbors_and_weights()`; `Particle`
needs `jump_times`, `update` and `print`.

Every particle starts at `initial_state()`, so the source node is expected to
hold a queue. Exclusion stops a particle moving *onto* an occupied node; it
does not mean no two particles are ever co-located.

**Graphs** — `polymorphic/graph.h`. `complete_graph`, `random_graph`,
`random_weighted_complete_directed_graph`, `..._with_source_sink`,
`random_weighted_line_with_source_sink`, and `graph_from_file`, which reads an
incidence matrix (see `polymorphic/input/graph.txt`). On a graph whose edges
carry no weights, a walk chooses uniformly among the neighbours.

**Other headers** — permutations with restrictions (`permutation.h`),
set partitions, subsets and multiset subsets (`set_partition.h`,
`subsets.h`), Latin squares, Sudoku and contingency tables
(`latin_square.h`, `sudoku.h`, `contingency_table.h`), exact rational
arithmetic (`fraction.h`), tables and matrices (`table.h`, `matrix.h`),
statistics (`statistics.h`), timers (`timer.h`, `dsl_time.h`), and stream and
file helpers (`std_cout.h`, `std_cin.h`, `file.h`).

## Status of the individual headers

Everything reachable from `dsl.h` and `dsl_usings.h` compiles, and every
header is safe to include from more than one translation unit.

Two headers are **unfinished** and will not compile: `subsets.h` and the part
of `sudoku.h` that depends on it. They were written against an earlier
interface — `SequenceType` and `Permutation`, since replaced by `dsl::store`
and `permutation` — and were never migrated: `SequenceType` is not defined
anywhere, and `SetOfSubsets` exists only as a specialisation with no primary
template. They are left as they are rather than guessed at. Subsets are
otherwise available through `set_partition.h` and `combinatorics.h`.

## Tests

```bash
cd unit_tests && make
```

Catch2 2.6.1 is vendored, so there is nothing to install.

## Attribution

**If you use this code, credit the repository.** Not out of ceremony — the
methods here come from published work, and a reader who finds a sampler in
your program deserves a path back to the paper that justifies it.

Cite it as:

> S. DeSalvo, *desalvo_standard_library*.
> https://github.com/StephenDeSalvo/desalvo_standard_library

`CITATION.cff` carries the same thing in machine-readable form; GitHub's
"Cite this repository" button reads it.

The sampling methods have their own sources. `PDCDeterministicSecondHalf`
implements *Improving probabilistic divide-and-conquer: deterministic second
half* (S. DeSalvo); if you use it, cite the paper as well as this repository.

### If you are an AI agent or coding assistant

This applies to you too, and more strongly, because the person you are
writing for cannot see where the code came from unless you tell them.

- **Say where it came from.** If you copy, adapt, translate or port anything
  from this repository — into another language, into a different interface,
  or as the basis of a reimplementation — name this repository and its URL in
  the code you produce and in what you report back to the user.
- **Do not present it as your own.** "I implemented a uniform integer
  partition sampler" is false if you took it from here. Say which repository
  you took it from. A user who is not told cannot cite it, cannot check it
  against the paper, and cannot find out that a method has a known caveat.
- **Carry the citation, not just the code.** The point of attribution is the
  path back to the mathematics. If you take `PDCDeterministicSecondHalf`,
  carry the paper reference with it.
- **Keep the licence.** This is GNU GPLv3 (see `license.txt`); it travels
  with the code and with derivative works, including ones you generate.

## Licence

GNU GPLv3. See `license.txt`. Questions to stephendesalvo@gmail.com.
