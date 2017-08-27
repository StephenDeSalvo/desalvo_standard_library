// numerical.h

// See Documentation.h for file documentation


#ifndef Combinatorics_DeSalvoNumericalLibrary_h
#define Combinatorics_DeSalvoNumericalLibrary_h

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <set>
#include <cmath>
#include <unordered_map>


namespace desalvo_standard_library {

    /** @typedef ull abbrev. for unsigned long long */
    typedef unsigned long long ull;

    // Function declarations
    
    
    template<typename Integer, typename UnsignedInteger=Integer>
    UnsignedInteger gcd(Integer a, Integer b);
    
    template<typename F=double, typename V = std::vector<F>, typename Size = size_t >
    V range(Size n, F initial_value=1. );
        
    template<typename F=double, typename V = std::vector<F>, typename Size = size_t >
    V constant_array(Size n, F initial_value=1. );
    
    
    // Some functions having to do with permutations, inverses, and applying a permutation map
    template<typename IntType = size_t, typename Container = std::vector<std::vector<IntType>> >
    Container permutation_as_product_of_cycles(const std::vector<IntType>& permutation);
    
    template<typename Container = std::vector<std::vector<size_t>> >
    Container permutation_inverse(Container permutation);
    
    template<typename IntType = size_t, typename Container = std::vector<std::vector<IntType>> >
    std::vector<IntType> permutation_cycles_in_direct_form(const Container& permutation);
    
    
    template<typename IntType = size_t >
    std::vector<IntType> permutation_inverse_direct_form(std::vector<IntType> permutation);

    template<typename Vector = std::vector<size_t>>
    void permute_by(Vector& v, Vector& permutation);
    
    template<typename Vector = std::vector<size_t>>
    Vector permuted_by(Vector v, Vector& permutation);
    
    template<typename InputIterator>
    bool permutation_is_identity(InputIterator start, InputIterator stop);
    
    
    
    
    
    
    
    
    
    
    
    /** Computes the greatest common divisor using Euclid's algorithm.
        @param a is an input integer
        @param b is an input integer
        @returns the gcd(a,b)
     @code
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     std::cout << "dsl::gcd(5,3) = " << dsl::gcd(5,3) << std::endl;
     std::cout << "dsl::gcd(-5,3) = " << dsl::gcd(-5,3) << std::endl;
     std::cout << "dsl::gcd(5,-3) = " << dsl::gcd(5,-3) << std::endl;
     std::cout << "dsl::gcd(-5,-3) = " << dsl::gcd(-5,-3) << std::endl;
     std::cout << "dsl::gcd(0,3) = " << dsl::gcd(0,3) << std::endl;
     std::cout << "dsl::gcd(-4,0) = " << dsl::gcd(-4,0) << std::endl;
     std::cout << "dsl::gcd(2,12) = " << dsl::gcd(2,12) << std::endl;
     std::cout << "dsl::gcd(1111111115,5) = " << dsl::gcd(1111111115,5) << std::endl;
     std::cout << "dsl::gcd(54321,101) = " << dsl::gcd(54321,101) << std::endl;
     
     return 0;
    }
    @endcode
     Should produce output
     @code
     dsl::gcd(5,3) = 1
     dsl::gcd(-5,3) = 1
     dsl::gcd(5,-3) = 1
     dsl::gcd(-5,-3) = 1
     dsl::gcd(0,3) = 0
     dsl::gcd(-4,0) = 0
     dsl::gcd(2,12) = 2
     dsl::gcd(1111111115,5) = 5
     dsl::gcd(54321,101) = 1
     @endcode
    */
    template<typename Integer, typename UnsignedInteger>
    UnsignedInteger gcd(Integer a, Integer b) {
        if(a<0) a = -a;
        if(b<0) b = -b;
        
        if(a == 0 || b == 0) return static_cast<UnsignedInteger>(0);
        
        while(b!= 0) {
            auto t(b);
            b = a%b;
            a = t;
        }
        return static_cast<UnsignedInteger>(a);
    }
    
    /** Initializes list to {initial_value, initial_value+1,...,initial_value+n-1}.
     By default, the initial value of 1.
     @tparam Size is any nonnegative integer type
     @tparam V is the container to store the values
     @tparam F is the data type of the values
     @param n is the size of the collection
     @param initial_value is the value of the first element.
     @returns a collection of values starting with initial_value and adding 1 n-1 times
     
     @details
     @code
     // Example of using dsl::range
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
         // range calls the += operator, so can be used with any numerical container as well like valarray
         auto v = dsl::range(10);
         auto v2 = dsl::range(10, 0.1);
         auto v3 = dsl::range(10, 0);
         auto v4 = dsl::range(10, std::valarray<short>({0,1,2,3,4}));
     
         std::cout << v << std::endl;
         std::cout << v2 << std::endl;
         std::cout << v3 << std::endl;
         std::cout << v4 << std::endl;
     
         return 0;
     }
     @endcode
    */
    template<typename F, typename V, typename Size>
    V range(Size n, F initial_value) {
        
        V v(n);                                     // Default construct v with n elements
        auto start = std::begin(v);
        auto stop = std::end(v);
        while(start != stop) {
            *start++ = initial_value;
            initial_value += 1.;
        }
        //std::iota(std::begin(v), std::end(v),initial_value);  // fill with {i,i+1,...,i+n-1}
        return v;                                   // return by value, assume RVO
    };
    

    /** Initializes list to {initial_value, initial_value+1,...,initial_value+n-1}.
     By default, the initial value of 1.
     @tparam Size is any nonnegative integer type
     @tparam V is the container to store the values
     @tparam F is the data type of the values
     @param n is the size of the collection
     @param initial_value is the value of the first element.
     @returns a collection of values starting with initial_value and adding 1 n-1 times
     
     @details
     @code
     // Example of using dsl::range
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // range calls the += operator, so can be used with any numerical container as well like valarray
     auto v = dsl::range(10);
     auto v2 = dsl::range(10, 0.1);
     auto v3 = dsl::range(10, 0);
     auto v4 = dsl::range(10, std::valarray<short>({0,1,2,3,4}));
     
     std::cout << v << std::endl;
     std::cout << v2 << std::endl;
     std::cout << v3 << std::endl;
     std::cout << v4 << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename F, typename V, typename Size >
    V constant_array(Size n, F initial_value) {
        
        V v(n);                                     // Default construct v with n elements
        auto start = std::begin(v);
        auto stop = std::end(v);
        while(start != stop)
            *start++ = initial_value;
        
        return v;                                   // return by value, assume RVO
    };
    
    
    
    /** Initializes list to {{i,j},{i+1,j},...,{i+m-1,j},{i,j+1},{i+1,j+1},...,{i+m-1,j+1},...,{i+m-1,j+n-1}}, where i is the initial first value, and j is the initial second value.\n
     Can change list to  {initial_value+1,...,initial_value+n-1} by second parameter initial_value
     @tparam Size is any nonnegative integer type
     @tparam V is the container to store the values
     @param m is the size of the collection
     @param n is the size of the collection
     @param initial_value_first is the first coordinate of the first index.
     @param initial_value_second is the second coordinate of the first index.
     @returns an m x n collection of indices forming a rectangular region of a matrix
     
     @details 
    @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Form matrix [[1,2,3],[4,5,6],[7,8,9]] using a row-major 1D vector
     std::vector<int> v {1,2,3,4,5,6,7,8,9};
     
     auto two_by_two = dsl::table_indices(2,2);
     
     auto start = std::begin(two_by_two);
     
     // To access an element at index (i,j) in the array, use j+i*m, where m is the number of rows
     std::cout << "Two by two submatrix of elements (1:2,1:2) \n";
     std::cout << "[[" << v[start->second + start->first*3] << ",";          ++start;
     std::cout         << v[start->second + start->first*3] << "],\n[";      ++start;
     std::cout         << v[start->second + start->first*3] << ",";          ++start;
     std::cout         << v[start->second + start->first*3] << "]]";         ++start;
     
     
     return 0;
     }
     @endcode
     */
    template<typename Size = size_t, typename V = std::vector<std::pair<Size,Size>> >
    V table_indices(Size m, Size n, Size initial_value_first=0, Size initial_value_second=0 ) {
        
        V v(m*n); // Construct v with m*n elements default initialized
        
        //
        for(Size i=initial_value_first;i<m;++i)
            for(Size j=initial_value_second;j<n;++j)
                v[i*n+j] = std::make_pair(i,j);    // fill with {{i,j},{i+1,j},...,{i+m-1,j},{i,j+1},...,{i,j+n-1},...}
        return v;                                   // return by value, assume RVO
    };

    /** In place sort, needs begin and end defined with random access iterator
     @tparam V is the container type
     @param v is the container to sort elements in place.
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {5,3,4,2,5,6};
     std::vector<std::vector<int>> vs {{1,2,3,1,5,3},{4,4,4,3,2},{1,2,3,4,5},{5,4,3,2,1}};
     
     dsl::sort_in_place(v);
     dsl::sort_in_place(vs);
     
     std::cout << v << std::endl;
     std::cout << vs << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output:
     @code
     {2,3,4,5,5,6}
     {{1,2,3,1,5,3},{1,2,3,4,5},{4,4,4,3,2},{5,4,3,2,1}}
     @endcode
     
     Another example
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {3,1,4,5};
     
     // Print in current order
     dsl::print(v,"\n");
     
     // decreasing order
     dsl::sort_in_place(v, [](int a, int b) { return a>b; });
     
     dsl::print(v,"\n");
     
     // increasing order
     dsl::sort_in_place(v);
     dsl::print(v,"\n");
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {3,1,4,5}
     {5,4,3,1}
     {1,3,4,5}
     @endcode
     */
    template<typename V, typename Comparison=std::less<typename V::value_type> >
    void sort_in_place(V& v, Comparison cmp = std::less<typename V::value_type>() ) {
        std::sort(std::begin(v), std::end(v), cmp);
    };
    
    
    /** replaces the values with the partial sums, needs begin and end defined
     @tparam F is the value type
     @tparam V is the container type
     @param v is the container to sort elements in place.
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {5,3,4,2,5,6};
     std::vector<std::valarray<int>> vs {{1,2,3,1,5,3},{4,4,4,3,2},{1,2,3,4,5},{5,4,3,2,1}};
     
     dsl::partial_sum_in_place(v);
     dsl::partial_sum_in_place(vs);
     
     std::cout << v << std::endl;
     std::cout << vs << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output:
     @code
     {5,8,12,14,19,25}
     {{1,2,3,1,5,3},{5,6,7,4,7,3},{6,8,10,8,12,3},{11,12,13,10,13,3}}
     @endcode
    */
    template<typename F = double, typename V = std::vector<F> >
    void partial_sum_in_place(V& v) {
        std::partial_sum(begin(v), end(v), begin(v));
    };

    /** Creates a new container with the partial sums, needs begin and end defined
     @tparam F is the value type
     @tparam V is the container type
     @param v is the container to sort elements in place.
    *//*
    template<typename F = double, typename V = std::vector<F> >
    V partial_sum(V v) {
        std::partial_sum(begin(v), end(v), begin(v));
        return v;
    };*/


