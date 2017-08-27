/** @mainpage DeSalvo Standard Library Documentation
    @author Stephen DeSalvo
    @date July 2015
    @brief A core library for C++ applications
 
    The namespace name is desalvo_standard_library, in honor to the Standard Library (NOT STL) but expanded to include my versions of commonly used functionality.  It will be abbrev. to dsl in the future, which is also an alias I use in my files and suggest for common use.
 
    When using the "dsl_usings.h" header, the following keywords are added:
 
 1.  dsl -- shorter way to refer to the library functions.
 2.  output  --  this is from dsl::file_type::output
 3.  input  --  this is from dsl::file_type::input
 4.  console  --  this is from dsl::file_type::console
 
 @section Introduction
 
 I found it a bit of a pain to code up many of the ubiquitous algorithms in combinatorics and probability.  In particular, in combinatorial probability one often wishes to generate all objects of a given weight for small weights, or randomly sample objects of a large weight, in order to search for macroscopic properties.
 
 Example: Counting. the log-concavity of the integer partition function is something which can be observed for the first billion or so values, excluding the first 25 (only works for even values).  One should like to exhaust all small cases first before attempting to prove such a result; see http://link.springer.com/article/10.1007%2Fs11139-014-9599-y#page-1 or http://arxiv.org/abs/1310.7982 .
 
 Example: Patterns.  Suppose we wish to explore bijections between various combinatorial objects, e.g., integer partitions into distinct parts vs integer partitions into odd parts.  We would first like to be able to count them to make sure that there are indeed the same number of objects in each set.  Then, to search for a combinatorial bijection, it would be preferable to generate a comprehensive list for small weights and attempt to find a pattern, which can be proven to continue for all n.
 
 Example: Random sampling.  For combinatorial objects with larger weight, a complete enumeration is infeasible, and so we often wish to sample, uniformly at random, such an object.  Efficient random samplers are often difficult to obtain; even if one has a polynomial time algorithm, a O(n^2) vs O(n) time algorithm makes a large difference in practice, as well as the memory constraints.
 
 Conceptual Solution: Integer partitions are one example of a combinatorial structure for which one might wish to look at the number of such objects, list them, list a subset, randomly generate a subset, etc.  It turns out that they are a typical example of a "decomposable" combinatorial structure, see http://arxiv.org/abs/1308.3279, and one can actually treat the other such structures in generality.
 
 In fact, the code that one writes is essentially the same in all cases, hence this library was designed to attempt to minimize the amount of duplicate code one would need to write in order to facilitate the above tasks as easily as possible.
 
 C++ Coding Solution: Our main tool is curiously recurring template pattern (CRTP), which allows us to write code in a base class, templated so that it calls the functions of the inherited class.  The effect is similar to much of mathematical theory, e.g., vector spaces: one starts by defining a list of rules that apply to all objects in a vector space X over a field F.  As long as F and X satisfy certain properties, then all theorems assuming that X is a vector space apply.  In this case, X acts like a base class, which we have provided, and F is the class specified by the programmer.  As long as F satisfies certain properties, then X will utilize those properties, from which others can be derived.
 
 @section overview Overview of libraries
 
 The libraries are divided into several categories.
 
 1.  Fundamental
 2.  CRTP base class
 3.  CRTP derived class
 4.  Utility
 
 @subsection fundamental Fundamental
 
 These are files like numerical.h, statistics.h, file.h.  These classes provide basic functionality which should be part of any general-purpose mathematical library of functionality.
 
 @subsection crtpb CRTP base class
 
 These are files like sequence.h, which provide a core set of functionality so that when a class is derived from them, they obtain all of the desired functionality.
 
 @subsection crtpd CRTP derived class

 These are files like permutation.h, which contains classes for permutations which inherit from a CRTP base class and provide only a few necessary functions in order to unlock functionality like complete enumeration or random generation.
 
 @subsection utility Utility
 
 Finally, there are files like std_cout.h, dsl_algorithm.h, dsl.h, dsl_usings.h, which are mostly wrappers around existing functionality.  They are meant to make working with collections of objects easier, for example allowing for Matlab-like syntax in a C++ environment.
 
 
*/


/** @namespace desalvo_standard_library
    @brief think of this namespace like std or boost, I typically use dsl as an alias.
 
    The core set of functionality is contained in this namespace.  It consists of the content in the following files: \n
 
    1.  numerical.h
    2.  statistics.h
    3.  std_cout.h
    4.  file.h
    5.  shrinking_set.h
    6.  sequence.h
    7.  permutation.h
    8.  dsl_algorithm.h
*/


/** @file documentation.h
    @author Stephen DeSalvo
    @date July, 2015
    @brief Contains documentation for files
 
    This is a separate file in order to keep the header files less cluttered, and it gives me the freedom to include as many examples as I wish without worrying about the length of the file.
 
 */

