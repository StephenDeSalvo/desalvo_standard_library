/** @file dsl_algorithm.h
    @brief Apply algorithms from the Standard Library to the entire container rather than using iterators
 
    These functions are designed to mimic the Standard Library algorithm file, only instead of using iterators, we apply the transformations to the entire collection of values in the container.  Thus, it is intended to have a more Matlab-style, functional feel, with slightly simpler syntax, so that we do not have to keep writing std::begin(v), std::end(v) all the time.
*/

#ifndef DeSalvo_Standard_Library_dsl_algorithm_h
#define DeSalvo_Standard_Library_dsl_algorithm_h

#include <numeric>
#include <functional>
#include <algorithm>

namespace desalvo_standard_library {
    
    /** iota
     @tparam V is a container with a forward iterator, and property value_type
     @param v is a container of values
     @param val is the initial value, which is statically cast to T
     @code
     #include "desalvo/dsl_algorithm.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     std::vector<char> small_letters() {
     std::vector<char> v(26);
     dsl::iota( v, 'a');
     return v;
     }
     
     std::vector<char> capital_letters() {
     std::vector<char> v(26);
     dsl::iota( v, 'A');
     return v;
     }
     
     int main(int argc, const char * argv[]) {
     
     std::vector<int> v(10);
     std::vector<char> v2(10);
     
     dsl::iota(v,0);
     dsl::print(v,"\n");
     
     dsl::iota(v,1);
     dsl::print(v,"\n");
     
     // the second input is cast to V::value_type, so class of v must have this property defined
     dsl::iota(v,-4.5);
     dsl::print(v,"\n");
     
     dsl::iota(v2, 'a');
     dsl::print(v2, "\n");
     
     dsl::print(small_letters(),"\n");
     dsl::print(capital_letters(),"\n");
     
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {0,1,2,3,4,5,6,7,8,9}
     {1,2,3,4,5,6,7,8,9,10}
     {-4,-3,-2,-1,0,1,2,3,4,5}
     {a,b,c,d,e,f,g,h,i,j}
     {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z}
     {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z}
     @endcode
     */
    template<typename V>
    void iota(V& v, typename V::value_type val=static_cast<typename V::value_type>(1)) {
        std::iota(std::begin(v), std::end(v), val);
    }

    /** next_permutation
        @tparam V is a container with a forward iterator, and property value_type
        @param v is a container of values
        @returns whether values restarted
     @code
     #include "desalvo/dsl_algorithm.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     std::vector<char> small_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'a');
     return v;
     }
     
     std::vector<char> capital_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'A');
     return v;
     }
     
     int main(int argc, const char * argv[]) {
     
     // generate first 5 letters a,b,c,d,e
     auto v = small_letters(5);
     dsl::print(v,"\n");
     
     // Print all permutations of a,b,c,d,e
     while( dsl::next_permutation(v))
     dsl::print(v,"\n");
     
     // generate first 5 CAPITAL letters A,B,C,D,E
     auto v2 = capital_letters(5);
     
     dsl::next_permutation(v2,std::greater<char>());
     dsl::print(v2,"\n");
     
     // Print all permutations of a,b,c,d,e
     while( dsl::next_permutation(v2,std::greater<char>()))
     dsl::print(v2,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {a,b,c,d,e}
     {a,b,c,e,d}
     {a,b,d,c,e}
     {a,b,d,e,c}
     (... a bunch of other permutations, don't take this line literally! ...)
     {e,d,b,a,c}
     {e,d,b,c,a}
     {e,d,c,a,b}
     {e,d,c,b,a}
     {E,D,C,B,A}
     {E,D,C,A,B}
     {E,D,B,C,A}
     {E,D,B,A,C}
     (... a bunch of other permutations, don't take this line literally! ...)
     {A,B,D,E,C}
     {A,B,D,C,E}
     {A,B,C,E,D}
     {A,B,C,D,E}

     @endcode
    */
    template<typename V>
    bool next_permutation(V& v) {
        return std::next_permutation(std::begin(v), std::end(v));
    }
    