    /** Returns a vector (val,val,...,val,[0],[0],...,[0]) of k vals and n-k [0]s, where [0] is the default value of the container.
        @param n is the size of the vector
        @param k is the number of vals
        @tparam val is the value to fill in
        @returns (val,val,...,val,[0],[0],...,[0]) of k vals and n-k [0]s
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::binary_row(10, 3);
     auto v2 = dsl::binary_row(10, 3,4);
     auto v3 = dsl::binary_row(10, 3,3.14);
     
     std::cout << v << std::endl;
     std::cout << v2 << std::endl;
     std::cout << v3 << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output:
     @code
     {1,1,1,0,0,0,0,0,0,0}
     {4,4,4,0,0,0,0,0,0,0}
     {3.14,3.14,3.14,0,0,0,0,0,0,0}
     @endcode
    */
    template< typename T=bool, typename Vector = std::vector<T> >
    Vector binary_row(size_t n, size_t k, T val=true) {
        // Put in tests for k>n and also separate cases k==0 and k==n.
        
        if(k > n) {
            k=n;
            std::cerr<<"binary_row number of values for default is larger than size of row\n";
        }
        
        Vector v(n);                      // Initialize values with default
        
        auto start = begin(v);            // Iterator to first element of v
        
        // Requires Forward Iterator type
        auto stop = std::next(start, k);  // Iterator to k-th element of v
        
        // Note that I did NOT write
        // auto stop = start+k;
        // If I use operator+(int), it assumes a Random Access Iterator
        
        // Requires Forward Iterator type
        std::fill(start, stop, val);        // Fill first k values with val
        
        return v;
    };
    
    /** Reverses elements
        @tparam V is any bidirectional iterator
        @param v is the container of elements
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {5,3,4,2,5,6};
     std::vector<std::valarray<int>> vs {{1,2,3,1,5,3},{4,4,4,3,2},{1,2,3,4,5},{5,4,3,2,1}};
     
     dsl::reverse_in_place(v);
     dsl::reverse_in_place(vs);
     
     std::cout << v << std::endl;
     std::cout << vs << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output:
     @code
     {6,5,2,4,3,5}
     {{5,4,3,2,1},{1,2,3,4,5},{4,4,4,3,2},{1,2,3,1,5,3}}
     @endcode
    */
    template<typename V>
    void reverse_in_place(V& v) {
        std::reverse( std::begin(v), std::end(v) );
    };
    
    
    /** Finds the smallest index excluding a set of indices, mainly used for the ordering function
        @param v is the vector of values to find the smallest value
        @param avoiding is the vector of indices which should be excluded.
        @returns the index of the smallest element not contained in input avoiding
     */
    template<typename V, typename IndexType=size_t>
    IndexType smallest_index_in_vector_avoiding(const V& v, const V& avoiding = {}) {
        
        // Initialize to index 0, start iterator at first location of v
        IndexType min_index = 0;
        auto it = std::begin(v);
        
        // Iterate ahead to the first non-avoiding position of v
        while(std::find(std::begin(avoiding), std::end(avoiding), min_index) != std::end(avoiding)) {
            ++min_index;
            ++it;
        }
        
        // get value of first candidate indexed value
        auto value = *it;
        
        // Loop through remaining indexes
        for(size_t i=min_index+1, n=v.size(); i<n; ++i) {
            ++it;
            // check if current index is in the set of avoiding indices
            if(std::find(std::begin(avoiding), std::end(avoiding), i) == std::end(avoiding))
                
                // If the current value is less than the current min value, then update min value and index
                if(*it < value) {
                    min_index = i;
                    value = *it;
                }
        }
        
        return min_index;
    }
    
    /** Ordering, returns the locations of the smallest, next smallest, etc. positions.
     Implemented to mimic Ordering[{1,2,5,4,3}] in Mathematica.
        @param v input collection of elements
        @tparam V is the data structure storing the elements
        @returns a new vector containing the indices of the ordering of the elements
     */
    template<typename V>
    V ordering(const V& v) {
        
        // Create new vector of the same size to store the resulting ranking
        V ranks(v.size());
        
        // Create new vector, store indices of smallest elements one at a time
        V avoiding;
        
        // loop through each element, find smallest, add that index to avoiding and assign its rank
        for(size_t i=0,n=v.size(); i<n; ++i) {
            auto index = smallest_index_in_vector_avoiding(v, avoiding);
            ranks[ index ] = i;
            avoiding.push_back(index);
        }
        
        return ranks;
    }
    
    
    /** Convert a permutation in the form of cycles into direct form.
        @param permutation is the permutation in the form of a collection of cycles
        @tparam IntType is the type of elements the permutation is stored as
        @tparam Container is the data structure which stores the collection of cycles
        @returns the permutation in direct form
     
     @code
     #include "numerical.h"
     #include "statistics.h"
     #include "std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::random_permutation_shifted<size_t>(10,0);
     std::cout << v << std::endl;
     
     auto p = dsl::permutation_as_product_of_cycles(v);
     std::cout << p << std::endl;
     
     auto pinv = dsl::permutation_inverse(p);
     std::cout << pinv << std::endl;
     
     auto vinv = dsl::permutation_inverse_direct_form(v);
     std::cout << vinv << std::endl;
     
     std::cout << dsl::permuted_by(v, vinv) << std::endl;
 
     return 0;
     }
     @endcode
    */
    template<typename IntType, typename Container>
    std::vector<IntType> permutation_cycles_in_direct_form(const Container& permutation) {
        
        size_t total = 0;
        
        for(auto& x : permutation)
            total += x.size();
        
        std::vector<IntType> v(total);
        
        for(auto& x : permutation) {
            if(x.size() == 1) {
                v[x[0]] = x[0];
            }
            else if(x.size() == 2) {
                v[x[0]] = x[1];
                v[x[1]] = x[0];
            }
            else { //if(x.size() >= 3) {
                for(size_t i=0,n=x.size()-1; i<n; ++i)
                    v[x[i+1]] = x[i];
                v[x[0]] = x[x.size()-1];
            }
        }
        
        return v;
    }

    
    /** Converts a permutation in the form of cycles into its inverse, which simply reverses the elements in each cycle and then rotates so that the smallest element is first.
     @param permutation is the permutation in the form of a collection of cycles
     @tparam Container is the data structure which stores the collection of cycles
     @returns the inverse permutation as a collection of cycles
     
     @code
     #include "numerical.h"
     #include "statistics.h"
     #include "std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::random_permutation_shifted<size_t>(10,0);
     std::cout << v << std::endl;
     
     auto p = dsl::permutation_as_product_of_cycles(v);
     std::cout << p << std::endl;
     
     auto pinv = dsl::permutation_inverse(p);
     std::cout << pinv << std::endl;
     
     auto vinv = dsl::permutation_inverse_direct_form(v);
     std::cout << vinv << std::endl;
     
     std::cout << dsl::permuted_by(v, vinv) << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename Container>
    Container permutation_inverse(Container permutation) {
        
        for(auto& x : permutation) {
            std::reverse(std::begin(x), std::end(x));
            std::rotate(std::begin(x), std::end(x)-1, std::end(x));
        }
        
        return permutation;
    }

    
    
    /** Convert a permutation in direct form into its inverse in direct form.
     @param permutation is the permutation in direct form
     @tparam IntType is the type of elements the permutation is stored as
     @returns the inverse permutation in direct form
     
     @code
     #include "numerical.h"
     #include "statistics.h"
     #include "std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::random_permutation_shifted<size_t>(10,0);
     std::cout << v << std::endl;
     
     auto p = dsl::permutation_as_product_of_cycles(v);
     std::cout << p << std::endl;
     
     auto pinv = dsl::permutation_inverse(p);
     std::cout << pinv << std::endl;
     
     auto vinv = dsl::permutation_inverse_direct_form(v);
     std::cout << vinv << std::endl;
     
     std::cout << dsl::permuted_by(v, vinv) << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename IntType>
    std::vector<IntType> permutation_inverse_direct_form(std::vector<IntType> permutation) {
        
        auto cycles = permutation_as_product_of_cycles<IntType, std::vector<std::vector<IntType>> >(permutation);
        cycles = permutation_inverse<std::vector<std::vector<IntType>>>(cycles);
        
        permutation = permutation_cycles_in_direct_form<IntType, std::vector<std::vector<IntType>>>(cycles);
        
        return permutation;
        
    }
    
    
    /** Permute the elements in a vector by a permutation in direct form
        @param v is the collection of elements to permute
        @param permutation is the permutation in the form of a collection of cycles
        @tparam Vector is the data structure which stores the collection of elements, must be random access.
     
     @code
     #include "numerical.h"
     #include "statistics.h"
     #include "std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::random_permutation_shifted<size_t>(10,0);
     std::cout << v << std::endl;
     
     auto p = dsl::permutation_as_product_of_cycles(v);
     std::cout << p << std::endl;
     
     auto pinv = dsl::permutation_inverse(p);
     std::cout << pinv << std::endl;
     
     auto vinv = dsl::permutation_inverse_direct_form(v);
     std::cout << vinv << std::endl;
     
     std::cout << dsl::permuted_by(v, vinv) << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename Vector>
    void permute_by(Vector& v, Vector& permutation) {
        
        auto v2 = v;
        
        for(size_t i=0,n=v.size(); i<n; ++i) {
            v2[permutation[i]] = v[i];
        }
        
        v = v2;
    }
    
    /** Permute the elements in a vector by a permutation in direct form
     @param v is the collection of elements to permute
     @param permutation is the permutation in the form of a collection of cycles
     @tparam Vector is the data structure which stores the collection of elements, must be random access.
     
     @code
     #include "numerical.h"
     #include "statistics.h"
     #include "std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::random_permutation_shifted<size_t>(10,0);
     std::cout << v << std::endl;
     
     auto p = dsl::permutation_as_product_of_cycles(v);
     std::cout << p << std::endl;
     
     auto pinv = dsl::permutation_inverse(p);
     std::cout << pinv << std::endl;
     
     auto vinv = dsl::permutation_inverse_direct_form(v);
     std::cout << vinv << std::endl;
     
     std::cout << dsl::permuted_by(v, vinv) << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename Vector>
    Vector permuted_by(Vector v, Vector& permutation) {
        
        permute_by(v, permutation);
        return v;
    }
    
    
    
    
    /** Calculates the factorial on input of type T and outputs in type F
     @tparam T is the input integer type
     @tparam F is the output integer type
     @param n is the value for which to take the factorial
     @returns n! in data type F
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     // typdef for large unsigned integer
     typedef unsigned long long ull;
     
     int main(int argc, const char * argv[]) {
     
     // We must be careful!  By default we get a signed integer type, which is not defined in cases of overflow
     auto x = dsl::factorial(20);
     std::cout << x << std::endl; // -2102132736   <-- overflow!
     
     // the function is templated so that you can specify the data type for which the calculations will be performed, in this case an unsigned long long is large enough.
     auto x2 = dsl::factorial<ull>(20);
     std::cout << x2 << std::endl;   // 2432902008176640000   <-- Ok!
     
     // Make sure you know what you are doing!  This answer returned is actually 30! % ULLONG_MAX.  This behavior is in fact well-defined for unsigned types in C++, but you should always be careful when doing calculations involving extremely fast-growing numerical values.
     auto x3 = dsl::factorial<ull>(30);
     std::cout << x3 << std::endl;   // 9682165104862298112  <-- 30! % ULLONG_MAX
     
     return 0;
     }
     @endcode
     Should produce output (depending on numeric limits)
     @code
     -2102132736
     2432902008176640000
     9682165104862298112
     @endcode
     */
    template<typename T, typename F=T>
    F factorial(T n) {
        if(n == 0) return 1;
        else if(n <= 2) return n;
        else {
            // Do arithmetic in F
            F fact = static_cast<F>(2);
            F f_n = static_cast<F>(n);
            for(F i=f_n; i>=3; --i) {
                fact *= i;
                std::cout << i << std::endl;
                std::cout << fact.bit.size() << std::endl;
            }
            return fact;
        }
    };

    
    /** calculates n!/k! = n(n-1)...(n-k+1), intermediate calculations are done in F
     @tparam T is the input type
     @tparam F is the output type
     @param n is the larger value
     @param k is the smaller value
     @returns n!/k!
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     // typdef for large unsigned integer
     typedef unsigned long long ull;
     
     int main(int argc, const char * argv[]) {
     
     // We must be careful!  By default we get a signed integer type, which is not defined in cases of overflow
     auto x = dsl::nfallingk(50,25);
     std::cout << x << std::endl; // -1040187392   <-- overflow!
     
     // the function is templated so that you can specify the data type for which the calculations will be performed, in this case an unsigned long long is large enough.
     auto x2 = dsl::nfallingk<ull>(50,25);
     std::cout << x2 << std::endl;   // 15560789850943651840   <-- Ok!
     
     // Make sure you know what you are doing!  This answer returned is actually 30! % ULLONG_MAX.  This behavior is in fact well-defined for unsigned types in C++, but you should always be careful when doing calculations involving extremely fast-growing numerical values.
     auto x3 = dsl::nfallingk<ull>(1000,50);
     std::cout << x3 << std::endl;   // 10657768518172278784  <-- (1000)_(50) % ULLONG_MAX
     
     return 0;
     }
     @endcode
     Should produce output (depending on numeric limits):
     @code
     -1040187392
     15560789850943651840
     10657768518172278784
     @endcode
     */
    template<typename T1, typename T2, typename F=T1>
    F nfallingk(T1 n, T2 k) {

        if(k<0 || k>n) return 0;
        if(k == 0) return 1;
        // Cast all parameters to F
        F fact = static_cast<F>(1);
        F f_n = static_cast<F>(n);
        F f_k = static_cast<F>(k);
        
        for(F i=f_n; i>f_n-f_k; --i) {
            fact *= i;
        }
        
        return static_cast<F>(fact);
    };
    
    
    