/** @file numerical.h
 @Author Stephen DeSalvo
 @date December, 2014
 @brief Deterministic Algorithms for Numerical Operations
 
 Contains algorithms and various functions for quick numerical evaluations and common collections of numerical values.
 
 
 /// TODO: UPDATE CODE EXAMPLES USING dsl
 
 @code
 // Easier to output std containers
 #include "DeSalvoOutputLibrary.h"
 
 #include "DeSalvoNumericalLibrary.h"
 namespace dsl = DeSalvoNumericalLibrary;
 @endcode
 
 Then in main preface all function/class calls with dsl::
 
 @code
 // Generate a vector with entries 1,...,10 of type size_t
 auto x = dsl::range(10);
 @endcode
 
 
 
 @code
 
 // Overloads of operator<< for standard library containers
 #include "std_cout.h"
 
 #include "numerical.h"
 
 int main(int argc, const char * argv[])
 {
 // Examples of how to use function range(...)
 
 // Generate a vector with entries 1,...,10 of type size_t
 auto x = dsl::range(10);
 std::cout <<"range(10): "<< x << std::endl;
 
 // Generate a vector with entries 1,...,10 of type int
 std::vector<int> vec = dsl::range(10);
 std::cout << "range(10): "<< vec << std::endl;
 
 // Generate a vector with entries a,...,a+10-1 of type int
 std::vector<int> vec2 = dsl::range(10, 5);
 std::cout << "range(10,5): "<<vec2 << std::endl;
 
 // Generate a vector with entries a,...,a+10-1 of type int
 std::vector<double> vec3 = dsl::range<double>(10, -4.5);
 std::cout << "range<double>(10,-4.5): "<<vec3 << std::endl;
 
 // Generate a list with 10 entries starting at -5.43, incrementing by 1.
 std::list<double> lst = dsl::range<double, std::list<double>>(10, -5.43);
 std::cout <<"range<double,list<double>>(10,-5.43): "<< lst << std::endl;
 
 // Examples of how to use sort, only works with random access iterators for now
 std::vector<int> v {1, -1, 23, -756, 222, 5, 4, -3, 77, 18};
 std::cout <<"v = "<< v << std::endl;
 
 // Create new vector with elements sorted
 auto v_sorted = dsl::sort(v);
 std::cout <<"sort(v): "<< v_sorted << std::endl;
 
 // In place sorting of the container, changes the elements rather than creating new object
 auto v_copy = v;
 dsl::sort_in_place(v_copy);
 std::cout <<"sort_in_place(v_copy): "<< v_copy << std::endl;
 
 std::vector<double> probability_masses {0.1, 0.2, 0.3, 0.2, 0.2};
 
 // Find the cumulative distribution using point probability masses
 auto cumulative_distribution = dsl::partial_sum(probability_masses);
 std::cout << "partial_sum({.1,.2,.3,.2,.2}): "<<cumulative_distribution << std::endl;
 
 // In place replace with cumulative distribution.
 auto probs = probability_masses;
 dsl::partial_sum_in_place(probs);
 std::cout <<"partial_sum_in_place(...):" << probs << std::endl;
 
 
 // array of 10 bools, first 3 are true
 std::vector<bool> switches = dsl::binary_row(10, 3);
 std::cout << "binary_row(10,3): "<<switches << std::endl;
 
 auto s =dsl::binary_row(10,4,std::string("x"));
 std::cout << "binary_row(10,4,string(\"x\")): "<<s <<std::endl;
 
 auto rev = dsl::reverse(switches);
 std::cout<<"reverse(binary_row(10,3)): "<<rev << std::endl;
 
 std::cout <<"reverse(range(10.,-0.5)): "<< dsl::reverse(dsl::range(10.,-0.5)) << std::endl;
 
 auto rng = dsl::range(11,-5);
 dsl::reverse_in_place(rng);
 std::cout <<"reverse_in_place(range(11,-5)): "<< rng << std::endl;
 
 // One list is a vector<int> the other is a list<double>, can mix and match types
 auto list1 = dsl::range(11);
 auto list2 = dsl::range<double, std::list<double>>(11,-5);
 std::cout<<"Print two lists of different types side-by-side one element per row"<<std::endl;
 dsl::print_side_by_side(list1, list2,",",";");
 std::cout<<std::endl;
 
 std::cout << "binomial(10,7): "<< dsl::binomial(10,7) << std::endl;
 
 // choose2(n) == binomial(n,2)
 std::cout << "choose2(10): "<< dsl::choose2(10) << std::endl;
 
 // choose3(n) == binomial(n,3)
 std::cout << "choose3(10): "<< dsl::choose3(10) << std::endl;
 
 // choose4(n) == binomial(n,4)
 std::cout << "choose4(10): "<< dsl::choose4(10) << std::endl;
 
 auto nums = dsl::range(9);
 std::cout << "sum of first 9 integers: "<< dsl::sum_of_powers(std::begin(nums), std::end(nums), 0, 1)<<std::endl;
 std::cout << "sum_of_powers first 9 squared integers: "<< dsl::sum_of_powers(std::begin(nums), std::end(nums), 0, 2)<<std::endl;
 std::cout << "sum_of_powers first 9 cubed integers: "<< dsl::sum_of_powers(std::begin(nums), std::end(nums), 0, 3)<<std::endl;
 std::cout << "sum_of_powers first 9 7th power integers: "<< dsl::sum_of_powers(std::begin(nums), std::end(nums), 0, 7)<<std::endl;
 
 std::cout<<"Select any 10 out of 10 things, order matters 10!: "<< dsl::factorial(10) << std::endl;
 std::cout<<"Select any 4 out of 10 things, order matters (10)_4: "<< dsl::nfallingk(10,4) << std::endl;
 std::cout<<"Select any 4 out of 10 things, unordered 10 choose 4: "<< dsl::binomial(10,4) << std::endl;
 
 std::cout<<"Select any 30 out of 30 things, order matters 30!: "<< dsl::ffactorial(30) << std::endl;
 std::cout<<"Select any 17 out of 30 things, order matters (30)_17: "<< dsl::fnfallingk(30,17) << std::endl;
 std::cout<<"Select any 24 out of 30 things, unordered 30 choose 24: "<< dsl::fbinomial(30,24) << std::endl;
 
 
 // All permutations of a collection.
 auto perms =dsl::permutations( std::vector<std::string>({"John", "Bob", "Sally"}));
 std::cout<<"All rearrangements of {John, Bob, Sally}:\n"<<perms<<std::endl;
 
 
 auto v = dsl::int_to_digits(10);
 cout << v << endl;
 
 std::vector<int> s = {1,2,3,4,5};
 cout << dsl::digits_to_int(s) << endl;
 
 // Does not work with strict initializer list.
 std::cout << dsl::has_unique_elements( std::vector<int>({1,2,3,4,5,6}) ) << std::endl;
 std::cout << dsl::has_unique_elements( std::vector<int>({1,4,3,4,5,6}) ) << std::endl;
 std::cout << dsl::has_unique_elements( std::vector<int>({1,3,3,4,5,6}) ) << std::endl;
 std::cout << dsl::has_unique_elements( std::vector<int>({1,0,3,-0,5,6}) ) << std::endl;
 std::cout << dsl::has_unique_elements( std::vector<int>({1,4,3,-1,5,6}) ) << std::endl;
 
 
 return 0;
 }
 
 @endcode
 
 The output should be:
 @code
 range(10): {1,2,3,4,5,6,7,8,9,10}
 range(10): {1,2,3,4,5,6,7,8,9,10}
 range(10,5): {5,6,7,8,9,10,11,12,13,14}
 range<double>(10,-4.5): {-4.5,-3.5,-2.5,-1.5,-0.5,0.5,1.5,2.5,3.5,4.5}
 range<double,list<double>>(10,-5.43): {-5.43,-4.43,-3.43,-2.43,-1.43,-0.43,0.57,1.57,2.57,3.57}
 v = {1,-1,23,-756,222,5,4,-3,77,18}
 sort(v): {-756,-3,-1,1,4,5,18,23,77,222}
 sort_in_place(v_copy): {-756,-3,-1,1,4,5,18,23,77,222}
 partial_sum({.1,.2,.3,.2,.2}): {0.1,0.3,0.6,0.8,1}
 partial_sum_in_place(...):{0.1,0.3,0.6,0.8,1}
 binary_row(10,3): {1,1,1,0,0,0,0,0,0,0}
 binary_row(10,4,string("x")): {x,x,x,x,,,,,,}
 reverse(binary_row(10,3)): {0,0,0,0,0,0,0,1,1,1}
 reverse(range(10.,-0.5)): {8.5,7.5,6.5,5.5,4.5,3.5,2.5,1.5,0.5,-0.5}
 reverse_in_place(range(11,-5)): {5,4,3,2,1,0,-1,-2,-3,-4,-5}
 Print two lists of different types side-by-side one element per row
 1,-5;2,-4;3,-3;4,-2;5,-1;6,0;7,1;8,2;9,3;10,4;11,5;
 binomial(10,7): 120
 choose2(10): 45
 choose3(10): 120
 choose4(10): 210
 sum of first 9 integers: 45
 sum_of_powers first 9 squared integers: 285
 sum_of_powers first 9 cubed integers: 2025
 sum_of_powers first 9 7th power integers: 8080425
 Select any 10 out of 10 things, order matters 10!: 3628800
 Select any 4 out of 10 things, order matters (10)_4: 5040
 Select any 4 out of 10 things, unordered 10 choose 4: 210
 Select any 30 out of 30 things, order matters 30!: 9.68217e+18
 Select any 17 out of 30 things, order matters (30)_17: 3.54097e+18
 Select any 24 out of 30 things, unordered 30 choose 24: 593775
 All rearrangements of {John, Bob, Sally}:
 {{Bob,Sally,John},{Sally,Bob,John},{Sally,John,Bob},{John,Sally,Bob},{Bob,John,Sally},{John,Bob,Sally}}
 @endcode
 
 */












