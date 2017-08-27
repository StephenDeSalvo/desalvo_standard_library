//
//  DeSalvoPermutationLibrary.h
//  DeSalvoLibraryTestKit
//
//  Created by Stephen DeSalvo on 11/4/14.
//  Copyright (c) 2014 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvoLibraryTestKit_DeSalvoPermutationLibrary_h
#define DeSalvoLibraryTestKit_DeSalvoPermutationLibrary_h

#include <set>
#include <utility>
#include <iterator>
#include <initializer_list>
#include <algorithm>
#include <numeric>

#include "statistics.h"
#include "dsl_algorithm.h"
#include "sequence.h"
#include "std_cout.h"


namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    // *************  Begin the Restricted Permutation Classes *****************
    
    /** @enum restrictions
        @brief either Unrestricted, or Restrictions listed by either pairs of (i, sigma(i)), or by a binary matrix.
     
     The choices at present are: {none, by_pairs, by_matrix}
     */
    enum class restrictions {none, fixed_point_free, by_pairs, by_function};
    
    
    // see documentation.h
    template<
    dsl::store seq=dsl::store::bidirectional,
    restrictions type=restrictions::none,
    typename T=size_t,
    typename V=std::vector<T>,
    typename SV=std::vector<V>
    >
    class permutation;
    
    
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::random_access,restrictions::fixed_point_free,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::random_access,permutation<dsl::store::random_access,restrictions::fixed_point_free,T,V,SV>,V,SV>
    //,dsl::random_variable<V,permutation<dsl::store::random_access,restrictions::fixed_point_free,T,V,SV> >
    {
    
        typedef dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::fixed_point_free,T,V,SV>, V> finite;


    public:
        
        /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
         @param input_n is the initial size of the permutation.
         */
        permutation(size_t input_n) : n(input_n) {
            first_element = first_in_sequence();
            finite::store_sequence();
            
        };
        
        
        /** Compute the first instance of a permutation with given restrictions in lexicographic ordering
         @returns the first permutation in lexicographic ordering with the given restrictions
         */
        V first_in_sequence() const {
            // What is the first fixed-point-free permutation using next_permutation starting with {1,2,...,n}?
            // n even --> {2,1,4,3,6,5,...,n,n-1}     transpose every consecutive pairs of indices
            // n odd -->  {2,1,4,3,6,5,...,n-1,n,n-2} same as above, form a 3-cycle using last three elements.
            
            // Initialize to vector of all n, {n,n,...,n}.
            // In case n is odd, the loop excludes n, so it would be 0 if initialized to 0.
            V v(n,n);
            
            // queue up the first two values, 1 and 2.
            T first = static_cast<T>(1);
            T second = static_cast<T>(2);
            
            // start points to first element, start2 points to second element.
            auto start2 = std::begin(v);
            auto start = start2++;
            
            // pointer to stop condition.
            auto stop = std::end(v);
            
            
            // **** ALGORITHM ****
            // For n=1, return error.
            // For all n>=2, form all pairs of transpositions.
            // For odd n>=2, swap the last and third to last elements
            
            if(n == 1) std::cout << "ERROR: No fixed-point free permutation of 1." << std::endl;
            
            // Fill in {2,1,4,3,6,5, ...}
            while(start != stop && start2 != stop) {
                
                // Set first value to 2, 4, 6, 8, ...
                *start = second;
                // Set second value to 1, 3, 5
                *start2 = first;
                
                // Increment each pair by two
                ++(++start);
                ++(++second);
                
                ++(++start2);
                ++(++first);
                
            }
            
            if(n%2 == 1) { // if n is odd
                // current state is {2,1,4,3,6,5,...,n-1,n-2,n}
                // start2 points to one after the n
                // start points to the n
                
                // Need to swap the n and n-2.
                
                // Position start2 to point to n-2
                --(--start2);
                
                // Swap the n and n-2
                std::swap(*start, *start2);
                
            }
            return v;
        }
        
        
        /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
         @returns the last permutation in lexicographic ordering with the given restrictions
         */
        /*V last_in_sequence() const {
         V v(n);
         
         // If n is even, then last one is {n,n-1,n-2,...,2,1}
         // If n is odd, then last one is {n,n-1,n-2,...,
         std::iota(v.rbegin(), v.rend(), static_cast<T>(1));
         
         // Replace line above with this for C++14, extends class for use with pointer types as well.
         // std::iota(std::rbegin(v), std::rend(v), static_cast<T>(1));
         
         
         if(n%2 == 1) {
         auto first = std::begin(v)+=n/2;
         auto second = std::begin(v)+=n/2+1;
         std::swap(*first, *second);
         }
         
         return v;
         }
         */
        
        
        /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the next state
         @returns whether or not the sequence restarted
         */
        bool next_in_sequence(V& v) const {
            bool flag = true;
            bool test_for_end = true;
            
            
            do {
                flag = true;
                
                test_for_end = dsl::next_permutation(v);
                
                // test for fixed points
                for(size_t i=0;i<n;++i)
                    flag &= (v[i] != i+1);
                
            // stop once a fixed point is found
            }while(!flag && test_for_end);
            
            // if no fixed points found, return element
            if(!test_for_end) {
                v = first_element;
                return false;
            }
            
            return (v != first_element);
        }
        
    private:
        size_t n; // size of the permutation
        
        V first_element;
    };
    
    
    
    
    // Special: bidirectional, fixed point free
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::bidirectional,restrictions::fixed_point_free,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::bidirectional,permutation<dsl::store::bidirectional,restrictions::fixed_point_free,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::fixed_point_free,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::fixed_point_free,T,V>, V>::iterator;
        
    public:
        
        /** Initializes permutation to have size n, computes the first and last elements in the sequence, and stores the entire sequence.
         @param input_n is the initial size of the permutation.
         */
        permutation(size_t input_n) : n(input_n) {
            first_element = first_in_sequence();
            last_element = last_in_sequence();
        };
        
        
        /** Compute the first instance of a permutation with given restrictions in lexicographic ordering
         @returns the first permutation in lexicographic ordering with the given restrictions
         */
        V first_in_sequence() const {
            // What is the first fixed-point-free permutation using next_permutation starting with {1,2,...,n}?
            // n even --> {2,1,4,3,6,5,...,n,n-1}     transpose every consecutive pairs of indices
            // n odd -->  {2,1,4,3,6,5,...,n-1,n,n-2} same as above, form a 3-cycle using last three elements.
            
            // Initialize to vector of all n, {n,n,...,n}.
            // In case n is odd, the loop excludes n, so it would be 0 if initialized to 0.
            V v(n,n);
            
            // queue up the first two values, 1 and 2.
            T first = static_cast<T>(1);
            T second = static_cast<T>(2);
            
            // start points to first element, start2 points to second element.
            auto start2 = std::begin(v);
            auto start = start2++;
            
            // pointer to stop condition.
            auto stop = std::end(v);
            
            
            // **** ALGORITHM ****
            // For n=1, return error.
            // For all n>=2, form all pairs of transpositions.
            // For odd n>=2, swap the last and third to last elements
            
            if(n == 1) std::cout << "ERROR: No fixed-point free permutation of 1." << std::endl;
            
            // Fill in {2,1,4,3,6,5, ...}
            while(start != stop && start2 != stop) {
                
                // Set first value to 2, 4, 6, 8, ...
                *start = second;
                // Set second value to 1, 3, 5
                *start2 = first;
                
                // Increment each pair by two
                ++(++start);
                ++(++second);
                
                ++(++start2);
                ++(++first);
                
            }
            
            if(n%2 == 1) { // if n is odd
                // current state is {2,1,4,3,6,5,...,n-1,n-2,n}
                // start2 points to one after the n
                // start points to the n
                
                // Need to swap the n and n-2.
                
                // Position start2 to point to n-2
                --(--start2);
                
                // Swap the n and n-2
                std::swap(*start, *start2);
                
            }
            return v;
        }
        
        /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
         @returns the last permutation in lexicographic ordering with the given restrictions
         */
        V last_in_sequence() const {
            V v(n);
            
            // If n is even, then last one is {n,n-1,n-2,...,2,1}
            // If n is odd, then last one is {n,n-1,n-2,...,
            std::iota(v.rbegin(), v.rend(), static_cast<T>(1));
            
            // Replace line above with this for C++14, extends class for use with pointer types as well.
            // std::iota(std::rbegin(v), std::rend(v), static_cast<T>(1));
            
            
            if(n%2 == 1) {
                auto first = std::begin(v)+=n/2;
                auto second = std::begin(v)+=n/2+1;
                std::swap(*first, *second);
            }
            
            return v;
        }
        
        /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the next state
         @returns whether or not the sequence restarted
         */
        bool next_in_sequence(V& v) const {
            bool flag = true;
            bool test_for_end = true;
            do {
                flag = true;
                
                test_for_end = dsl::next_permutation(v);
                
                //flag &= !test_for_end;
                
                for(size_t i=0;i<n;++i)
                    flag &= (v[i] != i+1);
                
            }while(!flag && test_for_end);
            
            if(!test_for_end) {
                v = first_element;
                return false;
            }
            
            
            return (v != first_element);
        }
        
        /** Given a current state, updates the input to the previous state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the previous state
         @returns whether or not the sequence restarted
         */
        bool previous_in_sequence(V& v) const {
            bool flag = true;
            bool test_for_end = true;
            do {
                flag = true;
                
                test_for_end = dsl::prev_permutation(v);
                
                //flag &= !test_for_end;
                
                for(size_t i=0;i<n;++i)
                    flag &= (v[i] != i+1);
                
            }while(!flag && test_for_end);
            
            if(!test_for_end) {
                v = last_element;
                return false;
            }
            
            
            return (v != last_element);
        }
        
    private:
        size_t n; // size of the permutation
        
        V first_element;
        V last_element;
        
        
    };
    
    
    
    // Specialization: forward, fixed point free
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::forward,restrictions::fixed_point_free,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::forward,permutation<dsl::store::forward,restrictions::fixed_point_free,T,V,SV>,V,SV> {
        
        typedef permutation<dsl::store::forward,restrictions::fixed_point_free,T,V,SV> perm;
        typedef dsl::finite_sequence<dsl::store::forward, perm, V> finite;
        
        friend typename finite::iterator;
        
    public:
        
        /** Initializes permutation to have size n, computes the first element in the sequence.
         @param input_n is the initial size of the permutation.
         */
        permutation(size_t input_n) : n(input_n) {
            first_element = first_in_sequence();
        };
        
        
        /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
         @returns the last permutation in lexicographic ordering with the given restrictions
         */
        V first_in_sequence() const {
            // What is the first fixed-point-free permutation using next_permutation starting with {1,2,...,n}?
            // n even --> {2,1,4,3,6,5,...,n,n-1}     transpose every consecutive pairs of indices
            // n odd -->  {2,1,4,3,6,5,...,n-1,n,n-2} same as above, form a 3-cycle using last three elements.
            
            // Initialize to vector of all n, {n,n,...,n}.
            // In case n is odd, the loop excludes n, so it would be 0 if initialized to 0.
            V v(n,n);
            
            // queue up the first two values, 1 and 2.
            T first = static_cast<T>(1);
            T second = static_cast<T>(2);
            
            // start points to first element, start2 points to second element.
            auto start2 = std::begin(v);
            auto start = start2++;
            
            // pointer to stop condition.
            auto stop = std::end(v);
            
            
            // **** ALGORITHM ****
            // For n=1, return error.
            // For all n>=2, form all pairs of transpositions.
            // For odd n>=2, swap the last and third to last elements
            
            if(n == 1) std::cout << "ERROR: No fixed-point free permutation of 1." << std::endl;
            
            // Fill in {2,1,4,3,6,5, ...}
            while(start != stop && start2 != stop) {
                
                // Set first value to 2, 4, 6, 8, ...
                *start = second;
                // Set second value to 1, 3, 5
                *start2 = first;
                
                // Increment each pair by two
                ++(++start);
                ++(++second);
                
                ++(++start2);
                ++(++first);
                
            }
            
            if(n%2 == 1) { // if n is odd
                // current state is {2,1,4,3,6,5,...,n-1,n-2,n}
                // start2 points to one after the n
                // start points to the n
                
                // Need to swap the n and n-2.
                
                // Position start2 to point to n-2
                --(--start2);
                
                // Swap the n and n-2
                std::swap(*start, *start2);
                
            }
            return v;
        }
        
        /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the next state
         @returns whether or not the sequence restarted
         */
        
        bool next_in_sequence(V& v) const {
            bool flag = true;
            bool test_for_end = true;
            do {
                flag = true;
                
                test_for_end = dsl::next_permutation(v);
                
                //flag &= !test_for_end;
                
                for(size_t i=0;i<n;++i)
                    flag &= (v[i] != i+1);
                
            }while(!flag && test_for_end);
            
            if(!test_for_end) {
                v = first_element;
                return false;
            }
            
            
            return (v != first_element);
        }
        
        
    private:
        size_t n; // size of the permutation
        
        V first_element;
        
        
    };

    
    
    // Specialized: random access, fixed point free
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::random_access,restrictions::none,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::random_access,permutation<dsl::store::random_access,restrictions::none,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::none,T,V,SV>, V> finite;
        
    public:
        
        /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
         @param input_n is the initial size of the permutation.
         */
        permutation(size_t input_n) : n(input_n) {
            first_element = first_in_sequence();
            finite::store_sequence();
            
        };
        
        
        /** creates {1,2,...,n}
         @returns the first permutation in lexicographic ordering with the given restrictions
         */
        V first_in_sequence() const {
            
            V v(n);
            
            std::iota(std::begin(v), std::end(v), static_cast<T>(1));
            
            return v;
        }
        
        
        /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the next state
         @returns whether or not the sequence restarted
         */
        bool next_in_sequence(V& v) const {
            return dsl::next_permutation(v);
        }
        
        
    private:
        size_t n; // size of the permutation
        
        V first_element;
    };
    
    
    
    
    // Special: bidirectional, no restrictions
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::bidirectional,restrictions::none,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::bidirectional,permutation<dsl::store::bidirectional,restrictions::none,T,V,SV>,V,SV>,
    public dsl::random_variable<V,permutation<dsl::store::bidirectional,restrictions::none,T,V> >
    {

        typedef dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::none,T,V>, V> finite;
    
        typedef dsl::random_variable<V, permutation<dsl::store::random_access, restrictions::fixed_point_free,T,V,SV> > random;

        friend typename finite::iterator;
        

        
    public:
        
        /** Initializes permutation to have size n, computes the first and last elements in the sequence, and stores the entire sequence.
         @param input_n is the initial size of the permutation.
         */
        permutation(size_t input_n) : n(input_n) {
            first_element = first_in_sequence();
            last_element = last_in_sequence();
        };
        
        
        /** Create {1,2,...,n-1,n}
         @returns the first permutation in lexicographic ordering {1,2,...,n-1,n}
         */
        V first_in_sequence() const {

            V v(n);
            std::iota(std::begin(v), std::end(v), static_cast<T>(1));
            
            return v;

        }
        
        /** Compute the last instance of a permutation in lexicographic ordering, {n,n-1,...,2,1}
         @returns the last permutation in lexicographic ordering with the given restrictions
         */
        V last_in_sequence() const {
            V v(n);
            
            // {n,n-1,n-2,...,2,1}
            std::iota(v.rbegin(), v.rend(), static_cast<T>(1));
            
            return v;
        }
        
        /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the next state
         @returns whether or not the sequence restarted
         */
        bool next_in_sequence(V& v) const {
            return dsl::next_permutation(v);
        }
        
        
        /** Given a current state, updates the input to the previous state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the previous state
         @returns whether or not the sequence restarted
         */
        bool previous_in_sequence(V& v) const {
            return dsl::prev_permutation(v);
        }
        
        template<typename URNG>
        V sample(URNG& gen = dsl::generator_64) {
            return dsl::random_permutation<T,V,URNG>(n,gen);
        }
        
        template<typename Function, typename URNG>
        V sample_using(Function distribution, URNG& gen = dsl::generator_64) {
            return distribution(gen);
        }
        
        
    private:
        size_t n; // size of the permutation
        
        V first_element;
        V last_element;
        
        
    };
    
    
    
    // Specialization: forward, fixed point free
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::forward,restrictions::none,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::forward,permutation<dsl::store::forward,restrictions::none,T,V,SV>,V,SV> {
        
        typedef permutation<dsl::store::forward,restrictions::none,T,V,SV> perm;
        typedef dsl::finite_sequence<dsl::store::forward, perm, V> finite;
        
        friend typename finite::iterator;
        
    public:
        
        /** Initializes permutation to have size n, computes the first element in the sequence.
         @param input_n is the initial size of the permutation.
         */
        permutation(size_t input_n) : n(input_n) {
            first_element = first_in_sequence();
        };
        
        
        /** Create {1,2,...,n-1,n}
         @returns the first permutation in lexicographic ordering {1,2,...,n-1,n}
         */
        V first_in_sequence() const {
            
            V v(n);
            std::iota(std::begin(v), std::end(v), static_cast<T>(1));
            
            return v;
            
        }
        
        /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
         @param v is the input state, which is updated to the next state
         @returns whether or not the sequence restarted
         */
        bool next_in_sequence(V& v) const {
            return dsl::next_permutation(v);
        }
        
    private:
        size_t n; // size of the permutation
        
        V first_element;
    };

    
    
    // Special: random access, restrictions by pairs
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::random_access,permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::by_pairs,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::by_pairs,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, const std::set< std::pair<size_t, size_t> >& initialize_restrictions );
        
        // Constructor using an initializer list
        permutation(size_t input_n, std::initializer_list<std::pair<size_t,size_t> > initial_list);
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        bool next_in_sequence(V& v) const;
        
        void insert(const std::pair<size_t, size_t>& res);
        void insert(std::pair<size_t, size_t>&& res);
        void insert(std::initializer_list<std::pair<size_t,size_t> > res);
        
        template<typename InputIterator>
        void insert(InputIterator start, InputIterator stop);
        
        
        void clear() { restrictions.clear(); };
        
        operator bool();
        
        //friend std::ostream& operator<<(std::ostream& out, const permutation& p) {
        //    return out << (p.permutation);
        //};
        
        
    private:
        size_t n;
        V first_element;
        
        std::set< std::pair<size_t, size_t > > restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n) : n(input_n) {
        first_element = first_in_sequence();
        finite::store_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n, const std::set< std::pair<size_t, size_t> >& initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions) {
        
        first_element = first_in_sequence();
        finite::store_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions with an initializer list of the form {{sigma(a), a},{sigma(b),b},...} meaning a cannot be in location sigma(a), b cannot be in location sigma(b), etc., and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n, std::initializer_list<std::pair<size_t,size_t> > initial_list) : n(input_n), restrictions(initial_list) {
        
        first_element = first_in_sequence();
        finite::store_sequence();
    };
    
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation for violations
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // search for each restriction in the permutation
            if( restrictions.find( std::make_pair(i+1,v[i]) ) != restrictions.end())
                return true;
        }
        return false;
    }
    
    /** Finds and returns the first in lexicographic ordering of the sequence with restrictions
     @returns the first in lexicographic ordering of the sequence with restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        
        // while we are violating at least one restriction, keep going
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // If we made it here, then we have updated v to a valid permutation which avoids all restrictions
        // and we didn't restart the sequence
        return true;
    }
    
    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::resize(size_t input_n) {
        n = input_n;
        reinitialize(); // <-- basically, recompute everything.
    };
    
    /** Inserts a new restriction
     @param res is a new restriction of the form {sigma(a),a}, i.e., a is not in location sigma(a)
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::insert(const std::pair<size_t, size_t>& res) {
        
        // inserts a new restriction, then need to recompute everything
        restrictions.insert(res);
        
        reinitialize();
        
        // A potential optimization is to simply update the current set of permutations, using a call to a function like reinitialize_with_added_restrictions() so that we don't have to go through the entire list again.
        
        // However, if we are using a random access list, more likely than not iterating through all n! is practical, and so we (presumably) do not lose much by not including this optimization at present.
        
    };
    
    /** Inserts a new restriction
     @param res is a new restriction of the form {sigma(a),a}, i.e., a is not in location sigma(a)
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::insert(std::pair<size_t, size_t>&& res) {
        
        // inserts a new restriction, then need to recompute everything
        restrictions.insert(res);
        reinitialize();
    };
    
    /** Inserts a collection of restrictions in an initializer list
     @param res is a set of new restrictions of the form {{sigma(a),a}, sigma(b),b},...}, i.e., a is not in location sigma(a), b is not in location sigma(b), etc.
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::insert(std::initializer_list<std::pair<size_t,size_t> > res) {
        
        // inserts new restrictions, then need to recompute everything
        for(auto& x : res)
            restrictions.insert(x);
        
        reinitialize();
        
    }
    
    /** Inserts a collection of restrictions any collection indexed by an input iterator type
     @tparam InputIterator is any iterator of type input iterator which when dereferenced returns a std::pair<size_t, size_t>
     @param res is a collection of new restrictions of the form {{sigma(a),a}, sigma(b),b},...}, i.e., a is not in location sigma(a), b is not in location sigma(b), etc.
     */
    template<typename T, typename V, typename SV>
    template<typename InputIterator>
    void permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::insert(InputIterator start, InputIterator stop) {
        
        // inserts new restrictions, then need to recompute everything
        while(start != stop)
            restrictions.insert(*start++);
        
        reinitialize();
    }
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::operator bool() {
        // The "null" state is simply a vector with size 0.
        return first_element != static_cast<V>(0);
    };
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_pairs,T,V,SV>::reinitialize() {
        
        typedef dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::by_pairs,T,V>, V> finite;
        
        // This is the same as the constructor, brute force enumerates through all possibilities again.
        finite::store_sequence();
        first_element = first_in_sequence();
        
        // a more efficient version would distinguish between adding more restrictions, in which case we don't need to iterate through the entire list again, merely take the current list and get rid of certain elements.
        
    }
    
    
    
    
    // Special: bidirectional, restrictions by pairs
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::bidirectional,permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, const std::set< std::pair<size_t, size_t> >& initialize_restrictions );
        
        // Constructor using an initializer list
        permutation(size_t input_n, std::initializer_list<std::pair<size_t,size_t> > initial_list);
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        V last_in_sequence() const;
        bool next_in_sequence(V& v) const;
        bool previous_in_sequence(V& v) const;
        
        
        void insert(const std::pair<size_t, size_t>& res);
        void insert(std::pair<size_t, size_t>&& res);
        void insert(std::initializer_list<std::pair<size_t,size_t> > res);
        
        template<typename InputIterator>
        void insert(InputIterator start, InputIterator stop);
        
        
        void clear() { restrictions.clear(); };
        
        operator bool();
        
        //friend std::ostream& operator<<(std::ostream& out, const permutation& p) {
        //    return out << (p.permutation);
        //};
        
        
    private:
        size_t n;
        V first_element;
        V last_element;
        
        //V permutation;
        std::set< std::pair<size_t, size_t > > restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n) : n(input_n) {
        
        // Prepare the first and last elements in the sequence.
        first_element = first_in_sequence();
        last_element = last_in_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n, const std::set< std::pair<size_t, size_t> >& initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions) {
        
        // Prepare the first and last elements in the sequence.
        first_element = first_in_sequence();
        last_element = last_in_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions with an initializer list of the form {{sigma(a), a},{sigma(b),b},...} meaning a cannot be in location sigma(a), b cannot be in location sigma(b), etc., and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n, std::initializer_list<std::pair<size_t,size_t> > initial_list) : n(input_n), restrictions(initial_list) {
        
        // Prepare the first and last elements in the sequence.
        first_element = first_in_sequence();
        last_element = last_in_sequence();
    };
    
    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // We have a valid permutation, and did not restart from the beginning.
        return true;
    }
    
    /** Given a current state, updates the input to the previous state, returns false if previous state restarts the sequence.
     @param v is the input state, which is updated to the previous state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::previous_in_sequence(V& v) const {
        
        // So first need to get the previous permutation, and if we have gone all the way around we need to stop
        if(!std::prev_permutation(std::begin(v), std::end(v)))
            return false;
        
        // keep going until we stop violating any restrictions
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::prev_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::prev_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // Obtained a permutation free from violations without going all the way around.
        return true;
    }
    
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // Check for a violation
            if( restrictions.find( std::make_pair(i+1,v[i]) ) != restrictions.end())
                return true;
        }
        return false;
    }
    
    
    /** Compute the first instance of a permutation with given restrictions in lexicographic ordering
     @returns the first permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        // If no such permutations exist, set v equal to the set with size 0.
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    
    /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
     @returns the last permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::last_in_sequence() const {
        
        //n = input_n;
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        // C++14 code
        //std::iota( std::rbegin(v), std::rend(v), static_cast<T>(1));
        
        // C++11 code, excludes pointer types
        std::iota( v.rbegin(), v.rend(), static_cast<T>(1));
        
        
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::prev_permutation( std::begin(v), std::end(v))) ) { }
        
        // If no such permutations exist, set v equal to the set with size 0.
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::resize(size_t input_n) {
        
        // Resize, then recompute everything
        n = input_n;
        reinitialize();
    };
    
    /** Inserts a new restriction
     @param res is a new restriction of the form {sigma(a),a}, i.e., a is not in location sigma(a)
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::insert(const std::pair<size_t, size_t>& res) {
        
        // Insert new restriction, then recompute.
        restrictions.insert(res);
        reinitialize();
    };
    
    /** Inserts a new restriction
     @param res is a new restriction of the form {sigma(a),a}, i.e., a is not in location sigma(a)
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::insert(std::pair<size_t, size_t>&& res) {
        
        // Insert new restriction, then recompute.
        restrictions.insert(res);
        reinitialize();
    };
    
    /** Inserts a collection of restrictions in an initializer list
     @param res is a set of new restrictions of the form {{sigma(a),a}, sigma(b),b},...}, i.e., a is not in location sigma(a), b is not in location sigma(b), etc.
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::insert(std::initializer_list<std::pair<size_t,size_t> > res) {
        
        // Insert new restrictions, then recompute.
        for(auto& x : res)
            restrictions.insert(x);
        
        reinitialize();
        
    }
    
    /** Inserts a collection of restrictions any collection indexed by an input iterator type
     @tparam InputIterator is any iterator of type input iterator which when dereferenced returns a std::pair<size_t, size_t>
     @param res is a collection of new restrictions of the form {{sigma(a),a}, sigma(b),b},...}, i.e., a is not in location sigma(a), b is not in location sigma(b), etc.
     */
    template<typename T, typename V, typename SV>
    template<typename InputIterator>
    void permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::insert(InputIterator start, InputIterator stop) {
        
        // Insert restrictions, then recompute.
        while(start != stop)
            restrictions.insert(*start++);
        
        reinitialize();
    }
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::operator bool() {
        // Check if there are any elements by testing against the array with 0 elements.
        return first_element != static_cast<V>(0);
    };
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_pairs,T,V,SV>::reinitialize() {
        
        // recompute the first and last elements.
        first_element = first_in_sequence();
        last_element = last_in_sequence();
    }
    
    
    
    
    
    
    
    
    
    
    
    
    // Special: forward, restrictions by pairs
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::forward,permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_pairs,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_pairs,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, const std::set< std::pair<size_t, size_t> >& initialize_restrictions );
        
        // Constructor using an initializer list
        permutation(size_t input_n, std::initializer_list<std::pair<size_t,size_t> > initial_list);
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        bool next_in_sequence(V& v) const;
        
        
        // inserts
        void insert(const std::pair<size_t, size_t>& res);
        void insert(std::pair<size_t, size_t>&& res);
        void insert(std::initializer_list<std::pair<size_t,size_t> > res);
        
        template<typename InputIterator>
        void insert(InputIterator start, InputIterator stop);
        
        /** Clears all restrictions, does NOT recompute. */
        void clear() { restrictions.clear(); };
        
        operator bool();
        
    private:
        size_t n;
        V first_element;
        V last_element;
        
        std::set< std::pair<size_t, size_t > > restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n) : n(input_n) {
        first_element = first_in_sequence();
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n, const std::set< std::pair<size_t, size_t> >& initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions) {
        first_element = first_in_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions with an initializer list of the form {{sigma(a), a},{sigma(b),b},...} meaning a cannot be in location sigma(a), b cannot be in location sigma(b), etc., and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::permutation(size_t input_n, std::initializer_list<std::pair<size_t,size_t> > initial_list) : n(input_n), restrictions(initial_list) {
        
        first_element = first_in_sequence();
    };
    
    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        
        // while we are violating a restriction, go to the next permutation
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // if we got this far, v does not violate any restrictions and hasn't started over yet.
        return true;
    }
    
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // Check each restriction against each entry
            if( restrictions.find( std::make_pair(i+1,v[i]) ) != restrictions.end())
                return true;
        }
        return false;
        
    }
    
    /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
     @returns the last permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        // resize to 0, i.e., the "null" state, indicating no such permutations exist with the given restrictions
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::resize(size_t input_n) {
        n = input_n;
        reinitialize();
    };
    
    /** Inserts a new restriction
     @param res is a new restriction of the form {sigma(a),a}, i.e., a is not in location sigma(a)
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::insert(const std::pair<size_t, size_t>& res) {
        
        // Add new restriction, then reinitialize
        restrictions.insert(res);
        reinitialize();
    };
    
    /** Inserts a new restriction
     @param res is a new restriction of the form {sigma(a),a}, i.e., a is not in location sigma(a)
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::insert(std::pair<size_t, size_t>&& res) {
        
        // Add new restriction, then reinitialize
        restrictions.insert(res);
        reinitialize();
    };
    
    /** Inserts a collection of restrictions in an initializer list
     @param res is a set of new restrictions of the form {{sigma(a),a}, sigma(b),b},...}, i.e., a is not in location sigma(a), b is not in location sigma(b), etc.
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::insert(std::initializer_list<std::pair<size_t,size_t> > res) {
        
        // Add new restrictions, then reinitialize
        for(auto& x : res)
            restrictions.insert(x);
        
        reinitialize();
        
    }
    
    /** Inserts a collection of restrictions any collection indexed by an input iterator type
     @tparam InputIterator is any iterator of type input iterator which when dereferenced returns a std::pair<size_t, size_t>
     @param res is a collection of new restrictions of the form {{sigma(a),a}, sigma(b),b},...}, i.e., a is not in location sigma(a), b is not in location sigma(b), etc.
     */
    template<typename T, typename V, typename SV>
    template<typename InputIterator>
    void permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::insert(InputIterator start, InputIterator stop) {
        
        // Add new restrictions, then reinitialize
        while(start != stop)
            restrictions.insert(*start++);
        
        reinitialize();
    }
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::operator bool() {
        // check for the "null" state to determine if any permutations exist with given restrictions
        return first_element != static_cast<V>(0);
    };
    
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_pairs,T,V,SV>::reinitialize() {
        first_element = first_in_sequence();
    }
    

    
    
    
    
    // Special: random access, restrictions by pairs
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::random_access,restrictions::by_function,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::random_access,permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::by_function,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::by_function,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, std::function<bool(V&)> restriction_function);
        void replace_restriction_function(std::function<bool(V&)> new_restriction_function);
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        bool next_in_sequence(V& v) const;
        
        
        operator bool();
        
    private:
        size_t n;
        V first_element;
        
        std::function<bool(V&)> restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::permutation(size_t input_n) : n(input_n), restrictions([](V& v) { return false;}) {
        first_element = first_in_sequence();
        finite::store_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::permutation(size_t input_n, std::function<bool(V&)> initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions) {
        
        first_element = first_in_sequence();
        finite::store_sequence();
        
    };
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation for violations
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // search for presence of restriction in the permutation
            if( restrictions(v) )
                return true;
        }
        return false;
    }
    
    /** Finds and returns the first in lexicographic ordering of the sequence with restrictions
     @returns the first in lexicographic ordering of the sequence with restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        
        // while we are violating at least one restriction, keep going
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // If we made it here, then we have updated v to a valid permutation which avoids all restrictions
        // and we didn't restart the sequence
        return true;
    }
    
    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::resize(size_t input_n) {
        n = input_n;
        reinitialize(); // <-- basically, recompute everything.
    };
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::operator bool() {
        // The "null" state is simply a vector with size 0.
        return first_element != static_cast<V>(0);
    };
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::reinitialize() {
        
        typedef dsl::finite_sequence<dsl::store::random_access, permutation<dsl::store::random_access,restrictions::by_function,T,V>, V> finite;
        
        // This is the same as the constructor, brute force enumerates through all possibilities again.
        finite::store_sequence();
        first_element = first_in_sequence();
        
        // a more efficient version would distinguish between adding more restrictions, in which case we don't need to iterate through the entire list again, merely take the current list and get rid of certain elements.
        
    }
    
    
    /** replaces restriction function with the one input, recalculates the set of objects
        @param new_restriction_function is the new function to test against violations
    */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::random_access,restrictions::by_function,T,V,SV>::replace_restriction_function(std::function<bool(V&)> new_restriction_function) {
        restrictions = new_restriction_function;
        reinitialize();
    }
    
    
    
    // Special: bidirectional, restrictions by function
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::bidirectional,permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::by_function,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::bidirectional, permutation<dsl::store::bidirectional,restrictions::by_function,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, std::function<bool(V&)> restriction_function);
        void replace_restriction_function(std::function<bool(V&)> new_restriction_function);
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        V last_in_sequence() const;
        bool next_in_sequence(V& v) const;
        bool previous_in_sequence(V& v) const;
        
        operator bool();
        
        //friend std::ostream& operator<<(std::ostream& out, const permutation& p) {
        //    return out << (p.permutation);
        //};
        
        
    private:
        size_t n;
        V first_element;
        V last_element;
        
        std::function<bool(V&)> restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::permutation(size_t input_n) : n(input_n), restrictions([](V& v) { return false;}){
        
        // Prepare the first and last elements in the sequence.
        first_element = first_in_sequence();
        last_element = last_in_sequence();
        
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::permutation(size_t input_n, std::function<bool(V&)> initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions) {
        
        first_element = first_in_sequence();
        last_element = last_in_sequence();
        
        
    };
    
    
    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // We have a valid permutation, and did not restart from the beginning.
        return true;
    }
    
    /** Given a current state, updates the input to the previous state, returns false if previous state restarts the sequence.
     @param v is the input state, which is updated to the previous state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::previous_in_sequence(V& v) const {
        
        // So first need to get the previous permutation, and if we have gone all the way around we need to stop
        if(!std::prev_permutation(std::begin(v), std::end(v)))
            return false;
        
        // keep going until we stop violating any restrictions
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::prev_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::prev_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // Obtained a permutation free from violations without going all the way around.
        return true;
    }
    
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // Check for a violation
            if( restrictions(v) )
                return true;
        }
        
        // Passed the test!  No violations
        return false;
    }
    
    
    /** Compute the first instance of a permutation with given restrictions in lexicographic ordering
     @returns the first permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        // If no such permutations exist, set v equal to the set with size 0.
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    
    /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
     @returns the last permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::last_in_sequence() const {
        
        //n = input_n;
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        // C++14 code
        //std::iota( std::rbegin(v), std::rend(v), static_cast<T>(1));
        
        // C++11 code, excludes pointer types
        std::iota( v.rbegin(), v.rend(), static_cast<T>(1));
        
        
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::prev_permutation( std::begin(v), std::end(v))) ) { }
        
        // If no such permutations exist, set v equal to the set with size 0.
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::resize(size_t input_n) {
        
        // Resize, then recompute everything
        n = input_n;
        reinitialize();
    };
    
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::operator bool() {
        // Check if there are any elements by testing against the array with 0 elements.
        return first_element != static_cast<V>(0);
    };
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::reinitialize() {
        
        // recompute the first and last elements.
        first_element = first_in_sequence();
        last_element = last_in_sequence();
    }
    
    /** replaces restriction function with the one input, recalculates the set of objects
     @param new_restriction_function is the new function to test against violations
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::bidirectional,restrictions::by_function,T,V,SV>::replace_restriction_function(std::function<bool(V&)> new_restriction_function) {
        restrictions = new_restriction_function;
        reinitialize();
    }
    
    
    
    
    
    
    // Special: forward, restrictions by pairs
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::forward,restrictions::by_function,T,V,SV> :
    //public  dsl::finite_sequence<dsl::store::forward,permutation<dsl::store::forward,restrictions::by_function,T,V,SV>,V,SV>,
    public dsl::finite_sequence_threadable<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V,SV>,V,SV>
    {
        
        typedef dsl::finite_sequence<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V>, V> finite;
        
        typedef dsl::finite_sequence_threadable<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V>, V> finite_threadable;
        
        
        friend typename dsl::finite_sequence<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V>, V>::iterator;
        
        friend typename dsl::finite_sequence_threadable<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, std::function<bool(V&)> restriction_function);
        void replace_restriction_function(std::function<bool(V&)> new_restriction_function);
        
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        V first_in_sequence(size_t i) const;
        bool next_in_sequence(V& v) const;
        
        operator bool();
        
    private:
        size_t n;
        V first_element;
        V last_element;
        
        std::function<bool(V&)> restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, with no restrictions, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::permutation(size_t input_n) : n(input_n), restrictions([](V& v) { return false;}),finite_threadable(input_n) {
        first_element = first_in_sequence();
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::permutation(size_t input_n, std::function<bool(V&)> initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions), finite_threadable(input_n) {
        
        first_element = first_in_sequence();
        
    };
    
    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        
        // while we are violating a restriction, go to the next permutation
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // if we got this far, v does not violate any restrictions and hasn't started over yet.
        return true;
    }
    
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // Check each restriction against each entry
            if( restrictions(v))
                return true;
        }
        return false;
        
    }
    
    /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
     @returns the last permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        // resize to 0, i.e., the "null" state, indicating no such permutations exist with the given restrictions
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };

    
    /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
     @returns the last permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::first_in_sequence(size_t i) const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        i=i+1;
        
        if(i == 1 || i == n+1)
            // Fill with {1,2,...,n}
            std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        else {
        auto it = std::begin(v);
        // Initialize as {i,1,2,...,i-1,i+1,...,n}
        *it++ = i;
        
        // fill in {i,1,2,...,i-1
        for(T j=1;j<=i-1;j++)
            *it++ = j;
        
        // fill in i+1,i+2,...,n}
        for(T j=i+1;j<=n;j++)
            *it++ = j;
            
        }
        
        // Fill with {1,2,...,n}
        //std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        // resize to 0, i.e., the "null" state, indicating no such permutations exist with the given restrictions
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };

    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::resize(size_t input_n) {
        n = input_n;
        reinitialize();
    };
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::operator bool() {
        // check for the "null" state to determine if any permutations exist with given restrictions
        return first_element != static_cast<V>(0);
    };
    
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::reinitialize() {
        first_element = first_in_sequence();
    }
    
    
    /** replaces restriction function with the one input, recalculates the set of objects
     @param new_restriction_function is the new function to test against violations
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::replace_restriction_function(std::function<bool(V&)> new_restriction_function) {
        restrictions = new_restriction_function;
        reinitialize();
    }

    
    
    
    
    
    
    // Save this in case extension above fails!
#if 0
    // Special: forward, restrictions by pairs
    template<typename T, typename V, typename SV>
    class permutation<dsl::store::forward,restrictions::by_function,T,V,SV> :
    public  dsl::finite_sequence<dsl::store::forward,permutation<dsl::store::forward,restrictions::by_function,T,V,SV>,V,SV> {
        
        typedef dsl::finite_sequence<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V>, V> finite;
        friend typename dsl::finite_sequence<dsl::store::forward, permutation<dsl::store::forward,restrictions::by_function,T,V>, V>::iterator;
        
    public:
        
        permutation(size_t input_n);
        
        // Constructor using a set of pairs
        permutation(size_t input_n, std::function<bool(V&)> restriction_function);
        void replace_restriction_function(std::function<bool(V&)> new_restriction_function);
        
        
        void resize(size_t n);
        
        V first_in_sequence() const;
        bool next_in_sequence(V& v) const;
        
        operator bool();
        
    private:
        size_t n;
        V first_element;
        V last_element;
        
        std::function<bool(V&)> restrictions;
        
        bool violates_restrictions(V& v) const;
        void reinitialize();
        
    };
    
    
    
    /** Initializes permutation to have size n, with no restrictions, computes the first element in the sequence, and stores the entire sequence.
     @param input_n is the initial size of the permutation.
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::permutation(size_t input_n) : n(input_n), restrictions([](V& v) { return false;}) {
        first_element = first_in_sequence();
    };
    
    /** Initializes permutation to have size n, initializes the restrictions, and computes the sequence.
     @param input_n is the initial size of the permutation
     @param initialize_restrictions is the initial set of restrictions
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::permutation(size_t input_n, std::function<bool(V&)> initialize_restrictions ) : n(input_n), restrictions(initialize_restrictions) {
        
        first_element = first_in_sequence();
        
    };

    
    /** Given a current state, updates the input to the next state, returns false if next state restarts the sequence.
     @param v is the input state, which is updated to the next state
     @returns whether or not the sequence restarted
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::next_in_sequence(V& v) const {
        
        // So first need to get the next permutation, and if we have gone all the way around we need to stop
        if(!std::next_permutation(std::begin(v), std::end(v)))
            return false;
        
        // while we are violating a restriction, go to the next permutation
        while(violates_restrictions(v)) {
            
            // If we come back to the beginning
            if( !std::next_permutation( std::begin(v), std::end(v))) {
                
                // Then we need to make sure to get to the first non-violating position
                while( violates_restrictions(v) ) {
                    std::next_permutation( std::begin(v), std::end(v));
                }
                
                // Return false indicating that we have reached the starting location
                return false;
            }
            
        }
        
        // if we got this far, v does not violate any restrictions and hasn't started over yet.
        return true;
    }
    
    
    /** tests for whether the input violates the restrictions
     @param v is the current state of the permutation
     @returns true if v violates the restrictions, false otherwise
     */
    template<typename T, typename V, typename SV>
    bool permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::violates_restrictions(V& v) const {
        
        // Check each part of the permutation
        for(size_t i=0, n=v.size(); i<n;i++) {
            
            // Check each restriction against each entry
            if( restrictions(v))
                return true;
        }
        return false;
        
    }
    
    /** Compute the last instance of a permutation with given restrictions in lexicographic ordering
     @returns the last permutation in lexicographic ordering with the given restrictions
     */
    template<typename T, typename V, typename SV>
    V permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::first_in_sequence() const {
        
        V v(n);
        
        // If n == 0, then return the empty partition.
        if(n == 0) {
            v.resize(0);
            return v;
        }
        
        // Fill with {1,2,...,n}
        std::iota( std::begin(v), std::end(v), static_cast<T>(1));
        
        bool existence = true;
        
        // While the restrictions are violated, go to next permutation
        while( violates_restrictions(v) && (existence = std::next_permutation( std::begin(v), std::end(v))) ) { }
        
        // resize to 0, i.e., the "null" state, indicating no such permutations exist with the given restrictions
        if(!existence) {
            v.resize(0);
        }
        
        return v;
    };
    
    
    
    /** Resizes the permutation and then reinitializes with the set of permutations
     @param input_n is the new size of the permutation
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::resize(size_t input_n) {
        n = input_n;
        reinitialize();
    };
    
    /** conversion to bool, returns true unless the restrictions are too tough and no such set of permutations exist.
     @returns true if there are no elements, false otherwise
     */
    template<typename T, typename V, typename SV>
    permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::operator bool() {
        // check for the "null" state to determine if any permutations exist with given restrictions
        return first_element != static_cast<V>(0);
    };
    
    
    /** reinitializes all of the elements in the sequence, typically after adding some new type of restrictions. */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::reinitialize() {
        first_element = first_in_sequence();
    }
    

    /** replaces restriction function with the one input, recalculates the set of objects
     @param new_restriction_function is the new function to test against violations
     */
    template<typename T, typename V, typename SV>
    void permutation<dsl::store::forward,restrictions::by_function,T,V,SV>::replace_restriction_function(std::function<bool(V&)> new_restriction_function) {
        restrictions = new_restriction_function;
        reinitialize();
    }
#endif
    
    
    
}



    
    
    
    
    



#endif