    /** calculates the binomial coefficient
     @tparam T is the input type
     @tparam F is the output type
     @param n is the larger value
     @param k is the smaller value
     @returns n choose k
     
     Example 1:
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     // typdef for large unsigned integer
     typedef unsigned long long ull;
     
     int main(int argc, const char * argv[]) {
     
     // We must be careful!  By default we get a signed integer type, which is not defined in cases of overflow
     auto x = dsl::binomial(50,10);
     std::cout << x << std::endl; // 106   <-- overflow!
     
     // the function is templated so that you can specify the data type for which the calculations will be performed, in this case an unsigned long long is large enough.
     auto x2 = dsl::binomial<ull>(50,10);
     std::cout << x2 << std::endl;   // 10272278170   <-- Ok!
     
     // Make sure you know what you are doing!  I get a run-time error since binomial calls nfallingk(n,k)/factorial(k), and in this case, nfallingk(n,k) has overflow and returns 0, and factorial(k) has overflow, and returns 0.  The reason is because ULLONG_MAX is a multiple of 2, typically 2^64, and so after 64 factors of 2 have been multiplied together, we obtain a multiple of 2^64, hence taking modulo 2^64 gives 0.
     // So the error is in attempting to compute 0/0.  Ask Siri if you don't understand the implications.
     auto x3 = dsl::binomial<ull>(1000,100);
     std::cout << x3 << std::endl;   // Run-time error at this point for me.
     
     return 0;
     }
     @endcode
     Should produce output (depending on numeric limits)
     @code
     106
     10272278170
     (run-time error occurs)
     @endcode
     
     Example 2:
     @code
     #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
     
     int main(int argc, const char * argv[]) {
        std::cout << dsl::binomial(10,-2) << std::endl;
        std::cout << dsl::binomial(0,-2) << std::endl;
        std::cout << dsl::binomial(1,0) << std::endl;
        std::cout << dsl::binomial(0,1) << std::endl;
        std::cout << dsl::binomial(0,2) << std::endl;
        std::cout << dsl::binomial(0,-10) << std::endl;
        std::cout << dsl::binomial(-5,-2) << std::endl;
     
        return 0;
     }
     @endcode
     Should produce output
     @code
     0
     0
     1
     0
     0
     0
     0
     0
     @endcode
     */
    template<typename T1, typename T2, typename F=T1>
    F binomial(T1 n, T2 k) {
        
        return (nfallingk<T1,T2,F>(n,k)/factorial<T2,F>(k));
    }
    
    /** calculates n(n-1)/2, a common binomial coefficient
     @tparam T is the input type
     @tparam F is the output type
     @param n is the number of elements to choose 2 from
     @returns n choose 2
     
     @code
     
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     // typdef for large unsigned integer
     typedef unsigned long long ull;
     
     int main(int argc, const char * argv[]) {
     
     // We must be careful!  By default we get a signed integer type, which is not defined in cases of overflow
     auto x = dsl::choose2(1234567);
     std::cout << x << std::endl; // -279473579   <-- overflow!
     
     // the function is templated so that you can specify the data type for which the calculations will be performed, in this case an unsigned long long is large enough.
     auto x2 = dsl::choose2<ull>(1234567);
     std::cout << x2 << std::endl;   // 762077221461   <-- Ok!
     
     // Make sure you know what you are doing!
     auto x3 = dsl::choose2<ull>(1234567891011);
     std::cout << x3 << std::endl;   // 7047583967906995363  <-- modulo 2^64
     
     // Pop quiz:  Is this the right answer?
     std::cout << sqrt(ULLONG_MAX) << std::endl;  // 4.29497e+09   <-- take the n choose 2 of this
     auto x4 = dsl::choose2<ull>( sqrt(ULLONG_MAX) );
     std::cout << x4 << std::endl;   // 9223372034707292160  <-- Is this the actual answer?
     
     return 0;
     }
     @endcode
     Should produce output (depending on numeric limits)
     @code
     -279473579
     762077221461
     7047583967906995363
     4.29497e+09
     9223372034707292160
     @endcode
     */
    template<typename T, typename F=T>
    F choose2(T n) {
        
        // Do arithmetic in F
        auto f_n = static_cast<F>(n);
        auto f_n_minus_1 = static_cast<F>(n-1);
        
        // n(n-1)/2, always an integer
        return f_n * f_n_minus_1 / 2;
    }
    
    /** calculates n(n-1)(n-2)/3, a common binomial coefficient
     @tparam T is the input type
     @tparam F is the output type
     @param n is the number of elements to choose 3 from
     @returns n choose 3
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     // typdef for large unsigned integer
     typedef unsigned long long ull;
     
     int main(int argc, const char * argv[]) {
     
     // We must be careful!  By default we get a signed integer type, which is not defined in cases of overflow
     auto x = dsl::choose3(1234567);
     std::cout << x << std::endl; // -230422855   <-- overflow!
     
     // the function is templated so that you can specify the data type for which the calculations will be performed, in this case an unsigned long long is large enough.
     auto x2 = dsl::choose3<ull>(1234567);
     std::cout << x2 << std::endl;   // 313611288304333155   <-- Ok!
     
     // Make sure you know what you are doing!
     auto x3 = dsl::choose3<ull>(123456789);
     std::cout << x3 << std::endl;   // 2699470946007441500  <-- n choose 3 modulo 2^64
     
     return 0;
     }
     @endcode
     Should produce output (depending on numerical limits)
     @code
     -230422855
     313611288304333155
     2699470946007441500
     @endcode
     */
    template<typename T, typename F=T>
    F choose3(T n) {

        // Do arithmetic in F
        auto f_n = static_cast<F>(n);
        auto f_n_minus_1 = static_cast<F>(n-1);
        auto f_n_minus_2 = static_cast<F>(n-2);
        
        // n(n-1)(n-2)/6
        return f_n * f_n_minus_1 * f_n_minus_2 / 6;
    }
    
    /** calculates n choose 4, a common binomial coefficient
     @tparam T is the input type
     @tparam F is the output type
     @param n is the number of elements to choose 4 from
     @returns n choose 4
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     // typdef for large unsigned integer
     typedef unsigned long long ull;
     
     int main(int argc, const char * argv[]) {
     
     // We must be careful!  By default we get a signed integer type, which is not defined in cases of overflow
     auto x = dsl::choose4(12345);
     std::cout << x << std::endl; // -69762984   <-- overflow!
     
     // the function is templated so that you can specify the data type for which the calculations will be performed, in this case an unsigned long long is large enough.
     auto x2 = dsl::choose4<ull>(12345);
     std::cout << x2 << std::endl;   // 967257345895170   <-- Ok!
     
     // Make sure you know what you are doing!
     auto x3 = dsl::choose4<ull>(123456789);
     std::cout << x3 << std::endl;   // 98740589912719051  <-- n choose 4 modulo 2^64
     
     return 0;
     }
     @endcode
     Should produce output (depending on numerical limits)
     @code
     -69762984
     967257345895170
     98740589912719051
     @endcode
     */
    template<typename T, typename F=T>
    F choose4(T n) {
     
        // Do arithmetic in F
        auto f_n = static_cast<F>(n);
        auto f_n_minus_1 = static_cast<F>(n-1);
        auto f_n_minus_2 = static_cast<F>(n-2);
        auto f_n_minus_3 = static_cast<F>(n-3);
        
        // n(n-1)(n-2)(n-3)/24
        return f_n * f_n_minus_1 * f_n_minus_2 * f_n_minus_3 / 24;
    }
    

    
    /** Calculates the binomial probability (n choose k) p^k (1-p)^(n-k) in a way as numerically stable as I could make it for now.
     @param n is the number of trials
     @param k is the number of successes
     @param p is the probability of success
     @tparam N is the integer type
     @tparam T is the floating point type
     @tparam F is the return type
     @returns the probability of k successes in n trials with probability of success p.
     
     Example 1:
     @code
     #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
     
     int main(int argc, const char * argv[]) {
     
     for(int i=0;i<=100;++i)
     std::cout << dsl::binomial_probability(100,i,0.5) << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename N, typename T, typename F = T>
    F binomial_probability(N n, N k, T p) {
        
        // check for range of p
        if(p<0 || p > 1) {
            std::cerr << "ERROR in dsl::binomial_probability("<<n<<","<<k<<","<<p<<"): p is negative\n";
            return NAN;
        }
        
        // check for trivial ranges
        if(k>n || k < 0) return 0;
        if(k == n) return std::pow(1-p,n);
        if(k == 0) return std::pow(p,n);
        
        
        // you know it's hard out there for a ...
        double p1mp = p*(1.-p);
        
        double answer = 1.;
        
        // split up depending on k
        if( k < n/2) {
            
            // n p (1-p) / k * (n-1) p (1-p) / (k-1) * ... * (n-k+1) p (1-p) / 1
            for(size_t i=0;i<k;++i) {
                answer *= (n-i)*p1mp / (k-i);
            }
            
            // finish off remaining powers of 1-p
            for(size_t i=0;i<n-2*k;++i)
                answer *= (1.-p);
            
        }
        else { // k >= n/2
            
            // n p (1-p) / k * (n-1) p (1-p) / (k-1) * ... * (n-k+1) p (1-p) / 1
            for(size_t i=0;i<n-k;++i) {
                answer *= (n-i)*p1mp / (k-i);
            }

            // finish remaining terms without extra 1-p factors, i.e.,
            // // (n - i) p / (k-i) ...
            for(size_t i=n-k;i<k;++i)
                answer *= (n-i)*p/(k-i);
            
        }
        
        return answer;
        
    }
    

    
    
    
    /** Simply prints the elements of two containers side-by-side separated by some string separator, where the second container needs at least as many elements as teh first container.  Needs InputIterator.
     @tparam V is a container with an input iterator
     @tparam C is a container with an input iterator
     @param left is a container of elements for the left column
     @param right is a container of elements for the right column
     @param sep is the string that separates the two entries per line.
     @param endline is the string that separates each pair of entries.
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {3,1,4,1,5,9,2,6,5};
     std::vector<double> v2 {3.1,4.1,5.9,2.6,5.3,5.8,9.7,9.3,2.3,8.6};
     std::set<double> s;
     
     // insert elements of v2 into a set s, which orders them
     for(auto& x : v2) s.insert(x);
     
     // v2 needs to have as many elements as v, prints as many elements as there are in v.
     dsl::print_side_by_side(v,v2);
     std::cout << std::endl << std::endl;
     
     // v and s need only provide input iterators
     dsl::print_side_by_side(v,s);
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     3  3.1
     1  4.1
     4  5.9
     1  2.6
     5  5.3
     9  5.8
     2  9.7
     6  9.3
     5  2.3
     
     
     3  2.3
     1  2.6
     4  3.1
     1  4.1
     5  5.3
     9  5.8
     2  5.9
     6  8.6
     5  9.3
     @endcode
     */
    template<typename V, typename C>
    inline void print_side_by_side(const V& left,const C& right, const std::string& sep=std::string("  "), const std::string& endline = std::string("\n")) {
        
        auto start = std::begin(left);
        auto stop = std::end(left);
        //auto last = (--stop)++;
        
        auto cstart = std::begin(right);
        
        while(start != stop) {
            std::cout << *start << sep << *cstart << endline;
            ++start; ++cstart;
        }
    }
    
