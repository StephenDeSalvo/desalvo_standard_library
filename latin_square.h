//
//  latin_square.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 3/27/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

/** @file latin_square.h
 @brief Classes and functions for Latin squares of all orders
 
 A Latin square of order n is an n by n table where each row and each column is a permutation of {1,2,...,n}.
 */

#ifndef DeSalvo_Standard_Library_latin_square_h
#define DeSalvo_Standard_Library_latin_square_h

#include <algorithm>
#include <future>
#include <thread>

#include "statistics.h"
#include "contingency_table.h"



namespace desalvo_standard_library {
    
    
    
   // See Documentation.h for full documentation
    template<typename ValueType=unsigned int, typename WorkingPrecision = unsigned int, typename Table=dsl::numerical_table<ValueType,WorkingPrecision> >
    class latin_square : public  dsl::finite_sequence<dsl::store::bidirectional, latin_square<ValueType,WorkingPrecision,Table>, Table> {
        
        
        typedef dsl::finite_sequence<dsl::store::bidirectional, latin_square<ValueType,WorkingPrecision,Table>, Table> finite;
        
        
        friend typename
        dsl::finite_sequence<dsl::store::bidirectional, latin_square<ValueType,WorkingPrecision,Table>, Table>::iterator;
        
    public:
        
        template<typename Parameters=unsigned int>
        class object;
        
        template<typename Parameters=unsigned int>
        class generator;
        
        
        // default initialization not allowed, must be initialized with a size.
        latin_square(unsigned int input_n = 0) : N(input_n) { };
        
        
        template<typename Parameters>
        class object : public Table {
        public:
            object(unsigned int N) : Table(N,N,0), n(N) {
                //latin_square ls;
                
                
                unsigned int top = 0;
                unsigned int increment = top;
                
                for(size_t i=0;i<N;++i) {
                    increment = top;
                    for(auto& x : this->row(i) ){
                        x = increment+=1;
                        increment = increment % N;
                    }
                    ++top;
                }
                
            }
            
            object(numerical_table<ValueType,WorkingPrecision> t) : numerical_table<ValueType,WorkingPrecision>(t), n(t.size_row()) { }
            
            
            
            class iterator_random_access;
            class iterator_bidirectional;
            class iterator_forward;
            
            bool all_conditions_satisfied(int verbose = 0) {
                
                // Check all row conditions
                for(size_t i=0;i<n;++i)
                    if(!dsl::is_permutation_of_n(this->begin_row(i), this->end_row(i),n)) {
                        if(verbose >=2) {
                            std::cout << "Error in latin_square: row " << i << " is not a permutation" << std::endl;
                        }
                        
                        return false;
                    }
                
                // Check all column conditions
                for(size_t i=0;i<n;++i)
                    if(!dsl::is_permutation_of_n(this->begin_column(i), this->end_column(i),n)) {
                        if(verbose >=2) {
                            std::cout << "Error in latin_square: column " << i << " is not a permutation" << std::endl;
                        }
                        return false;
                    }
                
                return true;
            }
            
            
        private:
            Parameters n;
            
            
        };
        
        template<typename Parameters>
        class generator {
        public:
            
            
            generator(Parameters initial_p) : n(initial_p) { };
            
            template<typename URNG=std::mt19937_64>
            typename latin_square::object<Parameters> operator()(URNG& gen = generator_64, int verbose=0);
            
            
            
        private:
            Parameters n;
            