/** @class permutation
 @brief container for restrictions of the form {none, fixed_point_free, by_pairs, by_function}
 
 Storage can be either random access, bidirectional, forward. \n
 
 Random access is useful for small n, since at present it loops through all n! permutations much be stored in memory.  There are more efficient iteratations through restricted permutations, but they have not been implemented in this version.
 
 Example 1:
 @code
 const int n = 4;
 
 fixed_point_free_permutation<size_t> v(n);
 std::cout << v.size() << std::endl;
 
 // Print out the 5th fixed-point free permutation of 4
 std::cout << v[4] << std::endl;
 
 // Add up the first two elements of each permutation.
 for(auto& x : v) std::cout << x[0]+x[1] << ",";
 std::cout << std::endl;
 
 // Supplies a random access iterator, so can do pointer arithmetic
 //auto start = std::begin(v);
 //auto stop = std::end(v);
 //auto it = start + 3;
 
 // We can access each permutation using generic algorithms.
 std::for_each(std::begin(v), std::end(v), [](const std::vector<size_t>& v) {std::cout << v << std::endl;});
 
 // Requires a random access iterator, can search for particular elements.
 auto flag = std::binary_search(std::begin(v), std::end(v), std::vector<size_t>({4,3,2,1}));
 std::cout << flag << std::endl;
 @endcode
 
 
 Example 2:
 @code
 fixed_point_free_permutation_one<size_t> v(4);
 
 // Add up the first two elements of each permutation.
 //for(auto& x : v) std::cout << x[0]+x[1] << ",";
 //std::cout << std::endl;
 
 // Supplies a random access iterator, so can do pointer arithmetic
 //auto start = std::begin(v);
 //auto stop = std::end(v);
 //auto it = start + 3;
 
 auto t = std::end(v);
 
 --t;
 
 auto s = std::begin(v);
 
 --s;
 ++s;
 
 std::cout << *s << std::endl;
 
 // We can access each permutation using generic algorithms.
 std::for_each(std::begin(v), std::end(v), [](const std::vector<size_t>& v) {std::cout << v << std::endl;});
 
 // Requires a random access iterator, can search for particular elements.
 auto flag = std::binary_search(std::begin(v), std::end(v), std::vector<size_t>({4,3,2,1}));
 std::cout << flag << std::endl;
 
 std::cout << v.count() << std::endl;
 @endcode
 
 Example 3:
 @code
 
 #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
 
 
 
 
 int main(int argc, const char * argv[]) {
 
 dsl::permutation<dsl::store::forward, dsl::restrictions::fixed_point_free> p(6);
 
 dsl::permutation<dsl::store::forward, dsl::restrictions::none> p2(4);
 
 dsl::permutation<dsl::store::random_access, dsl::restrictions::by_pairs> p3(5, {{1,2},{2,4}});
 dsl::permutation<dsl::store::forward, dsl::restrictions::by_pairs> p4(10, {{1,2},{2,4}});
 p4.insert({{1,1},{2,2},{3,3},{4,4},{5,5},{2,1}});
 
 
 //auto start = std::begin(p);
 //auto stop = std::end(p);
 
 for(auto& x : p)
 std::cout << x << std::endl;
 
 for(auto& x : p2)
 std::cout << x << std::endl;
 
 
 for(auto& x : p3)
 std::cout << x << std::endl;
 
 std::cout << "\n\n";
 
 std::cout << p3.size() << std::endl;
 
 p3.insert({{1,1},{2,2},{3,3},{4,4},{5,5},{2,1}});
 
 for(auto& x : p3)
 std::cout << x << std::endl;
 
 std::cout << p3.size() << std::endl;
 
 p3.resize(6);
 
 for(auto& x : p3)
 std::cout << x << std::endl;
 
 std::cout << p3.size() << std::endl;
 
 auto start = std::begin(p4);
 
 for(size_t i=0;i<100;++i)
 std::cout << *start++ << std::endl;
 
 
 //auto start = std::begin(p3);
 //auto stop = std::end(p3);
 
 //std::cout << *start << std::endl;
 
 
 return 0;
 }
 @endcode
 
 Example 4:
 @code
 #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
 
 // avoiding 132
 bool avoids_132(std::vector<size_t>& v) {
 
 size_t n = v.size();
 
 // Look at all triplets of indices to see if pattern is violated.
 // O(n^3) algorithm.
 for(size_t i=0;i<n-2;++i)
 for(size_t j=i+1;j<n-1;++j)
 for(size_t k=j+1;k<n;++k)
 if( (v[i] < v[j] && v[i] < v[k] && v[j] > v[k]) )
 return true;
 
 return false;
 
 }
 
 // avoiding 132 consecutively
 bool avoids_132_consecutively(std::vector<size_t>& v) {
 
 size_t n = v.size();
 
 // Look at all consecutive triplets of indices to see if pattern is violated.
 // O(n) algorithm.
 for(size_t i=0;i<n-2;++i)
 if( (v[i] < v[i+1] && v[i] < v[i+2] && v[i+1] > v[i+2]) )
 return true;
 
 return false;
 
 }
 
 int main(int argc, const char * argv[]) {
 
 dsl::permutation<dsl::store::random_access, dsl::restrictions::by_function> p(6, avoids_132);
 dsl::permutation<dsl::store::random_access, dsl::restrictions::by_function> p_consecutive(6, avoids_132_consecutively);
 
 
 for(auto& x : p)
 std::cout << x << std::endl;
 
 std::cout << "\n\n";
 
 for(auto& x : p_consecutive)
 std::cout << x << std::endl;
 
 
 
 return 0;
 }
 
 @endcode
 
 Example 5:
 @code
 #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
 
 // avoiding 132
 bool avoids_132(std::vector<size_t>& v) {
 
 size_t n = v.size();
 
 // Look at all triplets of indices to see if pattern is violated.
 // O(n^3) algorithm.
 for(size_t i=0;i<n-2;++i)
 for(size_t j=i+1;j<n-1;++j)
 for(size_t k=j+1;k<n;++k)
 if( (v[i] < v[j] && v[i] < v[k] && v[j] > v[k]) )
 return true;
 
 return false;
 
 }
 
 int main(int argc, const char * argv[]) {
 
 dsl::permutation<dsl::store::bidirectional, dsl::restrictions::by_function> p(11, avoids_132);
 
 // Should return the 11-th Catalan number: 58786
 std::cout << p.count() << std::endl;
 
 
 return 0;
 }
 @endcode
 
 Example 6: This code was used to update the values a(13)-a(14) in the sequence OEIS A165546.
 @code
 
 #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
 
 //function to test whether a permutation avoids both 3412 AND 2413.
 bool avoids_3412_and_2413(const std::vector<size_t>& v) {
 
 size_t n = v.size();
 
 // Look at all quadruplets of indices to see if pattern is violated.
 // O(n^4) algorithm.
 for(size_t h=0;h<n-3;++h)
 for(size_t i=h+1;i<n-2;++i)
 for(size_t j=i+1;j<n-1;++j)
 for(size_t k=j+1;k<n;++k)
 if( (v[j] < v[k] && v[k] < v[h] && v[h] < v[i]) ||
 (v[j] < v[h] && v[h] < v[k] && v[k] < v[i]) )
 return true;
 
 return false;
 
 }
 
 int main(int argc, const char * argv[]) {
 
 // Start your engines ...
 dsl::time t;
 
 // Create a collection of permutations of {1,2,...,13}, and the ability to iterate forward from a given valid permutation.
 size_t i = 13;
 dsl::permutation<dsl::store::forward, dsl::restrictions::by_function> p(i, avoids_3412_and_2413);
 
 // Calculate the number of elements in the set, threaded since we defined begin(i) and end(i) for i=1,2,...,n
 std::cout << i << ": " << p.count_by_threads() << ", ";
 std::cout << "in " << t.toc() << " seconds \n";
 std::cout.flush();
 
 // Repeat for permutations of {1,2,...,14}
 i = 14;
 dsl::permutation<dsl::store::forward, dsl::restrictions::by_function> p2(i, avoids_3412_and_2413);
 
 std::cout << i << ": " << p2.count_by_threads() << ", ";
 std::cout << "in " << t.toc() << " seconds \n";
 std::cout.flush();
 
 return 0;
 }
 @endcode
 
 Example 7:
 @code
 // Code to check the n choose k different sets, make sure each are occurring equally likely
 #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
 
 
 //function to test whether a permutation avoids both 3412 AND 2413.
 bool every_321_extends_to_3241(const std::vector<size_t>& v) {
 
 size_t n = v.size();
 
 bool flag = true;
 
 if(v[0] == 3 && v[1] == 5 && v[2] == 2 && v[3] == 4 && v[4] == 1) {
 //std::cout << v << std::endl;
 }
 
 // Look at all quadruplets of indices to see if pattern is violated.
 // O(n^4) algorithm.
 for(size_t i=0;i<n-2;++i)
 for(size_t j=i+1;j<n-1;++j)
 for(size_t k=j+1;k<n;++k) {
 //std::cout << v << std::endl;
 if( (v[i] > v[j] && v[j] > v[k] ) ) {
 
 flag = false; // reset flag
 
 for(size_t s = j+1;s<k;++s)
 if(v[s]>v[i]) {
 flag = true;
 break;
 }
 
 if(!flag) return true;
 }
 }
 
 return false;
 
 }
 
 int main(int argc, const char * argv[]) {
 
 // Start your engines ...
 dsl::time t;
 
 // The condition that every occurrence of 321 extends to an occurrence of 3241 implies the set of permutations is in one-to-one correspondence to the Bell numbers.
 size_t i = 5;
 dsl::permutation<dsl::store::forward, dsl::restrictions::by_function> p(i, every_321_extends_to_3241);
 
 for(auto& x : p)
 std::cout << x << ",";
 std::cout << std::endl;
 
 // Calculate the number of elements in the set, threaded since we defined begin(i) and end(i) for i=1,2,...,n
 std::cout << i << ": " << p.count_by_threads() << ", ";
 std::cout << "in " << t.toc() << " seconds \n";
 std::cout.flush();
 
 
 return 0;
 }
 @endcode
 
 Example 8:
 @code

// Attempt to speed up pattern-avoiding permutation calculation using a local cache.  Didn't really work like I hoped it would, even though optimized to not need locks or mutexes.

#include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.

// Store local cache of values which caused violations.  Restart from scratch every time the cache exceeds say 100 entries.

#include <map>

// max number of threads
size_t num_threads = 20;

// max number of stored violations
size_t max_caches = 100;
std::vector< std::vector< std::vector<size_t> > > cache(num_threads, std::vector<std::vector<size_t>>(max_caches,{0,0,0,0}));
std::vector< size_t > current_size(num_threads);

size_t id = 1;
std::map< std::thread::id, size_t > mapping;


//function to test whether a permutation avoids both 3412 AND 2413.
bool avoids_3412_and_2413(const std::vector<size_t>& v) {
    
    size_t n = v.size();
    
    // Establish an integer-correspondence with each thread, since thread ids are not convertible to int.
    // would ideally need to optimize this away, since not necessary to keep doing this every function call.
    // Only accounted for 4.2% of time.
    if(mapping[std::this_thread::get_id()] == 0)
    {
        mapping[std::this_thread::get_id()] = id++;
        //std::cout << "id = " << id << "mapping = " << mapping[std::this_thread::get_id()] << std::endl;
        
    }
    
    // Get current thread index
    size_t thread = mapping[std::this_thread::get_id()];
    
    
    // Go through elements in the cache first, since recently found violations are likely to continue occurring in future calls.
    for(size_t ii=0,nn=current_size[thread];ii<nn;++ii) {
        
        size_t h = cache[thread][ii][0];
        size_t i = cache[thread][ii][1];
        size_t j = cache[thread][ii][2];
        size_t k = cache[thread][ii][3];
        
        if( (v[j] < v[k] && v[k] < v[h] && v[h] < v[i]) ||
           (v[j] < v[h] && v[h] < v[k] && v[k] < v[i])    )
            return true;
    }
    
    // Look at all quadruplets of indices to see if pattern is violated.
    // O(n^4) algorithm.
    for(size_t h=0;h<n-3;++h)
        for(size_t i=h+1;i<n-2;++i) {
            // early abort condition
            while(h < n-3 && i < n-2 && v[h] > v[i]) {
                ++i;
                if(i == n-2) {
                    ++h;
                    i=h+1;
                }
            }
            for(size_t j=i+1;j<n-1;++j)
                for(size_t k=j+1;k<n;++k)
                    
                    // check condition
                    if( (v[j] < v[k] && v[k] < v[h] && v[h] < v[i]) ||
                       (v[j] < v[h] && v[h] < v[k] && v[k] < v[i]) ) {
                        
                        // If we are running this code, we have a violation!
                        
                        // clear cache if it gets too big.
                        if(current_size[thread] >= max_caches) {
                            //std::cout << "cache reset" << std::endl;
                            
                            // Rather than calling .clear, we reset the max relevant value
                            //cache.clear();
                            current_size[thread] = 0;
                        }
                        
                        // update cached value for future reference.
                        cache[thread][current_size[thread]++] = std::vector<size_t>({h,i,j,k});
                        return true;
                    }
        }
    
    // no violations occurred
    return false;
    
}

int main(int argc, const char * argv[]) {
    
    
    // Start your engines ...
    dsl::time t;
    
    // Create a collection of permutations of {1,2,...,13}, and the ability to iterate forward from a given valid permutation.
    size_t i = 11;
    dsl::permutation<dsl::store::forward, dsl::restrictions::by_function> p(i, avoids_3412_and_2413);
    
    //for(auto& x : p)
    //    std::cout << x << std::endl;
    
    // Calculate the number of elements in the set, threaded since we defined begin(i) and end(i) for i=1,2,...,n
    std::cout << i << ": " << p.count_by_threads() << ", ";
    //std::cout << i << ": " << p.count() << ", ";
    std::cout << "in " << t.toc() << " seconds \n";
    std::cout.flush();
    
    
    return 0;
}

 @endcode
 
 Example 9:
 @code
// Generates iid samples of random Mallows(q) permutations for q = qmin, ..., qmax and checks how many avoid 321 consecutively.  The variable n is the size of the permutation and m is the number of samples.


#include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.

// avoiding 321 consecutively
bool avoids_321_consecutively(const std::vector<size_t>& v) {
    
    size_t n = v.size();
    
    // Look at all consecutive triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-3;++i)
        if( (v[i] > v[i+1] && v[i+1] > v[i+2]) )
            return false;
    
    return true;
    
}

int main(int argc, const char * argv[]) {
    
    // Create mesh grid
    double qmin = 0.01;
    double qmax = 1.;
    size_t mesh_size = 20;
    std::vector<double> vals(mesh_size+1);
    
    // n is the size of the permutation, m is the number of iterations.
    size_t n = 30;
    size_t m = 100;
    
    // keeps track of which value of q we are using in the vector
    size_t index = 0;
    
    // q = qmin, qmin+delta, qmin+2delta, ..., qmax
    for(long double q = qmin; q <= qmax; q += (qmax-qmin)/(mesh_size-1)) {
        
        double avoids_321 = 0.; // count number that avoid 321
        
        for(size_t i=0;i<m;++i)
            
            // generate permutation using Mallows(q) distribution, test for whether it avoids 321 consecutively.
            if(avoids_321_consecutively(dsl::random_permutation_mallows(n, q, dsl::generator_64)))
                avoids_321 = avoids_321 + 1.;
        
        // Keep track of which ones avoid, store the average^(1/n)
        if(avoids_321)
            vals[index++] = std::pow(avoids_321/m,1./n);
        else
            vals[index++] = 1.;
    }
    
    dsl::print(vals,"[", ",", "]");
    
    //    std::cout << std::pow(avoids_321,-1./n) << std::endl;
    
    return 0;
}
 @endcode
 
 Example 10:
 @code
#include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.

// NOT WORKING YET!  In process of being generalized from m=3 to general m
//  consecutively
bool avoids_all_m(size_t m, size_t n) {
    
    // generate all permutations of size m
    auto patterns = dsl::permutations<>( dsl::range(m) );
    
    size_t m_size = patterns.size();
    
    // set up vector of bools to check which patterns occurred over all permutations of n
    std::vector<bool> occurred(m_size);
    
    dsl::permutation<forward> permutations(n);
    
    // for each permutation of n ...
    for(auto& x : permutations) {
        
        for(size_t i=0;i<n-2;++i)
            for(size_t j=i+1;j<n-1;++j)
                for(size_t k=j+1;k<n;++k)
                    for(size_t s=0,r=patterns.size(); s<r;++s)
                        
                        if( dsl::permutation_reduction({x[i],x[j],x[k]}) == patterns[s]) {
                            occurred[s] = true;
                            s = r; // stop this one
                            
                            // check if all conditions met
                            if(std::all_of(occurred.begin(), occurred.end(), true))
                                return true;
                        }
    }
    
    return false;
    
}

// Look at all consecutive triplets of indices to see if pattern is violated.
// O(n) algorithm.

return true;

}

int main() {
    
    size_t nmax = 12;
    
    std::vector<std::vector<size_t>> superpatterns(nmax, std::vector<size_t>(nmax));
    
    for(size_t i=2;i<=nmax;++i) {
        
        // All permutations of {1,2,..,i}
        dsl::permutation<forward> permutations(i);
        
        
        for(size_t j=2;j<=i;++i) {
            // all patterns of lengths 2, ..., i
            dsl::permutation<forward> patterns(j);
            
            
            
        }
    }
    
    //std::cout << v << std::endl;
    
    std::cout << reduction({1, 30, 500, 2, 4, 8, 11, 123, 50}) << std::endl;
    
    
    
    return 0;
}

 @endcode
 
Example 11:
 @code
// Generates iid samples of random Mallows(q) permutations for q = qmin, ..., qmax and estimates the probability of avoiding a given pattern of size 3. The variable n is the size of the permutation and m is the number of samples.

#include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
#include <map>


// avoiding 123
template<typename T>
bool avoids_123_consecutively(const std::vector<T>& v) {
    
    size_t n = v.size();
    
    // Look at all triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-2;++i)
        if( v[i] < v[i+1] && v[i+1] < v[i+2] ){
            //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
            return false;
        }
    //return true;
    
    return true;
    
}

// avoiding 132
template<typename T>
bool avoids_132_consecutively(const std::vector<T>& v) {
    
    size_t n = v.size();
    
    // Look at all triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-2;++i)
        if( (v[i] < v[i+1] && v[i] < v[i+2] && v[i+1] > v[i+2]) ){
            //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
            return false;
        }
    //return true;
    
    return true;
    
}



// avoiding 213
template<typename T>
bool avoids_213_consecutively(const std::vector<T>& v) {
    
    size_t n = v.size();
    
    // Look at all triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-2;++i)
        if( (v[i] > v[i+1] && v[i] < v[i+2] && v[i+1] < v[i+2]) ){
            //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
            return false;
        }
    //return true;
    
    return true;
    
}



// avoiding 132
template<typename T>
bool avoids_231_consecutively(const std::vector<T>& v) {
    
    size_t n = v.size();
    
    // Look at all triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-2;++i)
        if( (v[i] < v[i+1] && v[i] > v[i+2] && v[i+1] > v[i+2]) ){
            //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
            return false;
        }
    //return true;
    
    return true;
    
}



// avoiding 132
template<typename T>
bool avoids_312_consecutively(const std::vector<T>& v) {
    
    size_t n = v.size();
    
    // Look at all triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-2;++i)
        if( (v[i] > v[i+1] && v[i] > v[i+2] && v[i+1] < v[i+2]) ){
            //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
            return false;
        }
    //return true;
    
    return true;
    
}


// avoiding 132
template<typename T>
bool avoids_321_consecutively(const std::vector<T>& v) {
    
    size_t n = v.size();
    
    // Look at all triplets of indices to see if pattern is violated.
    // O(n) algorithm.
    for(size_t i=0;i<n-2;++i)
        if( (v[i] > v[i+1] && v[i+1] > v[i+2]) ){
            //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
            return false;
        }
    //return true;
    
    return true;
    
}


int main(int argc, const char * argv[]) {
    
    
    
    // Create mesh grid
    double qmin = .01;
    double qmax = 2.;
    size_t mesh_size = 51;
    std::vector<std::vector<double>> vals(6, std::vector<double>(mesh_size-1));
    std::vector<double> qs(mesh_size-1);
    
    // n is the size of the permutation, m is the number of iterations.
    size_t n = 20;
    size_t m = 100000;
    
    // keeps track of which value of q we are using in the vector
    size_t index = 0;
    
    std::map<int, size_t> pattern_to_counts;
    
    // q = qmin, qmin+delta, qmin+2delta, ..., qmax
    for(long double q = qmin; q < qmax; q += (qmax-qmin)/(mesh_size-1)) {
        
        qs[index] = q;
        
        pattern_to_counts[123] = 0;
        pattern_to_counts[132] = 0;
        pattern_to_counts[213] = 0;
        pattern_to_counts[231] = 0;
        pattern_to_counts[312] = 0;
        pattern_to_counts[321] = 0;
        
        //double avoids_321 = 0.; // count number that avoid 321
        
        for(size_t i=0;i<m;++i) {
            
            // generate random permutation in Mallows form
            auto v = dsl::random_permutation_mallows(n, q);
            //auto v = dsl::random_permutation(n);
            
            // generate permutation using Mallows(q) distribution, test for whether it avoids 321 consecutively.
            if(avoids_123_consecutively(v)) ++pattern_to_counts[123];
            if(avoids_132_consecutively(v)) ++pattern_to_counts[132];
            if(avoids_213_consecutively(v)) ++pattern_to_counts[213];
            if(avoids_231_consecutively(v)) ++pattern_to_counts[231];
            if(avoids_312_consecutively(v)) ++pattern_to_counts[312];
            if(avoids_321_consecutively(v)) ++pattern_to_counts[321];
        }
        
        // Keep track of which ones avoid, store the average^(1/n)
        if(pattern_to_counts[123] > 0) vals[0][index] = std::pow((double)pattern_to_counts[123]/m,1./n);
        else vals[0][index] = 1.;
        
        // Keep track of which ones avoid, store the average^(1/n)
        //if(pattern_to_counts[123] > 0) vals[0][index] = (double)pattern_to_counts[123]/m;
        //else vals[0][index] = 1.;
        
        if(pattern_to_counts[132] > 0) vals[1][index] = std::pow((double)pattern_to_counts[132]/m,1./n);
        else vals[1][index] = 1.;
        
        if(pattern_to_counts[213] > 0) vals[2][index] = std::pow((double)pattern_to_counts[213]/m,1./n);
        else vals[2][index] = 1.;
        
        if(pattern_to_counts[231] > 0) vals[3][index] = std::pow((double)pattern_to_counts[231]/m,1./n);
        else vals[3][index] = 1.;
        
        if(pattern_to_counts[312] > 0) vals[4][index] = std::pow((double)pattern_to_counts[312]/m,1./n);
        else vals[4][index] = 1.;
        
        if(pattern_to_counts[321] > 0) vals[5][index] = std::pow((double)pattern_to_counts[321]/m,1./n);
        else vals[5][index] = 1.;
        
        ++index;
        
    }
    
    for(size_t i=0;i<6;++i) {
        
        std::cout << "ListLinePlot[Transpose[";
        std::cout << "{";
        dsl::print(qs,"{", ",", "}");
        std::cout << ",";
        dsl::print(vals[i],"{", ",", "}");
        std::cout << "}";
        std::cout << "],PlotRange->{" <<
        (double)(floor((*std::min_element(vals[i].begin(), vals[i].end()))*10.))/10.;
        std::cout << ",1},PlotLabel->\"";
        
        switch(i) {
            case 0 : std::cout << "123"; break;
            case 1 : std::cout << "132"; break;
            case 2 : std::cout << "213"; break;
            case 3 : std::cout << "231"; break;
            case 4 : std::cout << "312"; break;
            case 5 : std::cout << "321"; break;
        }
        
        std::cout << "\"]";
        
        std::cout << std::endl;
        
    }

    return 0;
}

@endcode
*/