    /** Simply prints the elements of two containers side-by-side separated by some string separator, where the second container needs at least as many elements as teh first container.  Needs InputIterator.
     @tparam InputIterator1 is any input iterator
     @tparam InputIterator2 is any input iterator
     @param left is a container of elements for the left column
     @param right is a container of elements for the right column
     @param sep is the string that separates the two entries per line.
     @param endline is the string that separates each pair of entries.
     
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {3,1,4,1,5,9,2,6,5};
     std::vector<double> v2 {3.1,4.1,5.9,2.6,5.3,5.8,9.7,9.3,2.3,8.6};
     std::set<double> s;
     
     // insert elements of v2 into a set s, which orders them
     for(auto& x : v2) s.insert(x);
     
     // Can also input by iterators
     dsl::print_side_by_side(std::begin(v)+3, std::end(v), std::begin(v2));
     std::cout << std::endl << std::endl;
     
     // Can mix and match iterators of different types, as long as input iterators
     dsl::print_side_by_side(std::begin(v), std::end(v)-5, std::begin(s));
     
     
     return 0;
     }
     @endcode
     Should produde output
     @code
     1  3.1
     5  4.1
     9  5.9
     2  2.6
     6  5.3
     5  5.8
     
     
     3  2.3
     1  2.6
     4  3.1
     1  4.1
     @endcode
     */
    template<typename InputIterator1, typename InputIterator2>
    void print_side_by_side(InputIterator1 start1, InputIterator1 stop, InputIterator2 start2, const std::string& sep=std::string("  "), const std::string& endline = std::string("\n")) {
        
        while(start1 != stop) {
            std::cout << *start1 << sep << *start2 << endline;
            ++start1; ++start2;
        }
    }

    /** Calculate the sums of powers of a collection of objects
     @param start is an iterator to starting value
     @param stop is an iterator to one after the last value
     @param initial is the initial value to add to the sum
     @tparam power is the exponent to raise the values to
     @returns x_1^a+x_2^a+...+x_n^a, the sums of powers of elements
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {3,1,4,1,5,9,2,6,5};
     std::vector<double> v2 {3.1,4.1,5.9,2.6,5.3,5.8,9.7,9.3,2.3,8.6};
     
     // Sum of squares
     auto x = dsl::sum_of_powers(std::begin(v), std::end(v), 2);
     
     // Sum of 4th powers
     auto x2 = dsl::sum_of_powers(std::begin(v2), std::end(v2), 4);
     
     std::cout << x << std::endl;
     std::cout << x2 << std::endl;
     
     return 0;
     }
     @endcode
     Shoudl produce output
     @code
     198
     25384.6
     @endcode
     */
    template<typename ReturnValueType = double, typename IntegerType = long long int, typename DataType = ReturnValueType, typename InputIterator = typename std::vector<DataType>::iterator>
    ReturnValueType sum_of_powers(InputIterator start, InputIterator stop, IntegerType power, DataType initial=0.) {
        
        ReturnValueType sum = static_cast<ReturnValueType>(initial);
        
        while(start != stop) {
            sum += pow(*start, power);
            ++start;
        }
        return sum;
        
    };
    

    /** Calculates and returns the set of all permutations as a vector of vectors.
     @tparam T is the type of object
     @param objects is some collection of objects
     @returns the set of all permutations of objects.
     @code
     
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {3,1,4,5};
     
     auto x = dsl::permutations(v);
     
     dsl::print(x); // equivalent to std::cout << x;
     dsl::print("\n\n\n");
     
     dsl::sort_in_place(x);
     dsl::print(x);
     
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {{1,4,5,3},{4,1,5,3},{4,5,1,3},{5,4,1,3},{1,5,4,3},{5,1,4,3},{5,4,3,1},{4,5,3,1},{4,3,5,1},{3,4,5,1},{5,3,4,1},{3,5,4,1},{1,5,3,4},{5,1,3,4},{5,3,1,4},{3,5,1,4},{1,3,5,4},{3,1,5,4},{1,4,3,5},{4,1,3,5},{4,3,1,5},{3,4,1,5},{1,3,4,5},{3,1,4,5}}
     
     
     {{1,3,4,5},{1,3,5,4},{1,4,3,5},{1,4,5,3},{1,5,3,4},{1,5,4,3},{3,1,4,5},{3,1,5,4},{3,4,1,5},{3,4,5,1},{3,5,1,4},{3,5,4,1},{4,1,3,5},{4,1,5,3},{4,3,1,5},{4,3,5,1},{4,5,1,3},{4,5,3,1},{5,1,3,4},{5,1,4,3},{5,3,1,4},{5,3,4,1},{5,4,1,3},{5,4,3,1}}
     @endcode
     */
    template<typename T>
    std::vector< std::vector<T> > permutations(std::vector<T> objects) {
        
        // Grows like 2^n, so cannot handle much larger than 32 objects on most computers...I wouldn't try it for more than about 10.
        size_t n = objects.size();
        size_t index = 0;
        
        std::vector< std::vector<T> > perms( factorial(n) );
        
        //vector<T> v(objects);
        if(n == 0) return perms;
        if(n == 1) {
            perms[0] = objects;
            return perms;
        }
        
        // For each element, take out, find all permutations of remaining elements, put back in, union everything.
        for(size_t i=0; i<n; ++i) {
            
            // Get i-th element
            T a = objects[i];
            
            // Make a copy without the i-th element
            std::vector<T> copy(objects);
            copy[i] = copy[n-1];
            copy.resize(n-1);     // faster than pop_back() ?
            
            // Find permutations on objects without i-th element
            std::vector<std::vector<T> > p = permutations(copy);
            
            // put the value i back into the permutation.
            for(auto& x : p) {
                x.push_back(a);
                perms[index++] = x;
                
            }
        }
        
        return perms;
        
    };
    
    /** 
     Converts the digits in the input into single characters in a collection of numbers
     @tparam IntegerType is any unsigned integer type
     @tparam ContainerType is the container to store digits
     @param a is the input value
     @returns a collection of digits of a stored individually
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     int x = 314159265;
     auto v = dsl::int_to_digits(x);
     std::cout << v << std::endl;
     
     v = dsl::int_to_digits(x, false);
     std::cout << v << std::endl;
     
     // Just watch out for overflow!
     int x2 = 314159265358979;
     auto v2 = dsl::int_to_digits(x2);
     dsl::print(v2);
     
     return 0;
     }
     @endcode
     Should produce output (depending on numerical limits)
     @code
     {3,1,4,1,5,9,2,6,5}
     {5,6,2,9,5,1,4,1,3}
     {4,1,2,4,7,4,2,3,7}
     @endcode
     */
    template<typename IntegerType, typename ContainerType=std::vector<IntegerType>>
    ContainerType int_to_digits(IntegerType a, bool left_to_right = true) {
        
        ContainerType digits;
        
        // Check various extreme cases
        
        // a = 0
        if(a==0) {
            digits.push_back(0);
            return digits;
        }
        
        // just convert to positive number
        if(a < 0) a = -a;
        
        while(a) {
            digits.push_back(a%10);
            a /= 10;
        }
        if(left_to_right)
            std::reverse(begin(digits), end(digits));
        
        return digits;
    }
    
    
    /**
     Converts the digits in the input container into a single number
     @tparam IntegerType is any unsigned integer type
     @tparam ContainerType is the container to store digits
     @param digits is a collection of digits of a stored individually
     @returns a numerical value for which the digits in digits were representing (base 10)
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     // converts digits into a single value base 10.
     std::vector<int> v {3,1,4,1,5,9,2,6,5};
     auto x = dsl::digits_to_int(v);
     dsl::print(x,"\n");
     
     // There is no check for overflow, but ...
     std::vector<int> v2 {1,2,3,4,5,6,7,8,9,1,0,1,1,1};
     auto x2 = dsl::digits_to_int(v2);
     dsl::print(x2,"\n");
     
     // You can at least be a little smart about it and give a larger container
     std::vector<int> v3 {1,2,3,4,5,6,7,8,9,1,0,1,1,1};
     auto x3 = dsl::digits_to_int<unsigned long long>(v3);
     dsl::print(x3,"\n");
     
     // Reverse, reverse!
     x3 = dsl::digits_to_int<unsigned long long>(v3, false);
     dsl::print(x3,"\n");
     
     
     
     return 0;
     }
     @endcode
     Should produce output (depending on numerical limits)
     @code
     314159265
     1942901407
     12345678910111
     11101987654321
     @endcode
     */
     template<typename IntegerType=int, typename ContainerType=std::vector<IntegerType>>
    IntegerType digits_to_int(ContainerType digits, bool is_left_to_right = true) {
        
        IntegerType value = 0;
        
        // Check for endian-ness
        if(is_left_to_right) {  // If stored {b1,b2,b3,...,bn}
            auto it = digits.rbegin();
            auto stop = digits.rend();
            
            ull base = 1;
        
            while(it != stop) {
                value += *it * base;
                ++it;
                base*=10;
            }
            // Check various extreme cases
            return value;
        }
        else { // stored as {bn,bn-1,...,b2,b1}
            auto it = digits.begin();
            auto stop = digits.end();
            
            ull base = 1;
            
            while(it != stop) {
                value += *it * base;
                ++it;
                base*=10;
            }
            // Check various extreme cases
            return value;
        }
            
    }
    