            template<typename Vector, typename URNG = std::mt19937_64>
            Table bit_recursion(Vector rows, Vector columns, Table zeros, Table ones, unsigned int level, URNG& gen= generator_64, int verbose=0) {
                
                auto n = zeros.size_column();
                auto m = zeros.size_row();
                
                if(verbose >=1) {
                    std::cout << "r = " << rows << std::endl;
                    std::cout << "c = " << columns << std::endl;
                }
                
                
                Table& W = zeros;
                
                //EntryType one = 1;
                Table all_ones(m,n,1);
                Table all_zeros(m,n,0);
                
                auto sum_zeros = n - std::accumulate(zeros.begin_row(0), zeros.end_row(0), 0);
                auto sum_ones = n - std::accumulate(ones.begin_row(0), ones.end_row(0), 0);
                
                Table B0(n,n,0);
                Table B1(n,n,0);
                
                Table A0(m,n,0);
                Table A1(m,n,0);
                
                
                // If it is time to end both of the recursions
                if(sum_zeros<=1 && sum_ones <=1) {
                    return Table(n,n,0);
                }  // otherwise, if just the sum of the ones has terminated, then
                if(sum_ones <=1 ) {
                    
                    // Ignore B1, keep it to the initialized value of 0 above
                    // Just do the zeros recursion
                    
                    if(verbose >=1) std::cout << "W = \n" << zeros << std::endl;
                
                
                        for(auto& x : rows) x = sum_zeros/2;
                        for(auto& x : columns) x = sum_zeros/2;
                        
                        //A1 = bit_recursion(rows, columns, W10, W11, level+1,gen,verbose);
                    
                    auto obj0 = random_binary_contingency_table_with_zeros(rows, columns, zeros,gen);
                  
                    B0 = obj0;
                    
                    if(verbose >=1) {
                        std::cout << "B0=\n" << B0 << std::endl;
                    }
                    
                    auto W00 = zeros+B0;
                    auto W01 = zeros;
                    
                    // How to form W01 starting with current set of zeros and put in 1 wherever B0 = 0 and zeros = 0
                    for(size_t i=0;i<m;++i)
                        for(size_t j=0;j<n;++j)
                            if(W01(i,j) == 0 && B0(i,j) == 0)
                                W01(i,j) = 1;
                    
                    if(verbose >= 1) {
                        std::cout << "W00=\n" << W00 << std::endl;
                        std::cout << "W01=\n" << W01 << std::endl;
                    }
                    
                    auto sum_zeros = n - std::accumulate(W01.begin_row(0), W01.end_row(0), 0);
                    
                    if(sum_zeros > 1) {
                        for(auto& x : rows) x = sum_zeros/2;
                        for(auto& x : columns) x = sum_zeros/2;
                    
                    //if(std::accumulate(std::begin(rows), std::end(rows),0) > 0 && std::accumulate(std::begin(columns), std::end(columns),0) > 0) {
                        A0 = bit_recursion(rows, columns, W00, W01, level+1, gen, verbose);
                    //}
                    // B0 and A0 have values, B1 and A1 are zeros
                    //return B0+A0;
                    }
                    
                }
                else { // both are ripe for recursion!
                    
                    
                    
                    if(verbose >=1)
                        std::cout << "W = \n" << zeros << std::endl;
                    
                    for(auto& x : rows) x = sum_zeros/2;
                    for(auto& x : columns) x = sum_zeros/2;
                    
                    //auto obj0 = random_binary_contingency_table_with_zeros(rows, columns, zeros,gen);
                    typename binary_contingency_table_with_zeros<ValueType,ValueType,Vector,Table>::template object<std::tuple<Vector,Vector,Table> > obj0(rows, columns, zeros);
                    std::thread zeros_thread = std::thread(dsl::random_binary_contingency_table_with_zeros_for_thread<ValueType,ValueType,
                                              Vector,
                                              Table,
                                              std::tuple<Vector,Vector,Table>,
                                                           URNG>, std::ref(obj0), (rows), (columns), (zeros), std::ref(gen), 0);
                    
                    
                    for(auto& x : rows) x = sum_ones/2;
                    for(auto& x : columns) x = sum_ones/2;
                    
                    auto gen2 = gen;
                    
                    //auto obj1 = random_binary_contingency_table_with_zeros(rows, columns, ones,gen);
                    typename binary_contingency_table_with_zeros<ValueType,ValueType,Vector,Table>::template object<std::tuple<Vector,Vector,Table> > obj1(rows, columns, ones);
                    std::thread ones_thread = std::thread(dsl::random_binary_contingency_table_with_zeros_for_thread<ValueType,ValueType,
                                             Vector,
                                             Table,
                                             std::tuple<Vector,Vector,Table>,
                                                          URNG>, std::ref(obj1), (rows), (columns), (ones), std::ref(gen2), 0);

                    
                    
                    
                    zeros_thread.join();
                    ones_thread.join();
                    
                    B0 = obj0;
                    B1 = obj1;
                    
                    if(verbose >=1) {
                        std::cout << "B0=\n" << B0 << std::endl;
                        std::cout << "B1=\n" << B1 << std::endl;
                    }
                    
                    auto W00 = zeros+B0;
                    auto W01 = zeros;
                    
                    // How to form W01 starting with current set of zeros and put in 1 wherever B0 = 0 and zeros = 0
                    for(size_t i=0;i<m;++i)
                        for(size_t j=0;j<n;++j)
                            if(W01(i,j) == 0 && B0(i,j) == 0)
                                W01(i,j) = 1;
                    
                    if(verbose >= 1) {
                        std::cout << "W00=\n" << W00 << std::endl;
                        std::cout << "W01=\n" << W01 << std::endl;
                    }
                    
                    
                    auto W10 = ones+B1;
                    auto W11 = ones;
                    
                    // How to form W01 starting with current set of zeros and put in 1 wherever B0 = 0 and zeros = 0
                    for(size_t i=0;i<m;++i)
                        for(size_t j=0;j<n;++j)
                            if(W11(i,j) == 0 && B1(i,j) == 0)
                                W11(i,j) = 1;
                    
                    if(verbose >=1) {
                        std::cout << "W10=\n" << W10 << std::endl;
                        std::cout << "W11=\n" << W11 << std::endl;
                    }
                    
                    auto sum_zeros = n - std::accumulate(W00.begin_row(0), W00.end_row(0), 0);
                    
                    
                    if(sum_zeros >1) {
                        for(auto& x : rows) x = sum_zeros/2;
                        for(auto& x : columns) x = sum_zeros/2;
                        
                        A0 = bit_recursion(rows, columns, W00, W01, level+1,gen,verbose);
                    }
                    
                    auto sum_ones = n - std::accumulate(W10.begin_row(0), W10.end_row(0), 0);
                    
                    if(sum_ones > 1) {
                        
                        for(auto& x : rows) x = sum_ones/2;
                        for(auto& x : columns) x = sum_ones/2;
                        
                        A1 = bit_recursion(rows, columns, W10, W11, level+1,gen,verbose);
                    }
                    
                    
                    //auto sum_ones = n - std::accumulate(ones.begin_row(0), ones.end_row(0), 0);
                    
                    //}
                    
                    //for(auto& x : rows) x /= 2;
                    //for(auto& x : columns) x /= 2;
                    
                    if(verbose >=1) {
                        std::cout << "r = " << rows << std::endl;
                        std::cout << "c = " << columns << std::endl;
                    }
                    
                    /*
                     auto A0 = A;
                     if(W0 != ones)
                     A0 = A + 2*bit_recursion(r0, c0, W0,level+1);
                     
                     std::cout << "A0 = \n" << std::pow(2,level)*A0 << std::endl;
                     
                     auto A1 = ((1-A)-W);
                     if(W1 != ones)
                     A1 = ((1-A)-W)+2*bit_recursion(rows, columns, W1,level+1);
                     */
                    
                }
                
                
                return (B0+B1)+2*(A0 + A1);
                
                //                std::cout << "A1 = \n" << std::pow(2,level)*A1 << std::endl;
                
                //                return (A0+A1);
                
            }
            
            
        };
        
        
        template<typename Parameters=unsigned int, typename URNG=std::mt19937_64>
        object<Parameters> random(ValueType n, URNG& gen = generator_64, int verbose = 0) {
            generator<Parameters> generate(n);
            return generate(gen, verbose);
        }
        
        
        template<typename Parameters>
        class iterator_random_access {
        public:
            
