//
//  subsets.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 10/19/15.
//  Copyright (c) 2015 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_subsets_h
#define DeSalvo_Standard_Library_subsets_h

#include "sequence.h"

namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    
    // see documentation.h
    template<
    dsl::store seq=dsl::store::bidirectional,
    typename T=size_t,
    typename V=std::vector<T>,
    typename SV=std::vector<V>
    >
    class subsets;
    
    
    template<typename T, typename V, typename SV>
    class subsets<dsl::store::random_access,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::random_access,subsets<dsl::store::random_access,T,V,SV>,V,SV>
        {
        
            typedef dsl::finite_sequence<dsl::store::random_access,subsets<dsl::store::random_access,T,V,SV>,V,SV> finite;


            template<typename V, typename SV>
            class subsets<dsl::store::random_access,subsets<dsl::store::random_access,T,V,SV>
            {
            public:
                subsets(const V& superset) : max_value(superset.size()) {
                    
                    
                
                };
        
        
        
        /** The sequence goes emptyset,1,2,3,4,...,N,12,13,14,...,1N,23,24,25,...,2N,...,(N-1)N,123,124,...,(N-2)(N-1)N,1234,1235,...,1...N,emptyset. */
        bool next_in_sequence() {
            
            // First, load in largest possible value
            size_t n = max_value;
            
            // Load in largest value in the set
            auto largest = std::end(sets);
            auto start = std::begin(sets);
            
            // Check for emptyset
            if(largest == start) {
                sets.push_back(1);
                return true;
            }
            
            --largest;
            
            // Check to see if set is of the form ...(N-k)(N-k+1)...(N-1)N.
            while(largest != start && n == *largest ) {
                --largest;
                --n;
            }
            
            // If we made it to the first element, check if we have k(k+1)...(N-1)N
            if(largest == start) {
                
                // If we have largest set of a given size, restart with one size larger 12...k(k+1)
                if(n == *largest) {
                    if(n>1) {
                        sets = dnl::range<size_t,V>(sets.size()+1);
                        return true;
                    }
                    else {
                        sets.clear();
                        return false;
                    }
                }
            }
            // we have j(k+1)...(N-1)N, j<k.  Increment j to j+1, reset elements afterwards to (j+2)...(j+k-1)
            ++(*largest);  // Increment j to j+1
            size_t j = *largest;
            
            // Reset elements larger than j+1
            ++largest;
            ++j;
            while(largest != std::end(sets)) {
                *largest = j;
                ++largest;
                ++j;
            }
            
            
            return true;
        }
        
        
        V& get() { return sets; };
        
        V operator()() { return sets; };
        
        operator V() { return sets; };
        
        size_t& operator[](size_t i) {
            return sets[i];
        }
        
        void print(std::ostream& out = std::cout) {
            out << sets;
        }
        
        friend std::ostream& operator<<(std::ostream& out, SetOfSubsets& s) {
            return (out << s.sets);
        }
        
    private:
        V sets;
        size_t max_value;
    };
    
    
    template<typename V, typename SV>
    class SetOfSubsets<SequenceType::StoreAll, V, SV>
    {
    public:
        SetOfSubsets(size_t n) : max_value(n) {
            
            SetOfSubsets<SequenceType::StoreOne, V,SV> sets(n);
            
            do {
                all_sets.push_back( sets() );
            }while(sets.next_in_sequence());
        };
        
        V& operator[](size_t i) {
            return all_sets[i];
        }
        
        friend std::ostream& operator<<(std::ostream& out, SetOfSubsets& s) {
            return out << s.all_sets;
        }
        
        
    private:
        SV all_sets;
        size_t max_value;
    };
    
    
    
}
    
#endif