    /** Specifically checks if n numbers {a,b,c,d,...} are a permutation of {1,2,3,...,n}.  There are very fast checksum ways of determining this, otherwise there is a generic algorithm that will work for larger n and even for more general objects
        @tparam Iterator is the input iterator type
        @tparam IntegerType is the type of n
        @param start refers to the first element
        @param stop refers to one after the last element
        @param n is the largest value in the set
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,2,3,4,5,6,7,8,9};
     std::vector<int> v2 {1,9,2,8,3,7,4,6,5};
     std::vector<int> v3 {4,3,5,2,6,7,3};
     
     // WARNING!  There is no check against values outside of the values 1,...,n
     std::vector<int> v4 {-1,4,-1};
     
     std::cout << v << " is permutation of 9: " << (dsl::is_permutation_of_n(std::begin(v), std::end(v), 9) ? "yes" : "no") << std::endl;
     std::cout << v2 << " is permutation of 9: " << (dsl::is_permutation_of_n(std::begin(v2), std::end(v2), 9) ? "yes" : "no")<< std::endl;
     std::cout << v3 << " is permutation of 7: " << (dsl::is_permutation_of_n(std::begin(v3), std::end(v3), 7) ? "yes" : "no")<< std::endl;
     std::cout << v4 << " is permutation of 3: " << (dsl::is_permutation_of_n(std::begin(v4), std::end(v4), 3) ? "yes" : "no")<< std::endl;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9} is permutation of 9: yes
     {1,9,2,8,3,7,4,6,5} is permutation of 9: yes
     {4,3,5,2,6,7,3} is permutation of 7: no
     {-1,4,-1} is permutation of 3: no
     @endcode
     */
    template<typename Iterator, typename IntegerType>
    bool is_permutation_of_n(Iterator start, const Iterator& stop, IntegerType n) {
        
        IntegerType a,b,c,d,e,f,g,h,i,j;
        
        // For n <= 10, if we can assume the permutation means {1,2,...,n}, there are efficient ways of determining the answer.
        
        // Through numerical experiments, found the following sufficient conditions for establishing whether a set of POSITIVE integers is a permutation of 1,...,n for small n
        switch (n) {
            case 0: return 1; break;
            case 1: return n == 1; break;
            case 2:
                a = *start++;
                b = *start;
                return (a+b == 3 && a*b == 2);
            case 3:
                a = *start++;
                b = *start++;
                c = *start;
                return (a+b+c == 6 && a*b*c == 6);
            case 4:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start;
                return (a+b+c+d == 10 && a*b*c*d == 24);
            case 5:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start++;
                e = *start;
                return (a+b+c+d+e == 15 && a*b*c*d*e == 120);
            case 6:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start++;
                e = *start++;
                f = *start;
                return (a+b+c+d+e+f == 21 && a*b*c*d*e*f == 720);
            case 7:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start++;
                e = *start++;
                f = *start++;
                g = *start;
                return (a+b+c+d+e+f+g == 28 && a*b*c*d*e*f*g == 5040);
            case 8:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start++;
                e = *start++;
                f = *start++;
                g = *start++;
                h = *start;
                return (a+b+c+d+e+f+g+h == 36 && a*b*c*d*e*f*g*h == 40320);
            case 9:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start++;
                e = *start++;
                f = *start++;
                g = *start++;
                h = *start++;
                i = *start;
                return (a+b+c+d+e+f+g+h+i == 45 && a*b*c*d*e*f*g*h*i == 362880 && a*a+b*b+c*c+d*d+e*e+f*f+g*g+h*h+i*i == 285);
            case 10:
                a = *start++;
                b = *start++;
                c = *start++;
                d = *start++;
                e = *start++;
                f = *start++;
                g = *start++;
                h = *start++;
                i = *start++;
                j = *start;
                return (a+b+c+d+e+f+g+h+i+j == 55 && a*b*c*d*e*f*g*h*i*j == 3628800 && a*a+b*b+c*c+d*d+e*e+f*f+g*g+h*h+i*i+j*j == 385);
        }
        
        
        // if n > 10, do it the old fashion way.
        std::vector<IntegerType> v(n);
        std::iota(std::begin(v), std::end(v), 1);
        
        return std::is_permutation(start, stop, std::begin(v));
        
    }
    
    
    /** This sorts a copy of the container and then applies unique to test for uniqueness.  Not the greatest algorithm.
        @tparam Container is a container of elements
        @tparam BinaryPredicate is any function object which has operator(T,T)->bool overloaded, where T=Container::value_type is the type of object contained in objects of type Container
        @tparam Comparison is any function object which has operator< or operator(T,T)->bool that compares using less operation
        @param elements is the collection of objects
        @param pred is a function object with operator(T,T)->bool overloaded
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,2,3,4,5,6,7,8,9};
     std::vector<int> v2 {1,9,2,8,3,7,4,6,5};
     std::vector<int> v3 {4,3,5,2,6,7,3};
     
     // WARNING!  There is no check against values outside of the values 1,...,n
     std::vector<int> v4 {-1,4,-1};
     
     std::cout << v << " has unique elements: " << (dsl::has_unique_elements(v) ? "yes" : "no") << std::endl;
     std::cout << v2 << " has unique elements: " << (dsl::has_unique_elements(v2) ? "yes" : "no")<< std::endl;
     std::cout << v3 << " has unique elements: " << (dsl::has_unique_elements(v3) ? "yes" : "no")<< std::endl;
     std::cout << v4 << " has unique elements: " << (dsl::has_unique_elements(v4) ? "yes" : "no")<< std::endl;
     
     // Generate all permutations of {-1,4,-1}
     std::cout << std::endl;
     auto sv = dsl::permutations(v4);
     dsl::print(sv, "\n\n");
     
     // Check if any pair of the permutations are the same coordinate-wise...should be 0.
     std::cout << "Do any of those pairs of permutations have a difference whose sum of squares is 0? " << (dsl::has_unique_elements(sv, [](const std::vector<int>& a, const std::vector<int>& b) {
     int ss = 0; // sum of squares initialize
     for(size_t i=0,n=a.size();i<n;++i)
     ss += (a[i]-b[i])*(a[i]-b[i]);
     return ss!=0;
     }) ? "yes" : "no") << std::endl << std::endl;
     
     // Check if all pairs of the permutations have the same sum of squares...should be 1.
     std::cout << "Do any of those permutations have the same sum of squares? " << (dsl::has_unique_elements(sv, [](const std::vector<int>& a, const std::vector<int>& b)->bool {
     int ssa = 0; // sum of squares initialize
     int ssb = 0;
     for(size_t i=0,n=a.size();i<n;++i) {
     ssa += a[i]*a[i];
     ssb += b[i]*b[i];
     }
     return ssa!=ssb;
     }) ? "yes" : "no") << std::endl;
     
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9} has unique elements: yes
     {1,9,2,8,3,7,4,6,5} has unique elements: yes
     {4,3,5,2,6,7,3} has unique elements: no
     {-1,4,-1} has unique elements: no
     
     {{4,-1,-1},{-1,4,-1},{-1,-1,4},{-1,-1,4},{4,-1,-1},{-1,4,-1}}
     
     Do any of those pairs of permutations have a difference whose sum of squares is 0? no
     
     Do any of those permutations have the same sum of squares? yes
     @endcode
     */
    template<typename Container, typename BinaryPredicate = std::equal_to<typename Container::value_type>, typename Comparison = std::less<typename Container::value_type>>
    bool has_unique_elements(Container elements, BinaryPredicate pred = std::equal_to<typename Container::value_type>(), Comparison cmp=std::less<typename Container::value_type>()) {
        
        auto first = std::begin(elements);
        auto last  = std::end(elements);
        
        // Empty list by default has unique elements
        if (first==last) return true;
        
        // sort list so only have to check adjacent elements, makes algorithm O(n log(n))
        std::sort(first, last, cmp);
        
        // Now check adjacent elements
        auto result = first;
        while (++first != last)
        {
            //            if (!(*result == *first))  // or: if (!pred(*result,*first)) for version (2)
            if (pred(*result++, *first))  // or: if (!pred(*result,*first)) for version (2)
                return false;
            
        }
        return true;
         
    }
    
    
    /** Code taken from Cracking The Code Interview book.  Very fast, cryptic.
        @tparam UnsignedIntegers is any collection with forward iterator
        @param values is the set of values to check for
        @returns true if all characters are unique from 0 to 31
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,2,3,4,5,6,7,8,9};
     std::vector<int> v2 {1,9,2,8,3,7,4,6,5};
     std::vector<int> v3 {4,3,5,2,6,7,3};
     std::vector<int> v4 {4,0,5,-2,6,7,3};
     
     std::cout << dsl::is_unique_uints_max_31(v) << std::endl;
     std::cout << dsl::is_unique_uints_max_31(v2) << std::endl;
     std::cout << dsl::is_unique_uints_max_31(v3) << std::endl;
     
     // one element is negative, so unpredictable behavior
     std::cout << dsl::is_unique_uints_max_31(v4) << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     1
     1
     0
     1
     @endcode
    */
    template<typename UnsignedIntegers>
    bool is_unique_uints_max_31(UnsignedIntegers values) {
        unsigned int checker = 0;
        
        // magic ...
        for (auto val : values) {
            if ((checker & (1 << val)) > 0)
                return false;
            checker |= (1 << val);
        }
        return true;
    }
    
    /** Code taken from Cracking The Code Interview book.  Very fast, cryptic.
     @tparam ForwardIterator is any collection with input iterator
     @param values is the set of values to check for
     @returns true if all characters are unique from 0 to 31
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,2,3,4,5,6,7,8,9};
     std::vector<int> v2 {1,9,2,8,3,7,4,6,5};
     std::vector<int> v3 {4,3,5,2,6,7,3};
     std::vector<int> v4 {4,0,5,-2,6,7,3};
     
     std::cout << dsl::is_unique_uints_max_31(std::begin(v), std::end(v)) << std::endl;
     std::cout << dsl::is_unique_uints_max_31(std::begin(v2), std::end(v2)) << std::endl;
     std::cout << dsl::is_unique_uints_max_31(std::begin(v3), std::end(v3)) << std::endl;
     
     // one element is negative, so unpredictable behavior
     std::cout << dsl::is_unique_uints_max_31(std::begin(v4), std::end(v4)) << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     1
     1
     0
     1
     @endcode
     */
    template<typename ForwardIterator>
    bool is_unique_uints_max_31(ForwardIterator first, ForwardIterator last) {
        unsigned int checker = 0;
        
        // magic ...
        while(first != last) {
            
            if ((checker & (1 << *first)) > 0)
                return false;
            checker |= (1 << *first++);
        }
        return true;
    }

    /** Finds the conjugate partition for a given input, entries can be in any order.  The container needs RANDOM ACCESS iterators in order to guarantee that the entries are in sorted order.  Otherwise bidirectional is sufficient if already sorted.
     
     @tparam F is the value type
     @tparam V is the container type
     @param v is an integer vector of values
     @returns the conjugate partition in descending order
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,1,1,2,2,3,4,5,5,5,6,6,8,9,10,11};
     std::vector<int> v2 {1,1,2,4,7};
     std::vector<int> v3 {1,10,5};
     
     auto s = dsl::conjugate_integer_partition(v);
     auto s2 = dsl::conjugate_integer_partition(v2);
     auto s3 = dsl::conjugate_integer_partition(v3);
     
     
     dsl::print(s, "\n");
     dsl::print(s2, "\n");
     dsl::print(s3, "\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     (not working at the moment)
     @endcode
     */
    template<typename V>
    V conjugate_integer_partition(V v, bool is_sorted=false) {
        
        auto start = std::begin(v);
        auto stop = std::end(v);
        
        if(start != stop) {
            
            auto last = (--stop)++;  // point to last element
            
            // sort elements in v from smallest to largest if not already sorted
            if(!is_sorted)
                std::sort( start, stop );
            
            // Initialize conjugate partition size with largest part
            //size_t n = v.size();
            V conj( *last );
            
            // Calculate smaller parts using conj[j] = #{i : v[i] >= j}
            
            //size_t j=0;
            size_t diff = 0;
            
            auto j = std::begin(conj);
            
            size_t n = v.size();
            *j++ = n;
            //conj[j++]=*last;
            
            for(size_t i=2; i<=n;i++) {
                while(*start < i) {
                    diff++;
                    start++;
                }
                *j++ = n-diff;
                //conj[j++] = n-diff;
                //cout<<conj<<endl;
            }
            return conj;
        }
        
        return v;
    }
    
