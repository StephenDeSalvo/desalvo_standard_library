/** @file dsl_usings.h
 @brief includes standard files in desalvo_standard_library and includes common aliases and keywords
 
 This file includes the standard files in the desalvo_standard_library, defines the alias dsl for short, and it defines many other keywords.  Use dsl_usings.h until you are comfortable with the extended template syntax.
 
 
TODO: example of 
 @code
 // three different ways to create an input file.
 desalvo_standard_library::file<desalvo_standard_library::file_type::input> file("text.txt");
 dsl::file<dsl::file_type::input> file2("text.txt");
 dsl::file<input> file3("text.txt");
 @endcode
 */

#ifndef DeSalvo_Standard_Library_std_h
#define DeSalvo_Standard_Library_std_h

#include "std_cout.h"       // documented
#include "file.h"           // 
#include "numerical.h"
#include "dsl_algorithm.h" // apply algorithm to an entire container, just a wrapper.
#include "statistics.h"
#include "shrinking_set.h"
#include "sequence.h"
#include "permutation.h"
#include "time.h"
#include "combinatorics.h"
#include "table.h"



namespace dsl = desalvo_standard_library;

using dsl::file_type::output;
using dsl::file_type::input;
using dsl::file_type::console;

using dsl::store::bidirectional;
using dsl::store::forward;
using dsl::store::random_access;



#endif