    /** next_permutation
     @tparam V is a container with a forward iterator, and property value_type
     @tparam Compare is any type which provides a Binary Predicate testing for less than inequality
     @param v is a container of values
     @param cmp is the comparison function object
     @returns whether values restarted
     @code
     #include "desalvo/dsl_algorithm.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     std::vector<char> small_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'a');
     return v;
     }
     
     std::vector<char> capital_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'A');
     return v;
     }
     
     int main(int argc, const char * argv[]) {
     
     // generate first 5 letters a,b,c,d,e
     auto v = small_letters(5);
     dsl::print(v,"\n");
     
     // Print all permutations of a,b,c,d,e
     while( dsl::next_permutation(v))
     dsl::print(v,"\n");
     
     // generate first 5 CAPITAL letters A,B,C,D,E
     auto v2 = capital_letters(5);
     
     dsl::next_permutation(v2,std::greater<char>());
     dsl::print(v2,"\n");
     
     // Print all permutations of a,b,c,d,e
     while( dsl::next_permutation(v2,std::greater<char>()))
     dsl::print(v2,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {a,b,c,d,e}
     {a,b,c,e,d}
     {a,b,d,c,e}
     {a,b,d,e,c}
     (... a bunch of other permutations, don't take this line literally! ...)
     {e,d,b,a,c}
     {e,d,b,c,a}
     {e,d,c,a,b}
     {e,d,c,b,a}
     {E,D,C,B,A}
     {E,D,C,A,B}
     {E,D,B,C,A}
     {E,D,B,A,C}
     (... a bunch of other permutations, don't take this line literally! ...)
     {A,B,D,E,C}
     {A,B,D,C,E}
     {A,B,C,E,D}
     {A,B,C,D,E}
     
     @endcode
     */
    template<typename V, typename Compare>
    bool next_permutation(V& v, Compare&& cmp) {
        return std::next_permutation(std::begin(v), std::end(v), cmp);
    }
    
    /** prev_permutation
     @tparam V is a container with a forward iterator, and property value_type
     @param v is a container of values
     @returns whether values restarted
     @code
     #include "desalvo/dsl_algorithm.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     std::vector<char> small_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'a');
     return v;
     }
     
     std::vector<char> capital_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'A');
     return v;
     }
     
     int main(int argc, const char * argv[]) {
     
     // generate first 5 letters a,b,c,d,e
     auto v = small_letters(5);
     dsl::prev_permutation(v);
     
     // Print all permutations of a,b,c,d,e
     while( dsl::prev_permutation(v))
     dsl::print(v,"\n");
     
     // generate first 5 CAPITAL letters A,B,C,D,E
     auto v2 = capital_letters(5);
     dsl::print(v2,"\n");
     
     // Print all permutations of a,b,c,d,e
     while( dsl::prev_permutation(v2,std::greater<char>()))
     dsl::print(v2,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {e,d,c,a,b}
     {e,d,b,c,a}
     {e,d,b,a,c}
     {e,d,a,c,b}
     (... a bunch of other permutations, don't take this line literally! ...)
     {a,b,d,e,c}
     {a,b,d,c,e}
     {a,b,c,e,d}
     {a,b,c,d,e}
     {A,B,C,D,E}
     {A,B,C,E,D}
     {A,B,D,C,E}
     {A,B,D,E,C}
     (... a bunch of other permutations, don't take this line literally! ...)
     {E,D,B,A,C}
     {E,D,B,C,A}
     {E,D,C,A,B}
     {E,D,C,B,A}
     @endcode
     */
    template<typename V>
    bool prev_permutation(V& v) {
        return std::prev_permutation(std::begin(v), std::end(v));
    }
    
    /** prev_permutation
     @tparam V is a container with a forward iterator, and property value_type
     @tparam Compare is any type which provides a Binary Predicate testing for less than inequality
     @param v is a container of values
     @param cmp is the comparison function object
     @returns whether values restarted
     @code
     #include "desalvo/dsl_algorithm.h"
     #include "desalvo/std_cout.h"
     
     namespace dsl = desalvo_standard_library;
     
     std::vector<char> small_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'a');
     return v;
     }
     
     std::vector<char> capital_letters(size_t n=26) {
     std::vector<char> v(n);
     dsl::iota( v, 'A');
     return v;
     }
     
     int main(int argc, const char * argv[]) {
     
     // generate first 5 letters a,b,c,d,e
     auto v = small_letters(5);
     dsl::prev_permutation(v);
     
     // Print all permutations of a,b,c,d,e
     while( dsl::prev_permutation(v))
     dsl::print(v,"\n");
     
     // generate first 5 CAPITAL letters A,B,C,D,E
     auto v2 = capital_letters(5);
     dsl::print(v2,"\n");
     
     // Print all permutations of a,b,c,d,e
     while( dsl::prev_permutation(v2,std::greater<char>()))
     dsl::print(v2,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {e,d,c,a,b}
     {e,d,b,c,a}
     {e,d,b,a,c}
     {e,d,a,c,b}
     (... a bunch of other permutations, don't take this line literally! ...)
     {a,b,d,e,c}
     {a,b,d,c,e}
     {a,b,c,e,d}
     {a,b,c,d,e}
     {A,B,C,D,E}
     {A,B,C,E,D}
     {A,B,D,C,E}
     {A,B,D,E,C}
     (... a bunch of other permutations, don't take this line literally! ...)
     {E,D,B,A,C}
     {E,D,B,C,A}
     {E,D,C,A,B}
     {E,D,C,B,A}
     @endcode
     */
    template<typename V, typename Compare>
    bool prev_permutation(V& v, Compare cmp) {
        return std::prev_permutation(std::begin(v), std::end(v), cmp);
    }
    
    
}






#endif
