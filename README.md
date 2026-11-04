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
  partition tests above avoid both by standing alone.