/** @class binary_contingency_table
    @brief Stores a binary table with row sums and column sums specified
 
 
 
 
 Example 1:
 @code

#include <iostream>
#include "contingency_table.h"


namespace dsl = desalvo_standard_library;


int main(int argc, const char * argv[]) {
    
    
    
    
    
    dsl::binary_contingency_table<unsigned int> bct({2,2,2,2},{2,2,2,2});
    
    
    std::vector<unsigned int> r {1,1,1};
    std::vector<unsigned int> c {1,1,1};
    
    std::vector<dsl::numerical_table<unsigned int, unsigned int>> squares;
    std::vector<size_t> counts;
    
    std::default_random_engine gen(1);
    
    size_t i=0;
    size_t iterations = 10000;
    size_t index = 0;
    size_t current_index = 0;
    
    
    for(size_t ii=0;ii<iterations;++ii) {
        
        if(ii % 1000 == 0)
            std::cout << ii << std::endl;
        
        auto bct = dsl::random_binary_contingency_table<unsigned int>(r,c,gen);
        
        dsl::numerical_table<unsigned int, unsigned int> bt = bct;
        
        index = 0;
        
        for(i=0;i<current_index;++i) {
            if(bt == squares[i]) {
                ++counts[i];
                i=current_index;
            }
        }
        if(i==current_index) {
            squares.push_back(bt);
            counts.push_back(1);
            current_index++;
        }
    }
    
    std::cout << counts << std::endl;
    std::cout << counts.size() << std::endl;
    
    for(auto& x : squares)
        std::cout << x << std::endl;
    
    std::cout << bct.count() << std::endl;
    
    
    
    return 0;
}
 @endcode
 */