    /** Finds the conjugate partition for a given input, entries can be in any order.  The container needs RANDOM ACCESS iterators in order to guarantee that the entries are in sorted order.  Otherwise bidirectional is sufficient if already sorted.
     
     @tparam F is the value type
     @tparam V is the container type
     @param v is an integer vector of values
     @param is_sorted is a flag which when true skips the sorting of the elements
     @returns the conjugate partition in descending order
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,1,1,2,2,3,4,5,5,5,6,6,8,9,10,11};
     std::vector<int> v2 {1,1,2,4,7};
     std::vector<int> v3 {1,10,5};
     
     auto s = dsl::conjugate_integer_partition(v);
     auto s2 = dsl::conjugate_integer_partition(v2);
     auto s3 = dsl::conjugate_integer_partition(v3);
     
     
     dsl::print(s, "\n");
     dsl::print(s2, "\n");
     dsl::print(s3, "\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     (not working at the moment)
     @endcode
     */
    template<typename V>
    V conjugate(V v, bool is_sorted=false) {
        
        auto start = std::begin(v);
        auto stop = std::end(v);
        
        // sort elements in v from smallest to largest if not already sorted
        if(!is_sorted)
            std::sort( start, stop, std::greater<typename V::value_type>() );
        
        if(std::accumulate(start, stop, 0) == 0)
            return v;
        
        
        size_t current_value = 1;
        size_t conjugate_value = 0;
        size_t n = v.size();
        
        if(start != stop) {
            
            // Initialize conjugate partition size with largest part
            //size_t n = v.size();
            V conj( *start );
            
            auto conj_start = std::begin(conj);
            auto conj_stop = std::end(conj);
            
            while(conj_start != conj_stop) {
                *conj_start = std::count_if(start, stop, [&](typename V::value_type a) { return a >= current_value; });
                ++current_value;
                ++conj_start;
            }
            
            
            return conj;
        }
        
        return v;
    }
    
    
    /** Tests whether lhs and rhs satisfy a partial order dominance called majorization
        @param lhs is the smaller
        @param rhs is the larger
        @tparam Vector is the collection type
        @returns true if lhs is majorized by rhs
     
     Example 1:
     @code
     std::vector<int> v1 {0,0,0,1,2};
     std::vector<int> v2 {0,0,0,3};
     
     
     std::cout << dsl::majorizes(v1,v2) << std::endl;
     std::cout << dsl::majorizes(v2,v1) << std::endl;
     
     @endcode
     */
    
    template<typename Vector>
    bool majorizes(Vector lhs, Vector rhs) {
     
        auto lstart = std::begin(lhs);
        auto lstop = std::end(lhs);

        auto rstart = std::begin(rhs);
        auto rstop = std::end(rhs);

        std::sort(lstart, lstop, std::greater<typename Vector::value_type>());
        std::sort(rstart, rstop, std::greater<typename Vector::value_type>());
        
        auto ltotal = *lstart++;
        auto rtotal = *rstart++;
        
        while(ltotal <= rtotal && lstart != lstop && rstart != rstop) {
            ltotal += *lstart++;
            rtotal += *rstart++;
        }
        
        if(ltotal > rtotal) return false;
        
        if(lstart == lstop) {
        
            while(rstart != rstop) {
                rtotal += *(rstart++);
            }
            
        }
        else if(rstart == rstop) {
            
            while(lstart != lstop) {
                ltotal += *(lstart++);
            }
        }
        
        if(ltotal != rtotal) return false;
        
        return true;
        
    }
    
    
    /** sorts elements between a given value, e.g., sorts elements between all instances of 2.
     @param start is the initial location
     @param stop is one after last location
     @param val is the segmenting value
     */
    template<typename T, typename RandomAccess>
    void sort_between(RandomAccess start, RandomAccess stop, T val) {
        
        while(start <= stop) {
            
            // find next instance of val
            auto local = std::find(start, stop, val);
            std::sort(start, local);
            
            // update start
            start = local+1;
        }
    }
    
    
    /** Intention is to deprecate since std::lower_bound is the intended functionality:
     Returns an iterator indexed by the forward iterators to the value input, or an iterator equivalent to stop.
     @tparam T is the type of the value to search for
     @tparam ForwardIterator is any forward iterator type
     @param start is the beginning of the collection
     @param stop is one after the last element
     @returns iterator to found element, else iterator equivalent to stop
     */
    template<typename T, typename ForwardIterator>
    ForwardIterator binary_search_iterator(ForwardIterator start, ForwardIterator stop, T&& t) {
        return std::lower_bound(start, stop , t);
    }
    
    /** Returns an iterator using the first of a pair of elements indexed by the random access iterators.
        @tparam T is the type of the value to search for
        @tparam ForwardIterator is any forward iterator type
        @param start is the beginning of the collection
        @param stop is one after the last element
        @returns iterator to found element, else iterator equivalent to stop
    */
    template<typename T, typename ForwardIterator>
    ForwardIterator binary_search_iterator_first(ForwardIterator start, ForwardIterator stop, T&& t) {
        return std::lower_bound(start, stop, t, [](decltype(*start)& pair, T val) { return pair.first < val;});
    }
    
    /** Generic algorithm for apply, applies function to n elements
        @tparam _InputIterator is the input iterator type
        @tparam Size is any unsigned integer type large enough to index values from 0,...,__n-1
        @tparam _OutputIterator is the output iterator type for result
        @tparam _UnaryOperation is the function that is applied to each element
        @param __first is an iterator to the first of a collection of at least n elements
        @param __n is the number of elements for which to apply __op
        @param __result is an iterator to the first of a collection of at least n elements
        @param __op is the function object with unary function operator
     
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,2,3,4,5,6,7,8,9,10};
     
     // Square all numbers
     dsl::print(v,"\n");
     dsl::transform_n(std::begin(v), 10, std::begin(v), [](int& a) { return a*a; } );
     
     // double the first five
     dsl::print(v,"\n");
     dsl::transform_n(std::begin(v), 5, std::begin(v), [](int& a) { return a+a; } );
     dsl::print(v,"\n");
     
     // Square all numbers, store the result in s
     std::vector<int> s(10);
     dsl::transform_n(std::begin(v), 10, std::begin(s), [](int& a) { return a*a; } );
     dsl::print(s,"\n");
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9,10}
     {1,4,9,16,25,36,49,64,81,100}
     {2,8,18,32,50,36,49,64,81,100}
     {4,64,324,1024,2500,1296,2401,4096,6561,10000}
     @endcode
    */
    template<typename _InputIterator, typename Size, typename _OutputIterator, typename _UnaryOperation>
    void transform_n(_InputIterator __first, Size __n, _OutputIterator __result, _UnaryOperation __op) {
        for(Size i=0;i<__n;++i)
            *__result++ = __op(*__first++);
    }

    /** Generic algorithm for apply, applies function to n elements
     @tparam _InputIterator1 is the input iterator type of the first collection
     @tparam _InputIterator2 is the input iterator type of the second collection
     @tparam Size is any unsigned integer type large enough to index values from 0,...,__n-1
     @tparam _OutputIterator is the output iterator type for result
     @tparam _BinaryOperation is the function that is applied to each element
     @param __first1 is an iterator to the first of a collection of at least n elements
     @param __n is the number of elements for which to apply __op
     @param __first2 is an iterator to the first of a collection of at least n elements
     @param __result is an iterator to the first of a collection of at least n elements
     @param __binary_op is the function object with binary function operator
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v {1,2,3,4,5,6,7,8,9,10};
     
     // Square all numbers
     dsl::print(v,"\n");
     dsl::transform_n(std::begin(v), 10, std::begin(v), [](int& a) { return a*a; } );
     
     // double the first five
     dsl::print(v,"\n");
     dsl::transform_n(std::begin(v), 5, std::begin(v), [](int& a) { return a+a; } );
     dsl::print(v,"\n");
     
     // Square all numbers, store the result in s
     std::vector<int> s(10);
     dsl::transform_n(std::begin(v), 10, std::begin(s), [](int& a) { return a*a; } );
     dsl::print(s,"\n");
     
     // Go a little crazy
     std::vector<int> s2(10);
     dsl::transform_n(std::begin(v), 10, std::begin(s), std::begin(s2), [](int a, int b) { return a*b+b*b-4*a; } );
     dsl::print(s2,"\n");
     
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9,10}
     {1,4,9,16,25,36,49,64,81,100}
     {2,8,18,32,50,36,49,64,81,100}
     {4,64,324,1024,2500,1296,2401,4096,6561,10000}
     {16,4576,110736,1081216,6374800,1726128,5882254,17039104,43577838,100999600}
     @endcode
     */
    template<typename _InputIterator1, typename Size, typename _InputIterator2, typename _OutputIterator, typename _BinaryOperation>
    void transform_n(_InputIterator1 __first1, Size __n, _InputIterator2 __first2, _OutputIterator __result, _BinaryOperation __binary_op) {
        for(Size i=0;i<__n;++i)
            *__result++ = __binary_op(*__first1++, *__first2++);
    }

    /*
    template<typename T, typename F, typename = std::enable_if<std::is_pointer<F>::type> >
    F unique_copy_nonconsecutive(const T* start, const T* stop, F output) {
        //,HashFunction hash_function = std::hash<typename InputIterator::value_type>()) {
        // Ambiguous to use HashFunction template when using BinaryPredicate overloaded version as well
        
        if(start == stop) return output;
        
        // Have to hard code in this type for now, can't figure out how to template without ambiguity.
        std::hash<T> hash_function;
        
        // Create a (hash) table of values
        std::vector<T> hash_table;
        
        // Tests for finding an element in the table
        bool flag = false;
        
        // Iterate through all elements
        while(start != stop) {
            
            flag = false;
            
            // Search for current value in hash_table
            for(auto& x : hash_table)
                
                // If hash is already found
                if( hash_function(*start) == x ) {
                    flag = true;
                    break;
                }
            
            if(!flag) {
                hash_table.push_back(hash_function(*start));
                *output++ = *start;
            }
            
            ++start;
            
        }
        
        return output;
    }
*/
    
    
    
    /** copies unique elements from one list to another, does NOT assume the list is sorted.  CANNOT be used on pointer types at all.
     @tparam InputIterator is any input iterator type
     @tparam OutputIterator is any output iterator type
     @param start refers to first element in range of values to copy
     @param stop refers to one after last element in range of values to copy
     @param output refers to first element in container that will receive copied elements
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Throw in some digits of pi
     std::vector<int> v {3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3};
     std::cout << v << std::endl;
     
     // unique out the collection in order
     auto it = dsl::unique_copy_nonconsecutive(std::begin(v), std::end(v), std::begin(v));
     
     // erase remaining elements so new size of vector is the list without any duplicates, but still in order
     v.erase(it,std::end(v));
     
     dsl::print(v,"\n");
     
     // unique out digits which are multiples of 2, i.e., all even numbers are deemed equivalent
     it = dsl::unique_copy_nonconsecutive(std::begin(v), std::end(v), std::begin(v), [](int a, int b)->bool { return dsl::gcd(a,b)%2==0;});
     
     // erase remaining elements so new size of vector is the list without any duplicates, but still in order
     v.erase(it,std::end(v));
     
     dsl::print(v,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3}
     {3,1,4,5,9,2,6,8,7}
     {3,1,4,5,9,7}
     @endcode
    */
    template<typename InputIterator, typename OutputIterator >
    //,typename HashFunction = std::hash<typename InputIterator::value_type> >
    OutputIterator unique_copy_nonconsecutive(InputIterator start, InputIterator stop, OutputIterator output) {
                                              //,HashFunction hash_function = std::hash<typename InputIterator::value_type>()) {
                                              // Ambiguous to use HashFunction template when using BinaryPredicate overloaded version as well
        
        
        if(start == stop) return output;

        // Have to hard code in this type for now, can't figure out how to template without ambiguity.
        std::hash<typename InputIterator::value_type> hash_function;

        // Create a (hash) table of values
        std::vector<typename InputIterator::value_type> hash_table;
        
        // Tests for finding an element in the table
        bool flag = false;
        
        // Iterate through all elements
        while(start != stop) {
            
            flag = false;
            
            // Search for current value in hash_table
            for(auto& x : hash_table)
                
                // If hash is already found
                if( hash_function(*start) == x ) {
                    flag = true;
                    break;
                }
            
            if(!flag) {
                hash_table.push_back(hash_function(*start));
                *output++ = *start;
            }
            
            ++start;
            
        }
        
        return output;
    }
    