        private:
            
        };
        
        template<typename Parameters>
        class iterator_bidirectional {
        public:
            
        private:
            
        };
        
        template<typename Parameters>
        class iterator_forward {
        public:
            
        private:
            
        };
        
        
        
        
        template<typename Parameters=unsigned int>
        bool first_in_sequence(Table& t) const {
            
            // No more this->N defined, N is now local to object, generator, etc.
            t = object<Parameters>(N);
            
            
            return true;
        }
        
        template<typename Parameters=unsigned int>
        bool last_in_sequence(Table& t) const {
            
            first_in_sequence(t);
            
            std::transform(t.begin(), t.end(), t.begin(), [=](const ValueType& a) { return static_cast<ValueType>(N)+1 - a; });
            //t = (N+1) - t;
            
            
            return true;
        }
        
        
        
        /** ... */
        template<typename Parameters=unsigned int>
        bool next_in_sequence(Table& t) const {

            /*
            auto last = t;
            last_in_sequence(last);
            
            // If last element, return false
            if(last == t) {
                first_in_sequence(t);
                return false;
            }
             */
            
            // Need to check for whether current_row i is the permutation (i+1),...,n,1,2,...,i  instead of 1,2,...,n.
            std::vector<std::vector<ValueType>> canonical_rows(N,std::vector<ValueType>(N));
            std::vector<ValueType> temp_row(N);
            
            // Initialize canonical rows to compare inside loop.

            for(size_t i=0;i<N;++i)
                for(size_t j=0;j<N;++j)
                    canonical_rows[i][j] = (i+j) % N + 1;
            
            
            std::vector<ValueType> v(N);
            std::iota(begin(v), end(v), 1);
            
            size_t current_row = N-1;
            bool conditions = true;
            do {
                
                // update to next permutation
                std::next_permutation(t.begin_row(current_row)+1, t.end_row(current_row));
                std::copy(t.begin_row(current_row), t.end_row(current_row), std::begin(temp_row));
                
                //std::cout << temp_row << std::endl;
                //std::cout << canonical_rows[current_row] << std::endl;
                
                while(current_row > 1 && temp_row == canonical_rows[current_row]) {
                    current_row--;
                
                    // update to next permutation in current_row
                    std::next_permutation(t.begin_row(current_row)+1, t.end_row(current_row));
                    std::copy(t.begin_row(current_row), t.end_row(current_row), std::begin(temp_row));
                    
                }
                
                if(current_row == 1 && std::equal(t.begin_row(current_row), t.end_row(current_row), std::begin(canonical_rows[current_row]))) {
                    first_in_sequence(t);
                    return false;
                }
                current_row = N-1;
                
                // Check to make sure all rows and columns are permutations, except first of each is assumed.
                conditions = true;
                for(unsigned int i=1; i< N && conditions==true; ++i) {
                    conditions &= std::is_permutation(t.begin_row(i), t.end_row(i), std::begin(v));
                    conditions &= std::is_permutation(t.begin_column(i), t.end_column(i), std::begin(v));
                }
                //std::cout << conditions <<"*"<< std::endl;
                //std::cout << t << std::endl;
                
            } while(!conditions);
            
            return true;
            
        }

        
        
        
        