/** @class binary_contingency_table_with_zeros
    @brief Stores a binary table with row sums and column sums specified
 

 
 @code
 #include <iostream>
 #include "std_cout.h"
 #include "contingency_table.h"
 
 namespace dsl = desalvo_standard_library;
 
 int main(int argc, const char * argv[]) {
 
 
     std::vector<unsigned int> row {2,2,2,2};
     std::vector<unsigned int> column {2,2,2,2};
     dsl::numerical_table<unsigned int, unsigned int> W(4,4);
 
 // Can also restrict certain entries (i,j) to be 0 by letting W(i,j) = 1.
 //W(0,0) = 1;    W(0,2) = 1;
 //W(1,1) = 1;    W(1,3) = 1;
 //W(2,0) = 1;    W(2,1) = 1;
 //W(3,2) = 1;    W(3,3) = 1;
 
     auto bct = dsl::random_binary_contingency_table(row, column, W);
 
     std::cout << bct << std::endl;
 
     return 0;
 }
 @endcode

 Example: Enumerating all Latin squares of order 5 via binary contingency tables with zeros
 @code
#include <iostream>
#include "contingency_table.h"

namespace dsl = desalvo_standard_library;


int main(int argc, const char * argv[]) {
    
    // Naively, this would be the row and column sums for n=5
    //std::vector<unsigned int> r {2,2,2,2,2};
    //std::vector<unsigned int> c {2,2,2,2,2};
    
    // let the first row and column be 1,2,3,4,5.
    std::vector<unsigned int> r {1,2,1,2};
    std::vector<unsigned int> c {1,2,1,2};
    
    dsl::binary_contingency_table<unsigned int> bct(r,c);
    
    size_t index = 0;
    size_t index0 = 0;
    size_t index1 = 0;
    std::vector<size_t> items0;
    std::vector<size_t> items00;
    std::vector<size_t> items1;
    
    size_t total_count = 0;
    
    const size_t n = 5;
    
    // for each BCT with {1,2,1,2} rc-sums
    for(auto& x : bct) {
        
        //std::cout << index++ << std::endl;
        //std::cout << "W = \n" << x << std::endl;
        
        // rather than {1,1,1,1,1} rc-sums, assume {1,2,3,4,5} first row/column
        // The binary decomposition of 0,1,2,3,4 implies a 1 in column index 4
        // corresponding to value 3 = (11)_2
        r = {1,1,0,1};
        c = {1,1,0,1};
        
        dsl::binary_contingency_table_with_zeros<unsigned int> bct1(r,c,1-x);
        
        index1 = 0;
        // For each BCT on the 1 entries
        for(auto& z : bct1) {
            
            ++index1;
            
            // rather than {1,1,1,1,1} rc-sums, assume {1,2,3,4,5} first row/column
            // The binary decomposition of 0,1,2,3,4 implies a 1 in column index 3
            // corresponding to value 2 = (10)_2
            r = {1,0,1,1};
            c = {1,0,1,1};
            
            dsl::binary_contingency_table_with_zeros<unsigned int> bct0(r,c,x);
            
            // for each BCT on the 0 entries
            for(auto& y : bct0) {
                
                ++index0;
                auto W2 = x;
                
                //std::cout << "W0 = \n" << y << std::endl;
                
                // Update allowable entries with 00 locations
                for(size_t i=0;i<n-1;++i)
                    for(size_t j=0;j<n-1;++j)
                        if(y(i,j) == 1)
                            W2(i,j) = 1;
                
                //std::cout << "W00 = \n" << W2 << std::endl;
                
                r = {1,1,1,0};
                c = {1,1,1,0};
                
                dsl::binary_contingency_table_with_zeros<unsigned int> bct00(r,c,W2);
                size_t count00 = 0;
                
                // Iterate over the 00 table
                for(auto& w : bct00) {
                    ++count00;
                    
                    //std::cout << "ls = \n" << x+2*y+2*z+4*w<< std::endl;
                    ++total_count;
                }
                items00.push_back(count00);
            }
            
            items0.push_back(index0);
            index0=0;
        }
        
        items1.push_back(index1);
    }
    
    //std::cout << "items0 = " << items0 << std::endl;
    //std::cout << "items00 = " << items00 << std::endl;
    //std::cout << "items1 = " << items1 << std::endl;
    
    std::cout << "total number of reduced Latin squares of order five = " << total_count << std::endl;
    
    return 0;
}
@endcode

 Example: Enumerating all Latin squares of order 6 via binary contingency tables with zeros

 @code
// Iterates through all 6 x 6 binary contingency tables and subtables to form as many Latin squares as possible in reduced form.
#include <iostream>
#include "contingency_table.h"

namespace dsl = desalvo_standard_library;


int main(int argc, const char * argv[]) {
    
    // Naively, this would be the row and column sums for n=6
    //std::vector<unsigned int> r {3,3,3,3,3,3};
    //std::vector<unsigned int> c {3,3,3,3,3,3};
    
    // let the first row and column be 1,2,3,4,5,6.
    std::vector<unsigned int> r {2,3,2,3,2};
    std::vector<unsigned int> c {2,3,2,3,2};
    
    dsl::binary_contingency_table<unsigned int> bct(r,c);
    
    size_t index = 0;
    size_t index0 = 0;
    size_t index1 = 0;
    std::vector<size_t> items0;
    std::vector<size_t> items00;
    std::vector<size_t> items1;
    std::vector<size_t> items11;
    
    size_t total_count = 0;
    
    const size_t n = 6;
    
    // for each BCT with {2,3,2,3,2} rc-sums
    for(auto& x : bct) {
        
        std::cout << index++ << std::endl;
        //std::cout << "W = \n" << x << std::endl;
        
        // rather than {1,1,1,1,1,1} rc-sums, assume {1,2,3,4,5,6} first row/column
        r = {1,1,0,1,1};
        c = {1,1,0,1,1};
        
        dsl::binary_contingency_table_with_zeros<unsigned int> bct1(r,c,1-x);
        
        index1 = 0;
        
        // For each BCT on the 1 entries
        for(auto& z : bct1) {
            
            auto W1 = (1-x) + z;
            
            r = {1,1,1,1,0};
            c = {1,1,1,1,0};
            
            dsl::binary_contingency_table_with_zeros<unsigned int> bct11(r,c,W1);
            size_t count11 = 0;
            
            // Iterate over the 00 table
            for(auto& w : bct11) {
                ++count11;
                ++index1;
                
                //std::cout << "ls = \n" << x+2*z+4*w<< std::endl;
            }
            items11.push_back(count11);
        }
        
        items1.push_back(index1);
        
        
        index0 = 0;
        
        // rather than {1,1,1,1,1} rc-sums, assume {1,2,3,4,5} first row/column
        r = {1,0,1,1,1};
        c = {1,0,1,1,1};
        
        dsl::binary_contingency_table_with_zeros<unsigned int> bct0(r,c,x);
        
        // for each BCT on the 0 entries
        for(auto& y : bct0) {
            
            auto W2 = x+y;
            
            r = {1,1,1,0,1};
            c = {1,1,1,0,1};
            
            dsl::binary_contingency_table_with_zeros<unsigned int> bct00(r,c,W2);
            size_t count00 = 0;
            
            // Iterate over the 00 table
            for(auto& w : bct00) {
                ++count00;
                ++index0;
                
                //std::cout << "ls = \n" << 2*y+4*w<< std::endl;
                //++total_count;
            }
            items00.push_back(count00);
        }
        
        items0.push_back(index0);
        
        total_count += index0*index1;
        
        
    }
    
    std::cout << "items0 = " << items0 << std::endl;
    std::cout << "items00 = " << items00 << std::endl;
    std::cout << "items1 = " << items1 << std::endl;
    std::cout << "items11 = " << items11 << std::endl;
    
    std::cout << "total number of reduced Latin squares of order six = " << total_count << std::endl;
    
    return 0;
}
@endcode

 Example: Enumerating all Latin squares of order 7 via binary contingency tables with zeros.  Too slow to do the 100,000 iterations in serial, should probably parallelize outermost loop, but other more pressing activities at the moment!
 
 @code
 // Iterates through all 7 x 7 binary contingency tables and subtables to form as many Latin squares as possible in reduced form.
#include <iostream>
#include "contingency_table.h"

namespace dsl = desalvo_standard_library;


int main(int argc, const char * argv[]) {
    
    // Naively, this would be the row and column sums for n=7
    //std::vector<unsigned int> r {3,3,3,3,3,3,3};
    //std::vector<unsigned int> c {3,3,3,3,3,3,3};
    
    // let the first row and column be 1,2,3,4,5,6,7.
    std::vector<unsigned int> r {2,3,2,3,2,3};
    std::vector<unsigned int> c {2,3,2,3,2,3};
    
    dsl::binary_contingency_table<unsigned int> bct(r,c);
    
    size_t index  = 0;
    size_t index0 = 0;
    size_t index1 = 0;
    std::vector<size_t> items0;
    std::vector<size_t> items00;
    std::vector<size_t> items01;
    std::vector<size_t> items1;
    std::vector<size_t> items11;
    
    size_t total_count = 0;
    size_t n = 7;
    
    // determine locations of 1,3,5 using 1s of bct, and 0,2,4,6 using 0s of bct
    for(auto& x : bct) {
        
        std::cout << index++ << std::endl;
        
        // determine locations of the 3
        r = {1,1,0,1,1,1};
        c = {1,1,0,1,1,1};
        
        dsl::binary_contingency_table_with_zeros<unsigned int> bct1(r,c,1-x);
        
        index1 = 0;
        
        // Determine locations of 1 vs 5 using zeros of bct1
        for(auto& z : bct1) {
            
            auto W1 = (1-x) + z;
            
            r = {1,1,1,1,0,1};
            c = {1,1,1,1,0,1};
            
            dsl::binary_contingency_table_with_zeros<unsigned int> bct11(r,c,W1);
            size_t count11 = 0;
            
            // this loop fixes 1 or 5 in each possible way, 1 corresponds to 5, 0 corresponds to 1
            for(auto& w : bct11) {
                ++count11;
                ++index1;
                
            }
            items11.push_back(count11);
        }
        
        items1.push_back(index1);
        
        
        index0 = 0;
        
        // where are the 0,4 vs 2,6 locations.  A 1 now means a 2,6 entry
        r = {2,1,2,2,2,1};
        c = {2,1,2,2,2,1};
        
        dsl::binary_contingency_table_with_zeros<unsigned int> bct0(r,c,x);
        
        // for each BCT on the 0 entries
        for(auto& y : bct0) {
            
            // first do the 0,4 locations, determine where the 4s are
            auto W2 = x+y;
            
            r = {1,1,1,0,1,1};
            c = {1,1,1,0,1,1};
            
            dsl::binary_contingency_table_with_zeros<unsigned int> bct00(r,c,W2);
            size_t count00 = 0;
            
            // Iterate over the 00 table, a 1 places a 4 there
            for(auto& w : bct00) {
                ++count00;
                ++index0;
                
            }
            items00.push_back(count00);
            
            // Now we need to determine where the 2,6s are
            auto W01 = x;
            
            // Block out the locations of the 0,4s
            for(size_t i=0;i<n-1;++i)
                for(size_t j=0;j<n-1;++j)
                    if(y(i,j) == 0)
                        W01(i,j) = 1;
            
            
            
            r = {1,1,1,1,1,0};
            c = {1,1,1,1,1,0};
            
            dsl::binary_contingency_table_with_zeros<unsigned int> bct01(r,c,W01);
            size_t count01 = 0;
            
            // Iterate over the 00 table, find locations of the 6s
            for(auto& w : bct01) {
                ++count01;
                ++index0;
            }
            items01.push_back(count01);
            
        }
        
        items0.push_back(index0);
        
        total_count += index0*index1;
    }
    
    std::cout << "items0 = " << items0 << std::endl;
    std::cout << "items00 = " << items00 << std::endl;
    std::cout << "items1 = " << items1 << std::endl;
    std::cout << "items11 = " << items11 << std::endl;
    
    std::cout << "total number of reduced Latin squares of order six = " << total_count << std::endl;
    
    return 0;
}
 @endcode
*/