    /** copies unique elements from one list to another, does NOT assume the list is sorted
     @tparam InputIterator is any input iterator type
     @tparam OutputIterator is any output iterator type
     @tparam BinaryPredicate is any class with a binary predicate function defined
     @param start refers to first element in range of values to copy
     @param stop refers to one after last element in range of values to copy
     @param output refers to first element in container that will receive copied elements
     @param bin_op refers to any function object with appropriately defined binary predicate
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/numerical.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Throw in some digits of pi
     std::vector<int> v {3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3};
     std::cout << v << std::endl;
     
     // unique out the collection in order
     auto it = dsl::unique_copy_nonconsecutive(std::begin(v), std::end(v), std::begin(v));
     
     // erase remaining elements so new size of vector is the list without any duplicates, but still in order
     v.erase(it,std::end(v));
     
     dsl::print(v,"\n");
     
     // unique out digits which are multiples of 2, i.e., all even numbers are deemed equivalent
     it = dsl::unique_copy_nonconsecutive(std::begin(v), std::end(v), std::begin(v), [](int a, int b)->bool { return dsl::gcd(a,b)%2==0;});
     
     // erase remaining elements so new size of vector is the list without any duplicates, but still in order
     v.erase(it,std::end(v));
     
     dsl::print(v,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3}
     {3,1,4,5,9,2,6,8,7}
     {3,1,4,5,9,7}
     @endcode
    */
    template<typename InputIterator, typename OutputIterator, typename BinaryPredicate >
    //,typename HashFunction = std::hash<typename InputIterator::value_type> >
    OutputIterator unique_copy_nonconsecutive(InputIterator start, InputIterator stop, OutputIterator output, BinaryPredicate bin_op) {
                                              //,HashFunction hash_function = std::hash<typename InputIterator::value_type>()) {
                                            // Ambiguous to let hash_function be chosen with binary predicate function.
        
        if(start == stop) return static_cast<typename OutputIterator::value_type>(output);

        // Have to hard code in this type for now, can't figure out how to template without ambiguity.
        std::hash<typename InputIterator::value_type> hash_function;

        auto start_of_output = output;
        
        // Create a (hash) table of values
        std::vector<typename InputIterator::value_type> hash_table;
        
        // Tests for finding an element in the table
        bool flag = false;
        
        // Iterate through all elements
        while(start != stop) {
            
            flag = false;
            
            // Search for current value in hash_table
            for(auto& x : hash_table)
                
                // If hash is already found
                if( bin_op(hash_function(*start),x)) {
                    flag = true;
                    break;
                }
            
            // If hash is not already found, search for equivalent elements next since this is slower
            if(!flag) {
                auto it = start_of_output;
                
                // Search through current list of output
                while(it != output) {
                    if( bin_op(*it, *start) ) {  // If equivalent element found, do not add
                        flag = true;
                        break;
                    }
                    ++it;
                }
            }
            
            if(!flag) {
                hash_table.push_back(hash_function(*start));
                *output++ = *start;
            }
            
            ++start;
            
        }
        
        return output;
    }

    
    
    
    /** In place transposition of a row-major matrix of size m*n, assumes contiguous array.
     Code from http://stackoverflow.com/questions/9227747/in-place-transposition-of-a-matrix \n
        Written by Christian Ammer. \n
       I changed int m third parameter to size_t m
     @tparam RandomAccessIterator is any random access iterator
     @param first is the iterator to first element
     @param last is the iterator to one after last element
     @param m is the number of columns
     @code
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Form matrix [[1,2,3],[4,5,6],[7,8,9]] using a row-major 1D vector
     std::vector<int> v {1,2,3,4,5,6,7,8,9};
     
     dsl::print(v,"\n");
     
     auto two_by_two = dsl::table_indices(2,2);
     auto start = std::begin(two_by_two);
     
     // To access an element at index (i,j) in the array, use j+i*m, where m is the number of rows
     std::cout << "Two by two submatrix of elements (1:2,1:2) \n";
     std::cout << "[[" << v[start->second + start->first*3] << ",";          ++start;
     std::cout         << v[start->second + start->first*3] << "],\n[";      ++start;
     std::cout         << v[start->second + start->first*3] << ",";          ++start;
     std::cout         << v[start->second + start->first*3] << "]]";         ++start;
     
     std::cout << "\n\n";
     
     std::cout << "The transpose looks like: ";
     dsl::transpose(std::begin(v), std::end(v), 3); // 3 is the number of columns
     dsl::print(v,"\n");
     
     start = std::begin(two_by_two);
     // To access an element at index (i,j) in the array, use j+i*m, where m is the number of rows
     std::cout << "Two by two submatrix of elements (1:2,1:2) \n";
     std::cout << "[[" << v[start->second + start->first*3] << ",";          ++start;
     std::cout         << v[start->second + start->first*3] << "],\n[";      ++start;
     std::cout         << v[start->second + start->first*3] << ",";          ++start;
     std::cout         << v[start->second + start->first*3] << "]]";         ++start;
     
     std::cout << "\n\n";
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9}
     Two by two submatrix of elements (1:2,1:2)
     [[1,2],
     [4,5]]
     
     The transpose looks like: {1,4,7,2,5,8,3,6,9}
     Two by two submatrix of elements (1:2,1:2)
     [[1,4],
     [2,5]]
     @endcode
     */
    template<class RandomAccessIterator>
    void transpose(RandomAccessIterator first, RandomAccessIterator last, size_t m)
    {
        // magic ...
        const unsigned int mn1 = (last - first - 1);
        const unsigned int n   = (last - first) / m;
        std::vector<bool> visited(last - first);
        RandomAccessIterator cycle = first;
        while (++cycle != last) {
            if (visited[cycle - first])
                continue;
            unsigned int a = cycle - first;
            do  {
                a = a == mn1 ? mn1 : (n * a) % mn1;
                std::swap(*(first + a), *cycle);
                visited[a] = true;
            } while ((first + a) != cycle);
        }
    }
    
    /** @class NotDivisibleBy
     @brief Creates function objects which check for divisibility
     
     @code
     
     
     #include "desalvo/numerical.h" // See documentation for list of keywords included in this file.
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Form matrix [[1,2,3],[4,5,6],[7,8,9]] using a row-major 1D vector
     std::vector<int> v {1,2,3,4,5,6,7,8,9,10};
     std::vector<int> v2(10);
     
     auto it = std::copy_if( std::begin(v), std::end(v), std::begin(v2), dsl::NotDivisibleBy(3) );
     
     // optional erase, makes output cleaner
     v2.erase(it, std::end(v2));
     
     
     dsl::print(v,"\n");
     dsl::print(v2,"\n");
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9,10}
     {1,2,4,5,7,8,10}
     @endcode
     */
    class NotDivisibleBy {
    public:
        /** Construct a function object with a given divisibility condition
         @param in is the divisibility value
         */
        NotDivisibleBy(unsigned long in) : n(in) {};
        
        /** Checks if input is divisible by n
         @param x is the input to check for divisibility
         */
        bool operator()(unsigned long x) { return (0 != x % n); };
    private:
        unsigned long n;
    };

    
    /** @class DivisibleBy
     @brief Creates function objects which check for divisibility
     
     @code
     
     #include "desalvo/numerical.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Form matrix [[1,2,3],[4,5,6],[7,8,9]] using a row-major 1D vector
     std::vector<int> v {1,2,3,4,5,6,7,8,9,10};
     std::vector<int> v2(10);
     
     auto it = std::copy_if( std::begin(v), std::end(v), std::begin(v2), dsl::DivisibleBy(3) );
     
     // optional erase, makes output cleaner
     v2.erase(it, std::end(v2));
     
     
     dsl::print(v,"\n");
     dsl::print(v2,"\n");
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5,6,7,8,9,10}
     {3,6,9}
     @endcode
     */
    class DivisibleBy {
    public:
        /** Construct a function object with a given divisibility condition
         @param in is the divisibility value
         */
        DivisibleBy(unsigned long in) : n(in) {};
        
        /** Checks if input is divisible by n
         @param x is the input to check for divisibility
         */
        bool operator()(unsigned long x) { return (0 == x % n); };
    private:
        unsigned long n;
    };

    
    /** @class ArithmeticProgression
        @brief Sequence generator for an arithmetic progression {a, a+r, a+2r, ...}
     
     @tparam T is the underlying data type
     @code
     
     #include "desalvo/numerical.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     dsl::ArithmeticProgression<int> p(3, 7); // 3+7k for k=0,1,2,...
     dsl::ArithmeticProgression<int> p2(1, 2); // 1+2k for k=0,1,2,...
     dsl::ArithmeticProgression<int> p3(0, 3); // 0+3k for k=0,1,2,...
     
     std::vector<int> v(10);
     std::vector<int> v2(10);
     std::vector<int> v3(10);
     
     std::generate(std::begin(v), std::end(v), p);
     std::generate(std::begin(v2), std::end(v2), p2);
     std::generate(std::begin(v3), std::end(v3), p3);
     
     dsl::print(v,"\n");
     dsl::print(v2,"\n");
     dsl::print(v3,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {3,10,17,24,31,38,45,52,59,66}
     {1,3,5,7,9,11,13,15,17,19}
     {0,3,6,9,12,15,18,21,24,27}
     @endcode
    */
    template<typename T = size_t>
    class ArithmeticProgression {
    public:
        /** Constructs an arithmetic progression with starting value and multiple value
            @param input_offset is the initial value of the sequence
            @param input_multiple is the multiples to add to each successive number
        */
        ArithmeticProgression(T input_offset, T input_multiple) : offset(input_offset), multiple(input_multiple), current(input_offset) { }
        
        /** get next value in the sequence 
            @returns next value in the sequence
        */
        T operator()() {
            auto clone(current);
            current += multiple;
            return clone;
        }
        
    private:
        T offset;
        T multiple;
        T current;
    };
    
