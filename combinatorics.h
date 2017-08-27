//
//  combinatorics.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 9/24/15.
//  Copyright (c) 2015 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_combinatorics_h
#define DeSalvo_Standard_Library_combinatorics_h

#include "sequence.h"
#include "numerical.h"
#include <algorithm>

namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    
    /** @class north_east_lattice_path
        @brief all walks from (0,0) to (n,k) using up and right moves
     
        I wanted to enumerate all of the paths.
     
     Example:
     @code
     // Code to check the n choose k different sets, make sure each are occurring equally likely
     #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
     
     int main(int argc, const char * argv[]) {
     
     // from (0,0) to (n,k)
     size_t n = 6;
     size_t k = 3;
     size_t m = 100000;
     
     std::multiset< std::vector<bool> > s;
     
     // Insert each generated set into s
     for(size_t i=0;i<m;++i)
     s.insert(dsl::set_n_choose_k(n,k));
     
     // Generate all possible paths from (0,0) to (n,k)
     dsl::north_east_lattice_path<bool> paths(n,k);
     
     // Print out each path along with the number of times it occurs in s
     for(auto& x : paths)
     std::cout << x << ": " << s.count(x) << std::endl;
     
     return 0;
     }
     @endcode
    */
    template<typename T=bool, typename V=std::vector<T>, typename SV=std::vector<V>>
    class north_east_lattice_path :
    public dsl::finite_sequence<dsl::store::random_access,north_east_lattice_path<T,V,SV>,V,SV> {
    
        typedef dsl::finite_sequence<dsl::store::random_access,north_east_lattice_path<T,V,SV>,V,SV> finite;
        friend typename dsl::finite_sequence<dsl::store::random_access,north_east_lattice_path<T,V,SV>,V,SV>::iterator;
        
    public:
        
        north_east_lattice_path(size_t input_n, size_t input_k) : n(input_n), k(input_k) {
            first_element = first_in_sequence();
            finite::store_sequence();
        }
        
        
         V first_in_sequence() const {
             auto first = dsl::binary_row<T,V>(n,k);
             std::reverse(std::begin(first), std::end(first));
             return first;
         }
        
        bool next_in_sequence(V& v) const {
            return std::next_permutation(std::begin(v), std::end(v));
        }
        
        
    private:
        size_t n;
        size_t k;
        
        V first_element;
        
    };
    
    
    
}




#endif