/** @class latin_square
    @brief set of n x n tables with each row and column a permutation of {1,2,...,n}

 Example 1:
 @code
 // This code generates a latin square or oder i=4,5,...,100
dsl::time timer;

for(unsigned int n = 4; n<=100;n++) {
    
    srand(0);
    std::default_random_engine gen;
    
    std::cout << n << "\n";
    
    // repeatable results, switch with below for more random
    auto bct = dsl::random_latin_square(n,gen);
    //auto bct = dsl::random_latin_square(n);
    
    std::cout << "ls=\n"<<bct << std::endl;
    std::cout << "time = " << timer.toc() << " seconds" << std::endl;
    timer.reset();
    }
@endcode
 
 Example 2:
    @code
 
 // This code generates 1 million Latin squares of order 5 via conntingency tables and
 // counts the number of each type. 
 
const size_t iterations = 1000000;
const size_t n = 5;


std::vector<dsl::numerical_table<unsigned int, unsigned int>> squares;
//squares.push_back(std::vector<std::vector<unsigned int>>({{1,2,3,4},{2,1,4,3},{3,4,1,2},{4,3,2,1}}));
//squares.push_back(std::vector<std::vector<unsigned int>>({{1,2,3,4},{2,1,4,3},{3,4,2,1},{4,3,1,2}}));
//squares.push_back(std::vector<std::vector<unsigned int>>({{1,2,3,4},{2,3,4,1},{3,4,1,2},{4,1,2,3}}));
//squares.push_back(std::vector<std::vector<unsigned int>>({{1,2,3,4},{2,4,1,3},{3,1,4,2},{4,3,2,1}}));



size_t current_index = 0;
std::vector<size_t> counts;

srand(0);
std::default_random_engine gen;

auto bct = dsl::random_latin_square<unsigned int>(n,gen);
//auto bct = dsl::random_latin_square(n);

dsl::numerical_table<unsigned int, unsigned int> bt = bct;

//std::cout << bt << std::endl;


size_t index = 0;

std::vector<unsigned int> r(n);
for(auto& x : bt.column(0)) r[index++]=x-1;

bt.permute_rows(r);

index = 0;
std::vector<unsigned int> c(n);
for(auto& x : bt.row(0)) c[index++]=x-1;
bt.permute_columns(c);

squares.push_back(bt);
counts.push_back(1);
current_index = 1;

size_t i=0;

for(size_t ii=0;ii<iterations;++ii) {
    
    if(ii % 1000 == 0)
        std::cout << ii << std::endl;
        
        auto bct = dsl::random_latin_square<unsigned int>(n,gen);
        
        dsl::numerical_table<unsigned int, unsigned int> bt = bct;
        
        index = 0;
        
        
        for(auto& x : bt.column(0)) r[index++]=x-1;
        
        bt.permute_rows(r);
        
        index = 0;
        
        for(auto& x : bt.row(0)) c[index++]=x-1;
        bt.permute_columns(c);
        
        for(i=0;i<current_index;++i) {
            if(bt == squares[i]) {
                ++counts[i];
                i=current_index;
            }
        }
        if(i==current_index) {
            squares.push_back(bt);
            counts.push_back(1);
            current_index++;
        }
        
        }
        
        std::cout << counts << std::endl;
        std::cout << counts.size() << std::endl;
        
        std::sort(counts.begin(), counts.end());
        std::cout << counts << std::endl;

 @endcode
 
 */