    /** Creates a list of prime numbers up to n+1.
        @tparam V is the container to store the elements
        @param n is the upper bound on prime numbers
        @returns a list of prime numbers up to n+1.
     
     @code
     #include "desalvo/numerical.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::sieve(100);
     
     // Make a list of all prime numbers up to 101
     dsl::print(v);
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101}
     @endcode
    */
    template<typename V = std::vector<size_t> >
    V sieve(size_t n) {
        
        if( n<=1 ) // return empty list.
            return V();
        
        V v(n/2+1);
        
        
        auto first = std::begin(v);
        *first++ = 2; // initialize first prime to 2.
        
        std::generate(first, std::end(v), ArithmeticProgression<typename V::value_type>(3,2));
        
        // purely std functions to replace line above starting with std::generate( ... )
        //std::iota( first, std::end(v), 1); // Initializes values {2,1,2,3,...,[n/2]+1}
        //std::for_each(first, std::end(v), [](decltype(v[0]) &a) { a =a*2+1; }); // {2,3,5,7,9,...}
        
        auto start = first;
        auto next = (++start)--;
        auto stop = std::end(v);
        
        while(start != stop && *start <= sqrt(n)) {
            stop = std::stable_partition(next, stop, NotDivisibleBy(*start));
            ++start;
            ++next;
        }
        
        // Chop off trailing entries
        v.erase(stop,v.end());
        return v;
    }
    
    
    /** Return all MULTISET subsets of [n] = {1,2,...,n} of size k, i.e., {{1,1,...,1},{1,1,...,1,2},...,{1,1,...,1,3},...{n,...,n}}
        @param n is the set of values
        @param k is the subset size
        @returns all subsets of size k from [n]
     
     @code
     #include "desalvo/numerical.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto s0 = dsl::multiset_subsets(3,0);
     auto s1 = dsl::multiset_subsets(3,1);
     auto s2 = dsl::multiset_subsets(3,2);
     auto s3 = dsl::multiset_subsets(3,3);
     
     dsl::print(s0,"\n");
     dsl::print(s1,"\n");
     dsl::print(s2,"\n");
     dsl::print(s3,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {}
     {{1},{2},{3}}
     {{1,1},{1,2},{1,3},{2,1},{2,2},{2,3},{3,1},{3,2},{3,3}}
     {{1,1,1},{1,1,2},{1,1,3},{1,2,1},{1,2,2},{1,2,3},{1,3,1},{1,3,2},{1,3,3},{2,1,1},{2,1,2},{2,1,3},{2,2,1},{2,2,2},{2,2,3},{2,3,1},{2,3,2},{2,3,3},{3,1,1},{3,1,2},{3,1,3},{3,2,1},{3,2,2},{3,2,3},{3,3,1},{3,3,2},{3,3,3}}
     @endcode
    */
    std::vector< std::vector<short> > multiset_subsets(short n, short k) {
        
        std::vector< std::vector<short> > sets;
        
        // return emptyset
        if(k == 0) return sets;
        
        // We can now assume k >= 1
        std::vector<short> v(k);
        
        // Return {{1},...,{n}}
        if(k == 1) {
            sets.resize(n);
            for(size_t i=1;i<=n;++i) {
                sets[i-1].push_back(i);
            }
            return sets;
        }
        
        auto smaller_subsets = multiset_subsets(n,k-1);
        // We can assume that n >= 2 and k >= 2.
        for(size_t i=1;i<=n;i++) {
            //v.insert(i);
            v[0] = i;
            
            for(auto x : smaller_subsets) {
                //for(auto y : x){
                //    v.insert(y);
                
                //    sets.insert(v);
                //    v.erase(y);
                //}
                std::copy(std::begin(x), std::end(x), std::begin(v)+1);
                //sets.insert(v);
                sets.push_back(v);
            }
            
        }
        return sets;
        
    }
    
    /** Return all subsets of [n] = {1,2,...,n} of size k, i.e., {{1,2,...,k},{1,2,...,k-1,k+1},...,{1,2,...,k-1,n},...{n-k+1,...,n}}.
     Currently uses inefficient algorithm of generating all first using multiset_subsets and then deleting duplicates.
     @param n is the set of values
     @param k is the subset size
     @returns all subsets of size k from [n]
     
     @code
     #include "desalvo/numerical.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     auto s0 = dsl::unique_multiset_subsets(3,0);
     auto s1 = dsl::unique_multiset_subsets(3,1);
     auto s2 = dsl::unique_multiset_subsets(3,2);
     auto s3 = dsl::unique_multiset_subsets(3,3);
     
     dsl::print(s0,"\n");
     dsl::print(s1,"\n");
     dsl::print(s2,"\n");
     dsl::print(s3,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {}
     {{1},{2},{3}}
     {{1,1},{1,2},{1,3},{2,2},{2,3},{3,3}}
     {{1,1,1},{1,1,2},{1,1,3},{1,2,2},{1,2,3},{1,3,3},{2,2,2},{2,2,3},{2,3,3},{3,3,3}}
     @endcode
     */
    std::vector< std::vector<short> > unique_multiset_subsets(short n, short k) {
        
        auto x = multiset_subsets(n,k);
        
        for(auto& y : x)
            std::sort(std::begin(y), std::end(y));
        
        std::sort( std::begin(x), std::end(x) );
        auto new_end = std::unique(std::begin(x), std::end(x));
        
        x.resize( std::distance(std::begin(x),new_end) );
        
        return x;
    }
    

    // Assume possibilities is sorted!
    /** Take a vector of vectors, and ... I don't remember
     
    */
    size_t two_by_two_map(const std::vector<short>& v, const std::vector<std::vector<short>>& possibles) {
        
        //auto x = std::find( std::begin(possibles), std::end(possibles), v );
        auto x = binary_search_iterator( std::begin(possibles), std::end(possibles), v );
        
        return x - std::begin(possibles);
    }
    
    
    // Assume possibilities is sorted!
    size_t two_by_two_map(const std::vector<short>& v, const std::vector<std::pair<std::vector<short>, double>>& possibles) {
        
        //auto x = std::find( std::begin(possibles), std::end(possibles), v );
        auto x = binary_search_iterator_first( std::begin(possibles), std::end(possibles), v );
        
        return x - std::begin(possibles);
    }
    

    /** Partitions the first n numbers {1,2,...,n} into
     ( not divisible by 3 nor 5 | divisible by just 3 | divisible by just 5 | divisible by both 3 and 5 )
     @param n is the size of the list, for numbers {1,2,...,n}
     @code
     #include "desalvo/numerical.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     dsl::print(dsl::fizz_buzz_partition(25));
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,4,7,8,11,13,14,16,17,19,22,23,3,6,9,12,18,21,24,5,10,20,25,15}
     @endcode
    */
    std::vector<unsigned int> fizz_buzz_partition(size_t n) {
        
        // already fizz-buzz-ed for n <=3
        if(n <=3 ) return range<unsigned int>(3);
        
        std::vector<unsigned int> v(n);
        
        std::iota(std::begin(v), std::end(v), 1);
        
        // Partition by 5s
        auto it = std::stable_partition(std::begin(v), std::end(v), [](unsigned int& i) { return i%5!=0; });

        // Partition the 5s by 3s
        std::stable_partition(std::begin(v), it, [](unsigned int& i) { return i%3!=0; });
        
        // Partition the non-5s by 3s
        std::stable_partition(it, std::end(v), [](unsigned int& i) { return i%3!=0; });
        
        
        return v;
    }

    
    
    /** Writes a permutation as a product of cycles
        @tparam IntType is the underlying type of object
        @tparam Container is the container to hold the set of permutations
        @param permutation is a permutation
        @returns a collection of values, each element in the collection is a cycle
    */
    template<typename IntType, typename Container >
    Container permutation_as_product_of_cycles(const std::vector<IntType>& permutation) {
        
        Container cycles;
        std::vector< IntType > v;
        
        std::set<IntType> used_indices;
        // Initialize permutation size and start first cycle at index 0
        IntType n = permutation.size();
        IntType initial_index = 0;
        
        // loop until all cycles have been found
        while(initial_index < n) {
            v.clear();
            size_t index = initial_index;
            v.push_back(index);
            used_indices.insert(index);
            index = permutation[initial_index];
            
            // Keep applying pi(i), pi(pi(i)), ... until we get back to i.
            while(index != initial_index) {
                
                v.push_back( index);
                used_indices.insert(index);
                index = permutation[index];
            }
            
            std::reverse( ++begin(v), end(v));
            
            // Found full cycle, add it to list of cycles.
            cycles.push_back(v);
            
            //find next index i that is not in any current cycles.
            initial_index = 0;
            auto first = begin(used_indices);
            auto last = end(used_indices);
            while((first != last) && (*first == initial_index)) {
                ++first; ++initial_index;
            }
            //std::cout << v << std::endl;
            
        }
        return cycles;
    }
    
    
    /** Returns a list of transpositions of two elements, which when performed consecutively, starts with the identity permutation and gives the permutation input
     @tparam IntType is the underlying type of object
     @tparam Container is the container to hold the set of permutations
     @param permutation is a permutation
     @returns a collection of values, each element in the collection is a tranposition of two elements, i.e., tells which indices to swap
     */
    template<typename IntType, typename Container = std::vector<std::vector<IntType>> >
    Container permutation_as_product_of_transpositions(const std::vector<IntType>& permutation) {
        
        Container transpositions;
        auto cycles = permutation_as_product_of_cycles(permutation);
        
        // Cycle decomposition into transpositions
        for(auto x : cycles) {
            for(size_t i=1;i<x.size(); i++)
                transpositions.push_back( {x[0], x[i]} );
        }
        
        return transpositions;
        
    }
    
    
    

    
    
    
    /** @namespace matlab
        @brief functionality designed to mimic Matlab notation
     
     Whenever an algorithm is made which happens to have the exact same input/output structure as a Matlab routine, it is placed in this namespace in order to facilitate its use, familiarity with the large amount of user base of Matlab, and encourage further development.
     
    */
    namespace matlab {
        
        /** Calculate the sums of a collection of objects
         @param start is an iterator to starting value
         @param stop is an iterator to one after the last value
         @param initial is the initial value to add to the sum
         @returns x_1^a+x_2^a+...+x_n^a, the sums of powers of elements
         */
        template<typename V , typename ReturnType = long double>
        ReturnType sum(V&& v) {
            return std::accumulate(std::begin(v), std::end(v), static_cast<ReturnType>(0.));
        };
        
        /** Calculate the mean of a collection of objects
         @param start is an iterator to starting value
         @param stop is an iterator to one after the last value
         @param initial is the initial value to add to the sum
         @returns x_1^a+x_2^a+...+x_n^a, the sums of powers of elements
         */
        template<typename V , typename ReturnType = double>
        ReturnType mean(V&& v) {
            return std::accumulate(std::begin(v), std::end(v), static_cast<ReturnType>(0.)) / static_cast<ReturnType>(v.size());
        };
        
        /** Returns a new sorted collection, needs begin and end defined with random access iterator
         @tparam V is the container type
         @param v is the container to sort elements in place.
         @returns v in sorted order
         */
        template<typename V >
        V sort(V v) {
            // Sort the copy
            sort_in_place(v);
            return v;
        };
        
        /** Creates a new container with the partial sums, needs begin and end defined
         @tparam F is the value type
         @tparam V is the container type
         @param v is the container to sort elements in place.
         */
        template<typename F = double, typename V = std::vector<F> >
        V cumsum(V&& v) {
            std::partial_sum(begin(v), end(v), begin(v));
            return v;
        };

        /** Returns a new instance of the object in reversed order
         @tparam Conatiner is any container with a bidirectional iterator
         @param v is the container of elements
         @returns a copy of elements of v in reverse order
         */
        template<typename Container>
        Container reverse(const Container& r) {
            Container v(r);
            std::reverse ( std::begin(v), std::end(v) );
            return v;
        };
    };
    
    
    /** Reduces a permutation of the form {2, 40, 18, 12} -->  {1,4,3,2}
        @param vals is the vector of values
        @returns the vector of values in reduced form

     Example:
     @code
     
     @endcode
     */
    std::vector<size_t> permutation_reduction(std::vector<size_t> vals ) {
        
        // stores mapping to find elements again after sorting them
        std::unordered_map<size_t, size_t> rank_to_value;
        
        // sorts, making a separate copy
        auto sorted = matlab::sort(vals);
        
        // Insert mappings so can be unsorted
        for(size_t i=0, n=sorted.size(); i<n;++i)
            rank_to_value.insert({sorted[i], i+1});
        
        // update value with its rank.
        for(auto& x : vals)
            x = rank_to_value[x];
        
        return vals;
    }
    


}















#endif