    private:
        unsigned int N;
    };
    
    

    template<typename ValueType = unsigned int, typename WorkingPrecision=unsigned int,
             typename Table=numerical_table<ValueType,WorkingPrecision>, typename Parameters=unsigned int, typename URNG=std::mt19937_64>
    typename latin_square<ValueType,WorkingPrecision,Table>::template object<Parameters>
    random_latin_square(ValueType n, URNG& gen = generator_64, int verbose = 0)  {
        
        latin_square<ValueType,WorkingPrecision,Table> ls(n);
        
        return ls.random(n,gen, verbose);
        
    }
    
    
    
    
    
    /** Implements the PDC algorithm using binary contingency table with forced zeros. */
    template<typename ValueType, typename WorkingType, typename Table>
    template<typename Parameters>
    template<typename URNG>
    typename latin_square<ValueType,WorkingType,Table>::template object<Parameters>
    latin_square<ValueType,WorkingType,Table>::generator<Parameters>::operator()(URNG& gen, int verbose) {
        
        
        // Do a few special cases directly.
        if(n==1) {
            return latin_square<ValueType,WorkingType,Table>::template object<Parameters>(numerical_table<ValueType, WorkingType>(1,1,0));
        }
        if(n==2) {
            std::uniform_int_distribution<unsigned int> u(0,1);
            numerical_table<ValueType, WorkingType> A(2,2,0);
            A(0,0) = 1; A(1,1) = 1;
            if(u(gen) <=0.5)
                return latin_square<ValueType,WorkingType,Table>::template object<Parameters>(A);
            else
                return latin_square<ValueType,WorkingType,Table>::template object<Parameters>(1-A);
        }
        if(n==3) {
            std::vector<ValueType> v {0,1,2};
            std::shuffle(std::begin(v), std::end(v), gen);
            std::vector<std::vector<ValueType>> a {{v[0],v[1],v[2]},{v[1],v[2],v[0]},{v[2],v[0],v[1]}};
            numerical_table<ValueType, WorkingType> A(a);
            
            std::uniform_int_distribution<unsigned int> u(0,1);
            if(u(gen) <= 0.5)
                A.swap_rows(1,2);
            
            return latin_square<ValueType,WorkingType,Table>::object<Parameters>(A);
        }
        
        bool flag = true;
        
        
        std::vector<unsigned int> rows(n,n/2);
        std::vector<unsigned int> columns(n,n/2);
        numerical_table<ValueType, WorkingType> zeros(n,n,ValueType(0));
        //typename binary_contingency_table_with_zeros<ValueType, WorkingType>::object<Parameters> initial(rows, columns, zeros);
        auto A = zeros;
        
        
        
        while(flag) {
            
            // First table has no forced zeros 
            auto initial = random_binary_contingency_table<ValueType,WorkingType>(rows, columns, gen);
            
            
            
            numerical_table<ValueType, WorkingType> W = initial.as_table();
            
            std::cout << "W = \n" << W << std::endl;
            
            //for(auto& x : rows) x = x/2;
            //for(auto& x : columns) x = x/2;
            
            A = W+(2*bit_recursion(rows, columns, W, 1-W, 0, gen,verbose));
            
            if(verbose >=1) std::cout << "ls=\n" << A << std::endl;
            
            latin_square<ValueType,WorkingType,Table>::object<Parameters> ls(A+1);
            
            
            if(!ls.all_conditions_satisfied(verbose)) {
                flag = true;
                if(verbose >=1) std::cout << "One or more Latin square conditions failed, restarting." << std::endl;
            }
            else
                return ls;
            
        }
        
        latin_square<ValueType,WorkingType,Table>::object<Parameters> ls(A+1);
        
        return ls;
        
    }
    
    
    typedef latin_square<unsigned int, unsigned int>::object<std::vector<int> > latin_square_object;
    
    
};





#endif
