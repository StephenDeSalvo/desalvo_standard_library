/** @file DeSalvoSudoku
 @brief contains functionality for random sampling of Sudoku matrices
 
 
 @code
 
 ds::sudoku<short> sudoku;
 
 auto x = sudoku.importance_sample(dsl::generator_64);
 auto x2 = sudoku.importance_sample_no_partial_information(dsl::generator_64);
 cout << x << endl;
 cout << x.get_weight() << endl;
 
 cout << x2 << endl;
 cout << x2.get_weight() << endl;
 
 @endcode
 */


//
//  DeSalvoSudokuLibrary.h
//  DeSalvoLibraryTestKit
//
//  Created by Stephen DeSalvo on 11/4/14.
//  Copyright (c) 2014 Stephen DeSalvo. All rights reserved.
//



#ifndef DeSalvoLibraryTestKit_DeSalvoSudokuLibrary_h
#define DeSalvoLibraryTestKit_DeSalvoSudokuLibrary_h

#include <string>
#include <vector>
#include <array>
#include <set>
#include <sstream>
#include <exception>


#include "numerical.h"
#include "statistics.h"
#include "std_cout.h"
#include "file.h"
#include "table.h"
#include "permutation.h"
#include "shrinking_set.h"

namespace dsl = desalvo_standard_library;


namespace desalvo_standard_library {
    
    
    template<typename ValueType = short>
    class sudoku : private dsl::table<ValueType> {
        
        friend std::ostream& operator<<(std::ostream& out, const sudoku& t) {
            return out << static_cast<dsl::table<ValueType>>(t);
        };
        
    public:
        
        sudoku(size_t n_input) : dsl::table<ValueType>(n_input,n_input), n(n_input) {
            
            // Quick and dirty check for perfect square
            if( fabs(sqrt(n) - round(sqrt(n))) > .00001) {
                std::cout << "Error!  Inputted value of n="<<n_input<<" for Sudoku is not a perfect square.  Adjusting to n=";
                size_t m = sqrt(n)+1;
                n = m*m;
                std::cout << n<<std::endl;
                
            }
            
            std::vector<ValueType> v = dsl::range<ValueType>(n);
            
            // Initialize to default valid Sudoku
            for(size_t i=0;i<n;++i) {
                std::copy_n(v.begin(), n, this->begin_row(i));
                std::rotate(v.begin(), v.begin()+1, v.end());
            }
            
        }
        
        
        /** iterator that iterates through elements in a given block. */
        class block_iterator {
            
        public:
            block_iterator(size_t i) {
                
                // i is the block number, say blocks are numberered left to right, top to bottom.
                
            }
            
            
        };
        
        
    private:
        size_t n;
        
    };
    
    
    
    
#if 0
    std::string SudokuFolder = "/Users/stephendesalvo/Documents/Research/C++ Code/SudokuSampling/SudokuSampling/";
    
    numeric_data<size_t> d_points;
    numeric_data<size_t> rejection_count;
    numeric_data<size_t> rejection_count2;
    
    typedef dsl::Permutation<dsl::SequenceType::StoreAll, dsl::PermutationType::RestrictionPairs> RestrictedPermutationList;
    
    typedef unsigned long long ull;
    
    long double number_of_sudokus = 6670903752021072936960.0;
    
    template<typename IntType = short>
    class sudoku : public dsl::Table<9,9,IntType> {
        friend std::ostream& operator<<(std::ostream& out, const sudoku& sud) {
            return out << static_cast<dsl::Table<9,9,IntType>>(sud);
        }
        
        friend std::istream& operator>>(std::istream& in, const sudoku& sud) {
            char c;
            in >> c;
            
            return in;
            //return out << static_cast<dsl::Table<9,9,IntType>>(sud);
        }
        
        
        
        
        friend class RejectionLookup;
        
    public:
        
        
        sudoku();
        sudoku(bool); // initializes to some valid default sudoku
        
        template<typename Iterator>
        void insert_block(Iterator it, size_t block);
        
        bool IsRowPerm(size_t row);
        bool IsColumnPerm(size_t col);
        bool IsBlockPerm(size_t block);
        std::vector< std::pair<size_t, size_t> > indices_of_fellow_block_entries(size_t row, size_t column);
        
        
        bool blocks_four_five_six();
        void block(size_t blck);
        void reduce();
        
        template<typename URNG>
        void final_randomization(URNG gen = dsl::generator_64) {
            this->permute_columns( dsl::random_permutation_shifted(3, 3, gen));
            this->permute_columns( dsl::random_permutation_shifted(3, 6, gen));
            
            // Flip a coin, swap columns (3,4,5) with columns (6,7,8)
            if( gen()%2 == 1 ) {
                this->swap_columns(3,6);
                this->swap_columns(4,7);
                this->swap_columns(5,8);
            }
            
            this->apply_permutation_map( dsl::random_permutation<size_t>(9,gen) );
            
        }
        
        
        template<typename URNG>
        void operator()(URNG& gen = dsl::generator_64) {
            sample_first_three_rows(gen);
            initialize_restricted_permutations();
            sample_remaining_rows(gen);
            final_randomization(gen);
            
            
        }
        
        
        IntType& operator()(size_t i, size_t j) {
            return dsl::Table<9,9,IntType>::operator()(i,j);
        }
        
        void setup_table();
        void initialize_restricted_permutations();
        bool is_sudoku();
        
        ull number_partial_completions(size_t row, size_t column);
        std::vector<std::vector<std::vector<IntType> > > set_of_three_row_partial_completions(size_t row, size_t column);
        
        template<typename URNG>
        void sample_first_three_rows(URNG& gen = dsl::generator_64);
        
        template<typename URNG>
        void sample_remaining_rows(URNG& gen = dsl::generator_64);
        
        
        bool last_three_rows_are_completed();
        
        bool last_two_rows_are_completed();
        void complete_last_row();
        bool last_row_is_completable();
        
        std::vector< std::pair< std::vector<IntType>, std::vector<IntType> > > set_of_possible_last_two_rows();
        std::vector<std::vector<std::vector<IntType> > > set_of_possible_last_three_rows();
        std::vector<std::vector<std::vector<IntType> > > set_of_possible_last_three_rows(size_t row, size_t column);

        void iterate_through_all_first_three_rows();
        void iterate_through_all_first_three_rows_smaller_set();
        
        std::set<IntType> allowable_part_sizes_using_set(size_t i, size_t j);
        dsl::shrinking_unordered_set<IntType> allowable_part_sizes(size_t i, size_t j);
        
        class object {
            friend std::ostream& operator<<(std::ostream& out, const sudoku<IntType>::object& sud) {
                return out << sud.values;
            }
            
        public:
            /** Initialize object with values
             @param table contains the matrix elements
             @param wt contains the importance sampling weight, wt = f(table)/g(table)
             */
            object(dsl::Table<9,9,IntType> table, double wt) : values(table), weight(wt) { };
            
            double get_weight() const { return weight; };
            
            std::string as_one_line_matlab_matrix() {
                
                std::string line = "[";
                
                for(size_t i=0;i<8;++i) {
                    auto it = values.begin_row(i);
                    auto stop = values.end_row(i)-1;
                    
                    while(it != stop) {
                        line += std::to_string( *it++ );
                        line += ",";
                    }
                    line += std::to_string(*it);
                    line += ";";
                }
                auto it = values.begin_row(8);
                auto stop = values.end_row(8)-1;
                
                while(it != stop) {
                    line += std::to_string( *it++ );
                    line += ",";
                }
                line += std::to_string(*it);
                line += "];";
                return line;
                
            }
            
            std::string as_one_line_mathematica_matrix() {
                
                std::string line = "{{";
                
                for(size_t i=0;i<8;++i) {
                    auto it = values.begin_row(i);
                    auto stop = values.end_row(i)-1;
                    
                    while(it != stop) {
                        line += std::to_string( *it++ );
                        line += ",";
                    }
                    line += std::to_string(*it);
                    line += "},{";
                }
                auto it = values.begin_row(8);
                auto stop = values.end_row(8)-1;
                
                while(it != stop) {
                    line += std::to_string( *it++ );
                    line += ",";
                }
                line += std::to_string(*it);
                line += "}},";
                return line;
                
            }
            
            
            
        private:
            dsl::Table<9,9,IntType> values;
            double weight;
        };
        
        
        template<typename URNG>
        sudoku::object importance_sample(URNG& gen = dsl::generator_64);
        template<typename URNG>
        sudoku::object importance_sample_using_set(URNG& gen = dsl::generator_64);
        
        template<typename URNG>
        sudoku::object importance_sample_no_partial_information(URNG& gen = dsl::generator_64);
        
        template<typename URNG>
        sudoku::object importance_sample_with_first_three_rows(URNG& gen = dsl::generator_64);
        
        template<typename URNG>
        typename sudoku<IntType>::object importance_sample_first_three_last_two(URNG& gen = dsl::generator_64);
        
        template<typename URNG>
        typename sudoku<IntType>::object importance_sample_first_three_last_three(URNG& gen);
            
        
        std::string as_one_line_matlab_matrix();
        
        class RejectionLookup {
            friend class Sudoku;
            friend bool operator<(const RejectionLookup& lhs, const RejectionLookup& rhs) {
                return lhs.hashed_value < rhs.hashed_value;
            }
            friend bool operator==(const RejectionLookup& lhs, const RejectionLookup& rhs) {
                return lhs.hashed_value == rhs.hashed_value;
            }
        public:
            //        RejectionLookup(ull mult) : multiplicity(mult) { };
            RejectionLookup() { };
            
            RejectionLookup(std::string line) {
                
                std::istringstream str(line);
                
                // [
                char ch = str.get();
                
                // First number
                int a; str >> a;
                
                // ,
                ch = str.get();
                
                // Second number
                int b; str >> b;
                
                // ,
                ch = str.get();
                
                // third number
                int c; str >> c;
                
                // ] =>
                str.get();str.get();str.get();str.get();str.get();
                
                str >> multiplicity;
                
                //cout << a << endl;
                //cout << b << endl;
                //cout << c << endl;
                
                hashed_value = a*b+c;
                //cout << hash(a,b,c) << endl;
                //hashes.push_back(hash(a,b,c));
                
                row1 = dsl::int_to_digits< ull, std::vector<IntType> >(a);
                row2 = dsl::int_to_digits< ull, std::vector<IntType> >(b);
                row3 = dsl::int_to_digits< ull, std::vector<IntType> >(c);
                
                
                
            }
            //ull hash(ull a, ull b, ull c) { return a*b+c; };
            
            ull get_multiplicity() const { return multiplicity; };
            
        private:
            std::vector<IntType> row1;
            std::vector<IntType> row2;
            std::vector<IntType> row3;
            ull multiplicity;
            ull hashed_value;
            
        };
        
        
    private:
        std::vector< sudoku<IntType>::RejectionLookup > lookup;
        ull max_mult;
        RestrictedPermutationList permutations;
        
    };
    
    template<typename T>
    inline bool operator!=(const typename sudoku<T>::RejectionLookup& lhs,
                           const typename sudoku<T>::RejectionLookup& rhs) { return !(lhs == rhs); }
    
    
    
    template<typename IntType>
    sudoku<IntType>::sudoku() : permutations({0}) {
        setup_table();
    };
    
    
    template<typename IntType>
    sudoku<IntType>::sudoku(bool flag) :
    dsl::Table<9,9,IntType>(std::array<IntType,81>({1,2,3,4,5,6,7,8,9,
        4,5,6,7,8,9,1,2,3,
        7,8,9,1,2,3,4,5,6,
        2,3,4,5,6,7,8,9,1,
        5,6,7,8,9,1,2,3,4,
        8,9,1,2,3,4,5,6,7,
        3,4,5,6,7,8,9,1,2,
        6,7,8,9,1,2,3,4,5,
        9,1,2,3,4,5,6,7,8})),
    permutations({0}) {
        setup_table();
    };
    
    
    template<typename ValueType>
    bool sudoku<ValueType>::IsRowPerm(size_t row) {
        
        /*
         auto start = this->begin_row(row);
         auto stop = this->end_row(row);
         
         std::set<ValueType> st {1,2,3,4,5,6,7,8,9};
         
         while(start != stop)
         st.erase(*start++);
         return st.empty();
         */
        
        
        
        auto start = this->begin_row(row);
        ValueType a,b,c,d,e,f,g,h,i;
        
        a = *start++;
        b = *start++;
        c = *start++;
        d = *start++;
        e = *start++;
        f = *start++;
        g = *start++;
        h = *start++;
        i = *start++;
        
        if( a+b+c+d+e+f+g+h+i == 45 &&
           a*b*c*d*e*f*g*h*i == 362880 &&
           a*a+b*b+c*c+d*d+e*e+f*f+g*g+h*h+i*i == 285)
            return true;
        else
            return false;
        
        
    }
    
    template<typename ValueType>
    bool sudoku<ValueType>::IsColumnPerm(size_t col) {
        
        /*
         auto start = this->begin_column(col);
         auto stop = this->end_column(col);
         
         std::set<ValueType> st {1,2,3,4,5,6,7,8,9};
         
         while(start != stop)
         st.erase(*start++);
         return st.empty();
         */
        
        auto start = this->begin_column(col);
        ValueType a,b,c,d,e,f,g,h,i;
        
        a = *start++;
        b = *start++;
        c = *start++;
        d = *start++;
        e = *start++;
        f = *start++;
        g = *start++;
        h = *start++;
        i = *start++;
        
        if( a+b+c+d+e+f+g+h+i == 45 &&
           a*b*c*d*e*f*g*h*i == 362880 &&
           a*a+b*b+c*c+d*d+e*e+f*f+g*g+h*h+i*i == 285)
            return true;
        else
            return false;
        
        
    }
    
    template<typename ValueType>
    bool sudoku<ValueType>::IsBlockPerm(size_t block) {
        
        auto start = this->get();
        if( block <= 2) {
            
            start += (int)(block*3);
        }
        else if( block <= 5) {
            
            start += 3*9;
            start += (int)((block-3)*3);
        }
        else if( block <= 8) {
            
            start += 6*9;
            start += (int)((block-6)*3);
        }
        
        ValueType a,b,c,d,e,f,g,h,i;
        
        a = *start++;
        b = *start++;
        c = *start++;
        start += 6;
        d = *start++;
        e = *start++;
        f = *start++;
        start += 6;
        g = *start++;
        h = *start++;
        i = *start++;
        
        if( a+b+c+d+e+f+g+h+i == 45 &&
           a*b*c*d*e*f*g*h*i == 362880 &&
           a*a+b*b+c*c+d*d+e*e+f*f+g*g+h*h+i*i == 285)
            return true;
        else
            return false;
        
        /*
         std::set<ValueType> st {1,2,3,4,5,6,7,8,9};
         
         
         auto printer = start;
         cout << *(printer++) << ","<<*printer++ << ","<<*printer++ << endl;
         printer+=6;
         cout << *printer++ << ","<<*printer++ << ","<<*printer++ << endl;
         printer+=6;
         cout << *printer++ << ","<<*printer++ << ","<<*printer++ << endl;
         
         
         st.erase( *start++ );    st.erase( *start++ );    st.erase( *start++ );
         start += 6;
         st.erase( *start++ );    st.erase( *start++ );    st.erase( *start++ );
         start += 6;
         st.erase( *start++ );    st.erase( *start++ );    st.erase( *start++ );
         start += 6;
         */
        /*
         if(st.size())
         return false;
         else
         return true;
         */
        
    }
    
    
    template<typename IntType>
    template<typename Iterator>
    void sudoku<IntType>::insert_block(Iterator it, size_t block) {
        
        auto start = this->begin();
        if( block <= 2) {
            
            start += (int)(block*3);
        }
        else if( block <= 5) {
            
            start += 3*9;
            start += (int)((block-3)*3);
        }
        else if( block <= 8) {
            
            start += 6*9;
            start += (int)((block-6)*3);
        }
        
        *start++ = *it++;  *start++ = *it++;  *start++ = *it++;
        start += 6;
        *start++ = *it++;  *start++ = *it++;  *start++ = *it++;
        start += 6;
        *start++ = *it++;  *start++ = *it++;  *start++ = *it++;
        
    };
    
    
    template<typename IntType>
    void sudoku<IntType>::reduce() {
        
        auto start = this->begin();
        start += 3;
        
        for(size_t i=0;i<6;i++) {
            
        }
    }
    
    template<typename IntType>
    void sudoku<IntType>::setup_table() {
        // Users ▸ stephendesalvo ▸ Documents ▸ Research ▸ C++ Code ▸ SudokuSampling
        dsl::file<dsl::file_type::input> rejections("/Users/stephendesalvo/Documents/Research/C++ Code/SudokuSampling/SudokuSampling/results2long");
        
        //std::vector< sudoku<IntType>::RejectionLookup > lookup(36288);
        lookup.resize(36288);
        
        
        std::string line;
        size_t i = 0;
        
        max_mult = 0; // = start->get_multiplicity();
        //ull min_mult = INT_MAX;
        
        while(getline(rejections, line) ) {
            //        lookup.push_back( ds::sudoku<short>::RejectionLookup(line) );
            lookup[i] = ( sudoku<IntType>::RejectionLookup(line) );
            max_mult = std::max( max_mult, lookup[i].get_multiplicity());
            //min_mult = std::min( min_mult, lookup[i].get_multiplicity());
            //cout << lookup[i].row1 << endl;
            //cout << lookup[i].row2 << endl;
            //cout << lookup[i].row3 << endl;
            
            ++i;
        }
        
        /*
         ull total = 0;
         for(size_t i=0;i<36288; ++i)
         total += lookup[i].get_multiplicity();
         std::cout << "cumulative: "<< total << std::endl;
         */
    }
    
    template<typename IntType>
    template<typename URNG>
    void sudoku<IntType>::sample_first_three_rows(URNG& gen) {
        
        /*
         // Prepare the rejection probabilities for blocks 2 and 3.
         
         // Users ▸ stephendesalvo ▸ Documents ▸ Research ▸ C++ Code ▸ SudokuSampling
         dsl::File<dsl::FileType::Input> rejections("/Users/stephendesalvo/Documents/Research/C++ Code/SudokuSampling/SudokuSampling/results2long");
         
         std::vector< sudoku<IntType>::RejectionLookup > lookup(36288);
         
         std::string line;
         size_t i = 0;
         
         ull max_mult = 0; // = start->get_multiplicity();
         ull min_mult = INT_MAX;
         
         while(getline(rejections, line) ) {
         //        lookup.push_back( ds::sudoku<short>::RejectionLookup(line) );
         lookup[i] = ( sudoku<IntType>::RejectionLookup(line) );
         max_mult = std::max( max_mult, lookup[i].get_multiplicity());
         min_mult = std::min( min_mult, lookup[i].get_multiplicity());
         //cout << lookup[i].row1 << endl;
         //cout << lookup[i].row2 << endl;
         //cout << lookup[i].row3 << endl;
         
         ++i;
         }
         
         //std::cout << min_mult << ","<<max_mult << std::endl;
         
         */
        
        
        std::uniform_int_distribution<ull> rand_index(0,lookup.size()-1);
        std::uniform_real_distribution<double> unif(0.,1.);
        
        size_t index=0;
        size_t counts = 0;
        bool reject = true;
        
        while(reject) {
            index = rand_index(dsl::generator_32);
            
            double cutoff = (double)(lookup[index].multiplicity) / (double)max_mult;
            
            //cout << cutoff << endl;
            if(unif(dsl::generator_32) < cutoff)
                reject = false;
            ++counts;
        }
        
        //std::cout << counts << std::endl;
        
        //std::cout << sud.as_one_line_matlab_matrix() << std::endl;
        
        
        
        // Initialize B1 to identity permutation
        std::array<IntType,9> id {1,2,3,4,5,6,7,8,9};
        this->insert_block(std::begin(id), 0);
        
        std::array<IntType,9> block1;
        std::array<IntType,9> block2;
        
        std::copy( begin(lookup[index].row1), begin(lookup[index].row1)+3, begin(block1) );
        std::copy( begin(lookup[index].row2), begin(lookup[index].row2)+3, begin(block1)+3 );
        std::copy( begin(lookup[index].row3), begin(lookup[index].row3)+3, begin(block1)+6 );
        
        std::copy( begin(lookup[index].row1)+3, begin(lookup[index].row1)+6, begin(block2) );
        std::copy( begin(lookup[index].row2)+3, begin(lookup[index].row2)+6, begin(block2)+3 );
        std::copy( begin(lookup[index].row3)+3, begin(lookup[index].row3)+6, begin(block2)+6 );
        
        this->insert_block(begin(block1), 1);
        this->insert_block(begin(block2), 2);
        
    }
    
    template<typename IntType>
    void sudoku<IntType>::iterate_through_all_first_three_rows() {
        
        dsl::file<dsl::file_type::output> sudoku_file("/Users/stephendesalvo/Documents/Research/C++ Code/SudokuSampling/SudokuSampling/P_9s_32688_to_36288.txt");
        
        std::vector<size_t> sizes(36288);
        
        
        //        std::uniform_int_distribution<ull> rand_index(0,lookup.size()-1);
        //        std::uniform_real_distribution<double> unif(0.,1.);
        
        //        for(size_t index = 8700; index < 32688; ++index) {
        for(size_t index = 32686; index < 36288; ++index) {
            
            // Initialize B1 to identity permutation
            std::array<IntType,9> id {1,2,3,4,5,6,7,8,9};
            this->insert_block(std::begin(id), 0);
            
            std::array<IntType,9> block1;
            std::array<IntType,9> block2;
            
            std::copy( begin(lookup[index].row1), begin(lookup[index].row1)+3, begin(block1) );
            std::copy( begin(lookup[index].row2), begin(lookup[index].row2)+3, begin(block1)+3 );
            std::copy( begin(lookup[index].row3), begin(lookup[index].row3)+3, begin(block1)+6 );
            
            std::copy( begin(lookup[index].row1)+3, begin(lookup[index].row1)+6, begin(block2) );
            std::copy( begin(lookup[index].row2)+3, begin(lookup[index].row2)+6, begin(block2)+3 );
            std::copy( begin(lookup[index].row3)+3, begin(lookup[index].row3)+6, begin(block2)+6 );
            
            this->insert_block(begin(block1), 1);
            this->insert_block(begin(block2), 2);
            
            initialize_restricted_permutations();
            
            sizes[index] = this->permutations.size();
            std::cout << sizes[index] << std::endl;
            sudoku_file << sizes[index] << std::endl;
            
        }
        
        std::cout << sizes << std::endl;
        
        std::cout << (*std::max_element(std::begin(sizes), std::end(sizes))) << std::endl;
        
        sudoku_file << sizes << std::endl;
        
    }
    
    
    template<typename IntType>
    void sudoku<IntType>::iterate_through_all_first_three_rows_smaller_set() {
        
        dsl::file<dsl::file_type::input> rows("/Users/stephendesalvo/Documents/Research/C++ Code/SudokuSampling/SudokuSampling/Smaller_Rejection_Table2.txt");
        
        dsl::file<dsl::file_type::output> sudoku_file("/Users/stephendesalvo/Documents/Research/C++ Code/SudokuSampling/SudokuSampling/permutation_completions.txt");
        
        std::vector<size_t> sizes(71);
        
        
        std::string line;
        
        lookup.resize(71);
        
        
        size_t i = 0;
        
        max_mult = 0; // = start->get_multiplicity();
        //ull min_mult = INT_MAX;
        
        while(getline(rows, line) ) {
            //        lookup.push_back( ds::sudoku<short>::RejectionLookup(line) );
            lookup[i] = ( sudoku<IntType>::RejectionLookup(line) );
            max_mult = std::max( max_mult, lookup[i].get_multiplicity());
            //min_mult = std::min( min_mult, lookup[i].get_multiplicity());
            //cout << lookup[i].row1 << endl;
            //cout << lookup[i].row2 << endl;
            //cout << lookup[i].row3 << endl;
            
            ++i;
        }
        
        
        
        //        std::uniform_int_distribution<ull> rand_index(0,lookup.size()-1);
        //        std::uniform_real_distribution<double> unif(0.,1.);
        
        for(size_t index = 0; index < 71; ++index) {
            
            // Initialize B1 to identity permutation
            std::array<IntType,9> id {1,2,3,4,5,6,7,8,9};
            this->insert_block(std::begin(id), 0);
            
            std::array<IntType,9> block1;
            std::array<IntType,9> block2;
            
            std::copy( begin(lookup[index].row1), begin(lookup[index].row1)+3, begin(block1) );
            std::copy( begin(lookup[index].row2), begin(lookup[index].row2)+3, begin(block1)+3 );
            std::copy( begin(lookup[index].row3), begin(lookup[index].row3)+3, begin(block1)+6 );
            
            std::copy( begin(lookup[index].row1)+3, begin(lookup[index].row1)+6, begin(block2) );
            std::copy( begin(lookup[index].row2)+3, begin(lookup[index].row2)+6, begin(block2)+3 );
            std::copy( begin(lookup[index].row3)+3, begin(lookup[index].row3)+6, begin(block2)+6 );
            
            this->insert_block(begin(block1), 1);
            this->insert_block(begin(block2), 2);
            
            initialize_restricted_permutations();
            
            sizes[index] = this->permutations.size();
            std::cout << sizes[index] << std::endl;
            sudoku_file << sizes[index] << std::endl;
            
        }
        
        std::cout << sizes << std::endl;
        
        std::cout << (*std::max_element(std::begin(sizes), std::end(sizes))) << std::endl;
        
        sudoku_file << sizes << std::endl;
        
    }
    
    
    template<typename IntType>
    template<typename URNG>
    void sudoku<IntType>::sample_remaining_rows(URNG& gen) {
        
        size_t N2 = permutations.size();
        std::uniform_int_distribution<size_t> unif2(0,N2-1);
        
        size_t row_2_index;//, row_3_index;
        auto it3 = this->begin_row(3);
        
        bool flag = true;
        size_t counts = 0;
        
        while(flag) {
            do {
                
                it3 = this->begin_row(3);
                row_2_index =unif2(gen);
                this->insert( permutations[row_2_index] , it3);
                
                it3 = this->begin_row(4);
                row_2_index = unif2(gen);
                this->insert(permutations[row_2_index], it3);
                
                it3 = this->begin_row(5);
                row_2_index = unif2(gen);
                this->insert(permutations[row_2_index], it3);
                
                
                //it3 = sud.begin_row(7);
                //row_2_index = unif2(dsl::generator_64);
                //sud.insert(block3[row_2_index], it3);
                
                //it3 = sud.begin_row(8);
                //row_2_index = unif2(dsl::generator_64);
                //sud.insert(block3[row_2_index], it3);
                
                
                //if( sud.last_row_is_completable() ) {
                //    sud.complete_last_row();
                //}
                //else
                //cout << "Not completable" << endl;
                
                
                ++counts;
                if(counts % 1000000 == 0)
                    std::cout << counts << std::endl;
                
                
            }while( !this->IsBlockPerm(3) || !this->IsBlockPerm(4) || !this->IsBlockPerm(5) );
            //        !sud.IsBlockPerm(6) || !sud.IsBlockPerm(7) || !sud.IsBlockPerm(8) );
            
            
            
            it3 = this->begin_row(6);
            row_2_index =unif2(gen);
            this->insert( permutations[row_2_index] , it3);
            
            if(last_two_rows_are_completed() )
                flag = false;
            
            //if(last_three_rows_are_completed())
            //    flag = false;
        }
        //} while(flag);
        rejection_count.add_point(counts);
        rejection_count.write_to_file(SudokuFolder+"rejection_counts.txt");
    }
    
    template<typename IntType>
    bool sudoku<IntType>::last_row_is_completable() {
        std::set<IntType> full {1,2,3,4,5,6,7,8,9};
        std::vector<IntType> last_row(9);
        size_t index = 0;
        
        auto col_it = this->begin_column(0);
        auto stop = this->end_column(0);
        
        auto flag = true;
        
        while(flag && index <= 8) {
            
            col_it = this->begin_column(index);
            stop = this->end_column(index);
            
            full = {1,2,3,4,5,6,7,8,9};
            
            while(col_it != stop) {
                full.erase(*col_it++);
            }
            if(full.size() == 1) {
                last_row[index++] = *(full.begin());
            }
            else
                flag=false;
        }
        
        if(flag) {
            full = {1,2,3,4,5,6,7,8,9};
            
            auto it = last_row.begin();
            auto row_stop = last_row.end();
            
            while(it != row_stop) {
                full.erase(*it++);
            }
            
            if(!full.empty())
                flag = false;
            
            
        }
        
        return flag;
        
    }
    
    
    template<typename IntType>
    void sudoku<IntType>::complete_last_row() {
        
        std::set<IntType> full {1,2,3,4,5,6,7,8,9};
        
        auto row_it = this->begin_row(8);
        auto col_it = this->begin_column(0);
        auto stop = this->end_column(0);
        
        for(size_t i=0;i<9;++i) {
            
            full = {1,2,3,4,5,6,7,8,9};
            col_it = this->begin_column(i);
            stop   = this->end_column(i);
            
            while(col_it != stop) {
                full.erase(*col_it++);
            }
            *row_it++ = *(full.begin());
        }
        
    }
    
    
    template<typename IntType>
    std::vector< std::pair< std::vector<IntType>, std::vector<IntType> > > sudoku<IntType>::set_of_possible_last_two_rows() {
        
        std::set<IntType> full {1,2,3,4,5,6,7,8,9};
        std::vector<IntType> last_row(9);
        std::vector<IntType> penultimate_row(9);
        size_t index = 0;
        
        //dsl::DataPoints<size_t> d_points;
        
        std::vector< std::pair<IntType,IntType> > constraints(9);
        
        auto col_it = this->begin_column(0);
        auto stop = this->end_column(0);
        
        bool flag = true;
        
        while(flag && index <= 8) {
            
            col_it = this->begin_column(index);
            stop = this->end_column(index)-2;
            
            full = {1,2,3,4,5,6,7,8,9};
            
            // Erase elements already in the same column
            while(col_it != stop)
                full.erase(*col_it++);
            
            if(full.size() == 2) {
                auto it = full.begin();
                constraints[index].first = *it++;
                constraints[index].second = *it;
                ++index;
            }
            else
                flag=false;
        }
        
        // All columns have two completable entries.
        if(flag) {
            
            //std::cout << "columns are a go, checking blocks ... "<<std::endl;
            //std::cout << *this << std::endl;
            //std::cout << constraints << std::endl;
            
            size_t initial = 0;
            while(flag && initial <= 7) {
                full = {1,2,3,4,5,6,7,8,9};
                
                // now we check blocks to make sure possible entries don't violate blocks
                
                // block 7
                
                auto it = this->begin_row(6) + initial;
                
                // Erase first three elements of row 7
                full.erase(*it++);
                full.erase(*it++);
                full.erase(*it++);
                
                // Erase each of the first three pairs
                full.erase(constraints[initial].first);  full.erase(constraints[initial].second);
                full.erase(constraints[initial+1].first);  full.erase(constraints[initial+1].second);
                full.erase(constraints[initial+2].first);  full.erase(constraints[initial+2].second);
                
                
                if(!full.empty())
                    flag = false;
                
                initial+= 3;
            }
            
        }
        
        //size_t number_completable;
        bool penultimate_flag = false;
        bool last_flag = false;
        
        std::vector< std::pair< std::vector<IntType>, std::vector<IntType> > > completable_rows;
        // Both column and blocks are fine, now check rows
        if(flag) {
            
            //std::cout << "Blocks are a go ... checking rows. "<<std::endl;
            
            IntType nine = 9;
            size_t bit = 1;
            
            // All 2^9 possible ways of filling in the pairs of entries.
            for(size_t i=0;i<512;++i) {
                
                
                bit = 1;
                // For each combination, fill in the rows ...
                for(size_t j=0;j<9;++j) {
                    
                    penultimate_row[j] = i&bit ? constraints[j].first : constraints[j].second;
                    last_row[j] = i&bit ? constraints[j].second : constraints[j].first;
                    bit*=2;
                    
                }
                
                // Now check if each is a permutation ...
                penultimate_flag = dsl::is_permutation_of_n(begin(penultimate_row), end(penultimate_row), nine);
                
                last_flag = dsl::is_permutation_of_n(begin(last_row), end(last_row), nine);
                
                // and if both are permutations of {1,2..,9}, then add this to the list of possible completions.
                if(penultimate_flag && last_flag) {
                    completable_rows.push_back( std::make_pair( penultimate_row, last_row) );
                }
                
                
                
            }
            
        }
        
        
        return completable_rows;
        
        /*
         if(completable_rows.size()) {
         std::cout << "Completable! There are " << completable_rows.size() << " different ways"<<std::endl;
         d_points.add_point(completable_rows.size());
         d_points.write_to_file(SudokuFolder+"d.txt");
         }
         
         
         
         std::uniform_real_distribution<double> final_rejection(0.,1.);
         
         // Right now, 16 is a provable (but naive) upper bound.  It is most probably 4, but have no proof at this time.
         if(final_rejection(dsl::generator_32) < (double)(completable_rows.size())/16.) {
         
         std::cout << "Made it through rejection! " << std::endl;
         
         flag = true;
         
         // Generate random set of completable rows, uniform over those possible.
         std::uniform_int_distribution<size_t> unif(0,completable_rows.size()-1);
         auto r_index = unif(dsl::generator_32);
         
         auto rit = this->begin_row(7);
         auto it = begin(completable_rows[ r_index ].first);
         
         // Fill in penultimate row
         for(size_t i=0;i<9;++i)
         *rit++ = *it++;
         
         
         rit = this->begin_row(8);
         it = begin(completable_rows[ r_index ].second);
         
         // Fill in final row
         for(size_t i=0;i<9;++i)
         *rit++ = *it++;
         
         }
         else
         flag = false;
         */
        
        //return flag;
        
    }
    
    
    
    
    
    template<typename IntType>
    bool sudoku<IntType>::last_two_rows_are_completed() {
        
        std::set<IntType> full {1,2,3,4,5,6,7,8,9};
        std::vector<IntType> last_row(9);
        std::vector<IntType> penultimate_row(9);
        size_t index = 0;
        
        //dsl::DataPoints<size_t> d_points;
        
        std::vector< std::pair<IntType,IntType> > constraints(9);
        
        auto col_it = this->begin_column(0);
        auto stop = this->end_column(0);
        
        bool flag = true;
        
        while(flag && index <= 8) {
            
            col_it = this->begin_column(index);
            stop = this->end_column(index)-2;
            
            full = {1,2,3,4,5,6,7,8,9};
            
            // Erase elements already in the same column
            while(col_it != stop)
                full.erase(*col_it++);
            
            if(full.size() == 2) {
                auto it = full.begin();
                constraints[index].first = *it++;
                constraints[index].second = *it;
                ++index;
            }
            else
                flag=false;
        }
        
        // All columns have two completable entries.
        if(flag) {
            
            //std::cout << "columns are a go, checking blocks ... "<<std::endl;
            //std::cout << *this << std::endl;
            //std::cout << constraints << std::endl;
            
            size_t initial = 0;
            while(flag && initial <= 7) {
                full = {1,2,3,4,5,6,7,8,9};
                
                // now we check blocks to make sure possible entries don't violate blocks
                
                // block 7
                
                auto it = this->begin_row(6) + initial;
                
                // Erase first three elements of row 7
                full.erase(*it++);
                full.erase(*it++);
                full.erase(*it++);
                
                // Erase each of the first three pairs
                full.erase(constraints[initial].first);  full.erase(constraints[initial].second);
                full.erase(constraints[initial+1].first);  full.erase(constraints[initial+1].second);
                full.erase(constraints[initial+2].first);  full.erase(constraints[initial+2].second);
                
                
                if(!full.empty())
                    flag = false;
                
                initial+= 3;
            }
            
        }
        
        //size_t number_completable;
        bool penultimate_flag = false;
        bool last_flag = false;
        
        std::vector< std::pair< std::vector<IntType>, std::vector<IntType> > > completable_rows;
        // Both column and blocks are fine, now check rows
        if(flag) {
            
            std::cout << "Blocks are a go ... checking rows. "<<std::endl;
            
            IntType nine = 9;
            size_t bit = 1;
            
            // All 2^9 possible ways of filling in the pairs of entries.
            for(size_t i=0;i<512;++i) {
                
                
                bit = 1;
                // For each combination, fill in the rows ...
                for(size_t j=0;j<9;++j) {
                    
                    penultimate_row[j] = i&bit ? constraints[j].first : constraints[j].second;
                    last_row[j] = i&bit ? constraints[j].second : constraints[j].first;
                    bit*=2;
                    
                }
                
                // Now check if each is a permutation ...
                penultimate_flag = dsl::is_permutation_of_n(begin(penultimate_row), end(penultimate_row), nine);
                
                last_flag = dsl::is_permutation_of_n(begin(last_row), end(last_row), nine);
                
                // and if both are permutations of {1,2..,9}, then add this to the list of possible completions.
                if(penultimate_flag && last_flag) {
                    completable_rows.push_back( std::make_pair( penultimate_row, last_row) );
                }
                
                
                
            }
            
        }
        
        if(completable_rows.size()) {
            std::cout << "Completable! There are " << completable_rows.size() << " different ways"<<std::endl;
            d_points.add_point(completable_rows.size());
            d_points.write_to_file(SudokuFolder+"d.txt");
        }
        
        
        
        std::uniform_real_distribution<double> final_rejection(0.,1.);
        
        // Right now, 16 is a provable (but naive) upper bound.  It is most probably 4, but have no proof at this time.
        if(final_rejection(dsl::generator_32) < (double)(completable_rows.size())/16.) {
            
            std::cout << "Made it through rejection! " << std::endl;
            
            flag = true;
            
            // Generate random set of completable rows, uniform over those possible.
            std::uniform_int_distribution<size_t> unif(0,completable_rows.size()-1);
            auto r_index = unif(dsl::generator_32);
            
            auto rit = this->begin_row(7);
            auto it = begin(completable_rows[ r_index ].first);
            
            // Fill in penultimate row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
            
            rit = this->begin_row(8);
            it = begin(completable_rows[ r_index ].second);
            
            // Fill in final row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
        }
        else
            flag = false;
        
        
        return flag;
        
    }
    
    template<typename IntType>
    std::string sudoku<IntType>::as_one_line_matlab_matrix() {
        
        std::string line = "[";
        
        for(size_t i=0;i<8;++i) {
            auto it = this->begin_row(i);
            auto stop = this->end_row(i)-1;
            
            while(it != stop) {
                line += std::to_string( *it++ );
                line += ",";
            }
            line += std::to_string(*it);
            line += ";";
        }
        auto it = this->begin_row(8);
        auto stop = this->end_row(8)-1;
        
        while(it != stop) {
            line += std::to_string( *it++ );
            line += ",";
        }
        line += std::to_string(*it);
        line += "];";
        return line;
        
    }
    
    
    
    
    template<typename IntType>
    void sudoku<IntType>::initialize_restricted_permutations() {
        
        permutations.clear();
        
        permutations.insert( {1,this->operator()(0,0)});
        permutations.insert( {1,this->operator()(1,0)});
        permutations.insert( {1,this->operator()(2,0)});
        
        permutations.insert( {2,this->operator()(0,1)});
        permutations.insert( {2,this->operator()(1,1)});
        permutations.insert( {2,this->operator()(2,1)});
        
        permutations.insert( {3,this->operator()(0,2)});
        permutations.insert( {3,this->operator()(1,2)});
        permutations.insert( {3,this->operator()(2,2)});
        
        permutations.insert( {4,this->operator()(0,3)});
        permutations.insert( {4,this->operator()(1,3)});
        permutations.insert( {4,this->operator()(2,3)});
        
        permutations.insert( {5,this->operator()(0,4)});
        permutations.insert( {5,this->operator()(1,4)});
        permutations.insert( {5,this->operator()(2,4)});
        
        permutations.insert( {6,this->operator()(0,5)});
        permutations.insert( {6,this->operator()(1,5)});
        permutations.insert( {6,this->operator()(2,5)});
        
        permutations.insert( {7,this->operator()(0,6)});
        permutations.insert( {7,this->operator()(1,6)});
        permutations.insert( {7,this->operator()(2,6)});
        
        permutations.insert( {8,this->operator()(0,7)});
        permutations.insert( {8,this->operator()(1,7)});
        permutations.insert( {8,this->operator()(2,7)});
        
        permutations.insert( {9,this->operator()(0,8)});
        permutations.insert( {9,this->operator()(1,8)});
        permutations.insert( {9,this->operator()(2,8)});
        
        permutations.resize(9);
        
        //permutations.initialize_permutations_memory_for_time(9);
        
    }
    
    template<typename IntType>
    bool sudoku<IntType>::blocks_four_five_six() {
        
        IntType* three_perms = new IntType[27]();
        
        IntType index = 1;
        for_each(three_perms, three_perms+9, [&](IntType& a) { a = index++; });
        index %= 9;
        for_each(three_perms+9, three_perms+18, [&](IntType& a) { a = index++; });
        index %= 9;
        for_each(three_perms+18, three_perms+27, [&](IntType& a) { a = index++; });
        
        auto it = this->begin_row(3);
        
        
        
    }
    
    template<typename IntType>
    bool sudoku<IntType>::is_sudoku() {
        if( this->IsRowPerm(0)    && this->IsRowPerm(1)    && this->IsRowPerm(2)    &&
           this->IsRowPerm(3)    && this->IsRowPerm(4)    && this->IsRowPerm(5)    &&
           this->IsRowPerm(6)    && this->IsRowPerm(7)    && this->IsRowPerm(8)    &&
           this->IsColumnPerm(0) && this->IsColumnPerm(1) && this->IsColumnPerm(2) &&
           this->IsColumnPerm(3) && this->IsColumnPerm(4) && this->IsColumnPerm(5) &&
           this->IsColumnPerm(6) && this->IsColumnPerm(7) && this->IsColumnPerm(8) &&
           this->IsBlockPerm(0)  && this->IsBlockPerm(1)  && this->IsBlockPerm(2)  &&
           this->IsBlockPerm(3)  && this->IsBlockPerm(4)  && this->IsBlockPerm(5)  &&
           this->IsBlockPerm(6)  && this->IsBlockPerm(7)  && this->IsBlockPerm(8)
           )
            return true;
        else
            return false;
    }
    
    
    
    template<typename IntType>
    bool sudoku<IntType>::last_three_rows_are_completed() {
        
        
        std::vector<std::set<IntType>> full(9, {1,2,3,4,5,6,7,8,9});
        std::vector<IntType> last_row(9);
        std::vector<IntType> penultimate_row(9);
        std::vector<IntType> third_row(9);
        size_t index = 0;
        
        std::vector< std::vector<IntType> > constraints(9);
        
        
        auto col_it = this->begin_column(0);
        auto stop = this->end_column(0);
        
        bool flag = true;
        
        while(flag && index <= 8) {
            
            col_it = this->begin_column(index);
            stop = this->end_column(index)-3;
            
            // Erase elements already in the same column
            while(col_it != stop)
                full[index].erase(*col_it++);
            
            if(full[index].size() == 3) {
                auto it = full[index].begin();
                IntType first = *it++;
                IntType second = *it++;
                IntType third = *it;
                constraints[index] = {first, second, third};
                ++index;
            }
            else
                flag=false;
        }
        
        // All columns have three completable entries.
        if(flag) {
            
            std::cout << "columns are a go, checking blocks ... "<<std::endl;
            //std::cout << *this << std::endl;
            //std::cout << constraints << std::endl;
            
            size_t initial = 0;
            while(flag && initial <= 7) {
                std::set<IntType> full_local = {1,2,3,4,5,6,7,8,9};
                
                // now we check blocks to make sure possible entries don't violate blocks
                
                // Erase each of the first three pairs
                
                full_local.erase(constraints[initial][0]);
                full_local.erase(constraints[initial][1]);
                full_local.erase(constraints[initial][2]);
                
                full_local.erase(constraints[initial+1][0]);
                full_local.erase(constraints[initial+1][1]);
                full_local.erase(constraints[initial+1][2]);
                
                full_local.erase(constraints[initial+2][0]);
                full_local.erase(constraints[initial+2][1]);
                full_local.erase(constraints[initial+2][2]);
                
                if(!full_local.empty())
                    flag = false;
                
                initial+= 3;
            }
            
        }
        
        //size_t number_completable;
        bool third_flag = false;
        bool penultimate_flag = false;
        bool last_flag = false;
        
        std::vector< std::vector< std::vector<IntType> > > completable_rows;
        
        // Both column and blocks are fine, now check rows
        if(flag) {
            
            std::cout << "Blocks are a go ... checking rows. "<<std::endl;
            
            std::vector< std::vector< IntType > > all_perms_of_rows(9, std::vector<IntType>(18));
            
            for(size_t j=0;j<9;j++) {
                
                size_t index= 0;
                all_perms_of_rows[j][index++] = constraints[j][0];
                all_perms_of_rows[j][index++] = constraints[j][1];
                all_perms_of_rows[j][index++] = constraints[j][2];
                
                while(std::next_permutation( begin(constraints[j]), end(constraints[j]))) {
                    all_perms_of_rows[j][index++] = constraints[j][0];
                    all_perms_of_rows[j][index++] = constraints[j][1];
                    all_perms_of_rows[j][index++] = constraints[j][2];
                }
            }
            
            //std::cout << all_perms_of_rows << std::endl;
            
            std::vector<IntType> partial_row(6);
            
            
            for(size_t i1=0;i1<6;i1++) {
                std::cout << i1 << std::endl;
                for(size_t i2=0;i2<6;i2++)
                    for(size_t i3=0;i3<6;i3++)
                        for(size_t i4=0;i4<6;i4++) {
                            
                            partial_row[0] = all_perms_of_rows[0][i1*3];
                            partial_row[1] = all_perms_of_rows[0][i1*3+1];
                            partial_row[2] = all_perms_of_rows[0][i1*3+2];
                            partial_row[3] = all_perms_of_rows[3][i4*3];
                            partial_row[4] = all_perms_of_rows[3][i4*3+1];
                            partial_row[5] = all_perms_of_rows[3][i4*3+2];
                            
                            if( dsl::has_unique_elements(partial_row) ) {
                                
                                for(size_t i5=0;i5<6;i5++) {
                                    
                                    
                                    
                                    partial_row[0] = all_perms_of_rows[1][i2*3];
                                    partial_row[1] = all_perms_of_rows[1][i2*3+1];
                                    partial_row[2] = all_perms_of_rows[1][i2*3+2];
                                    partial_row[3] = all_perms_of_rows[4][i5*3];
                                    partial_row[4] = all_perms_of_rows[4][i5*3+1];
                                    partial_row[5] = all_perms_of_rows[4][i5*3+2];
                                    
                                    if( dsl::has_unique_elements(partial_row) ) {
                                        
                                        for(size_t i6=0;i6<6;i6++) {
                                            
                                            
                                            
                                            
                                            partial_row[0] = all_perms_of_rows[2][i3*3];
                                            partial_row[1] = all_perms_of_rows[2][i3*3+1];
                                            partial_row[2] = all_perms_of_rows[2][i3*3+2];
                                            partial_row[3] = all_perms_of_rows[5][i6*3];
                                            partial_row[4] = all_perms_of_rows[5][i6*3+1];
                                            partial_row[5] = all_perms_of_rows[5][i6*3+2];
                                            
                                            if(dsl::has_unique_elements(partial_row)) {
                                                
                                                for(size_t i7=0;i7<6;i7++)
                                                    for(size_t i8=0;i8<6;i8++)
                                                        for(size_t i9=0;i9<6;i9++) {
                                                            
                                                            third_row[0] = all_perms_of_rows[0][i1*3];
                                                            third_row[1] = all_perms_of_rows[0][i1*3+1];
                                                            third_row[2] = all_perms_of_rows[0][i1*3+2];
                                                            third_row[3] = all_perms_of_rows[3][i4*3];
                                                            third_row[4] = all_perms_of_rows[3][i4*3+1];
                                                            third_row[5] = all_perms_of_rows[3][i4*3+2];
                                                            third_row[6] = all_perms_of_rows[6][i7*3];
                                                            third_row[7] = all_perms_of_rows[6][i7*3+1];
                                                            third_row[8] = all_perms_of_rows[6][i7*3+2];
                                                            
                                                            penultimate_row[0] = all_perms_of_rows[1][i2*3];
                                                            penultimate_row[1] = all_perms_of_rows[1][i2*3+1];
                                                            penultimate_row[2] = all_perms_of_rows[1][i2*3+2];
                                                            penultimate_row[3] = all_perms_of_rows[4][i5*3];
                                                            penultimate_row[4] = all_perms_of_rows[4][i5*3+1];
                                                            penultimate_row[5] = all_perms_of_rows[4][i5*3+2];
                                                            penultimate_row[6] = all_perms_of_rows[7][i8*3];
                                                            penultimate_row[7] = all_perms_of_rows[7][i8*3+1];
                                                            penultimate_row[8] = all_perms_of_rows[7][i8*3+2];
                                                            
                                                            last_row[0] = all_perms_of_rows[2][i3*3];
                                                            last_row[1] = all_perms_of_rows[2][i3*3+1];
                                                            last_row[2] = all_perms_of_rows[2][i3*3+2];
                                                            last_row[3] = all_perms_of_rows[5][i6*3];
                                                            last_row[4] = all_perms_of_rows[5][i6*3+1];
                                                            last_row[5] = all_perms_of_rows[5][i6*3+2];
                                                            last_row[6] = all_perms_of_rows[8][i9*3];
                                                            last_row[7] = all_perms_of_rows[8][i9*3+1];
                                                            last_row[8] = all_perms_of_rows[8][i9*3+2];
                                                            
                                                            
                                                            
                                                            // Now check if each is a permutation ...
                                                            
                                                            IntType nine = 9;
                                                            
                                                            third_flag = dsl::is_permutation_of_n(begin(third_row), end(third_row), nine);
                                                            penultimate_flag = dsl::is_permutation_of_n(begin(penultimate_row), end(penultimate_row), nine);
                                                            last_flag = dsl::is_permutation_of_n(begin(last_row), end(last_row), nine);
                                                            
                                                            // and if both are permutations of {1,2..,9}, then add this to the list of possible completions.
                                                            if(third_flag && penultimate_flag && last_flag) {
                                                                completable_rows.push_back( {third_row, penultimate_row, last_row} );
                                                            }
                                                            
                                                        }
                                            }
                                        }
                                        
                                    }
                                }
                            }
                            
                        }
                
            }
            
            /*
             while(std::next_permutation( begin(constraints[0]), end(constraints[0])))
             while(std::next_permutation( begin(constraints[1]), end(constraints[1])))
             while(std::next_permutation( begin(constraints[2]), end(constraints[2])))
             while(std::next_permutation( begin(constraints[3]), end(constraints[3])))
             while(std::next_permutation( begin(constraints[4]), end(constraints[4])))
             while(std::next_permutation( begin(constraints[5]), end(constraints[5])))
             while(std::next_permutation( begin(constraints[6]), end(constraints[6])))
             while(std::next_permutation( begin(constraints[7]), end(constraints[7])))
             while(std::next_permutation( begin(constraints[8]), end(constraints[8]))) {
             
             for(size_t j=0;j<9;j++) {
             third_row[j] = constraints[j][0];
             penultimate_row[j] = constraints[j][1];
             last_row[j] = constraints[j][2];
             }
             
             
             
             
             // Now check if each is a permutation ...
             
             IntType nine = 9;
             
             third_flag = dsl::is_permutation_of_n(begin(third_row), end(third_row), nine);
             penultimate_flag = dsl::is_permutation_of_n(begin(penultimate_row), end(penultimate_row), nine);
             last_flag = dsl::is_permutation_of_n(begin(last_row), end(last_row), nine);
             
             // and if both are permutations of {1,2..,9}, then add this to the list of possible completions.
             if(third_flag && penultimate_flag && last_flag) {
             completable_rows.push_back( {third_row, penultimate_row, last_row} );
             }
             }
             
             
             */
            
            
        }
        
        if(completable_rows.size()) {
            std::cout << "Completable! There are " << completable_rows.size() << " different ways"<<std::endl;
        }
        std::uniform_real_distribution<double> final_rejection(0.,1.);
        
        // Right now, 16 is a provable (but naive) upper bound.  It is most probably 4, but have no proof at this time.
        if(final_rejection(dsl::generator_32) < (double)(completable_rows.size())/1728.) {
            
            std::cout << "Made it through rejection! " << std::endl;
            
            flag = true;
            
            // Generate random set of completable rows, uniform over those possible.
            std::uniform_int_distribution<size_t> unif(0,completable_rows.size()-1);
            auto r_index = unif(dsl::generator_32);
            
            auto rit = this->begin_row(6);
            auto it = begin(completable_rows[ r_index ][0]);
            
            // Fill in penultimate row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
            rit = this->begin_row(7);
            it = begin(completable_rows[ r_index ][1]);
            
            // Fill in penultimate row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
            
            rit = this->begin_row(8);
            it = begin(completable_rows[ r_index ][2]);
            
            // Fill in final row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
        }
        else
            flag = false;
        
        
        return flag;
        
    }
    
    template<typename IntType>
    std::vector< std::vector< std::vector<IntType> > > sudoku<IntType>::set_of_possible_last_three_rows() {

        return set_of_possible_last_three_rows(6,0);
        
    }
    
    template<typename IntType>
    std::vector<std::vector<std::vector<IntType> > > sudoku<IntType>::set_of_possible_last_three_rows(size_t row, size_t column) {
        
        std::vector< std::vector< std::vector<IntType> > > completable_rows;
        
        //std::cout << *this << std::endl;
        auto x = allowable_part_sizes(row, column);
        ull count = 0;
        
        if(x.size()) {
            
            // If last element in table, and can be completed, done.
            if(row == 8 && column == 8) {
                
                //this->operator()(row,column) = x[0];
                //std::cout << *this << std::endl;
                //std::cout << "Complete!" << std::endl;
                
                std::vector<IntType> row1(9);
                std::vector<IntType> row2(9);
                std::vector<IntType> row3(9);
                
                std::copy( this->begin_row(6), this->end_row(6), std::begin(row1) );
                std::copy( this->begin_row(7), this->end_row(7), std::begin(row2) );
                std::copy( this->begin_row(8), this->end_row(8), std::begin(row3) );
                
                completable_rows.push_back( {row1, row2, row3} );
                
                this->operator()(row, column) = 0;
                return completable_rows;
            }
            auto row_save = row;
            auto col_save = column;
            
            
            // Check if at end of current row.
            // Row is not 8 since we checked for this earlier.
            ++column;
            if(column == 9) {
                column = 0;
                ++row;
            }
            // Loop through all possible values for given entry
            for(auto value : x) {
                
                this->operator()(row_save, col_save) = value;
                
                // check number of partial completions recursively.
                //count += number_partial_completions(row,column);
                
                for(auto x : set_of_possible_last_three_rows(row, column) )
                    completable_rows.push_back( x );
                
            }
            this->operator()(row_save, col_save) = 0;
            
        }
        else {
            this->operator()(row, column) = 0;
            return completable_rows;
        }
        
        //return count;
        return completable_rows;

        
    }
    
    
    template<typename IntType>
    std::vector< std::pair<size_t,size_t> > sudoku<IntType>::indices_of_fellow_block_entries(size_t row, size_t column) {
        
        std::vector< std::pair<size_t,size_t> > indices(9);
        
        // Block 1
        if(row <=2 && column <= 2) {
            indices[0] = std::make_pair(0,0);
            indices[1] = std::make_pair(1,0);
            indices[2] = std::make_pair(2,0);
            indices[3] = std::make_pair(0,1);
            indices[4] = std::make_pair(1,1);
            indices[5] = std::make_pair(2,1);
            indices[6] = std::make_pair(0,2);
            indices[7] = std::make_pair(1,2);
            indices[8] = std::make_pair(2,2);
        }
        
        // Block 4
        if(row >=3 && row <= 5 && column <= 2) {
            indices[0] = std::make_pair(3,0);
            indices[1] = std::make_pair(4,0);
            indices[2] = std::make_pair(5,0);
            indices[3] = std::make_pair(3,1);
            indices[4] = std::make_pair(4,1);
            indices[5] = std::make_pair(5,1);
            indices[6] = std::make_pair(3,2);
            indices[7] = std::make_pair(4,2);
            indices[8] = std::make_pair(5,2);
        }
        
        // Block 7
        if(row >=6 && row <= 8 && column <= 2) {
            indices[0] = std::make_pair(6,0);
            indices[1] = std::make_pair(7,0);
            indices[2] = std::make_pair(8,0);
            indices[3] = std::make_pair(6,1);
            indices[4] = std::make_pair(7,1);
            indices[5] = std::make_pair(8,1);
            indices[6] = std::make_pair(6,2);
            indices[7] = std::make_pair(7,2);
            indices[8] = std::make_pair(8,2);
        }
        
        
        
        // Block 2
        if(row <=2 && column >= 3 && column <= 5) {
            indices[0] = std::make_pair(0,3);
            indices[1] = std::make_pair(1,3);
            indices[2] = std::make_pair(2,3);
            indices[3] = std::make_pair(0,4);
            indices[4] = std::make_pair(1,4);
            indices[5] = std::make_pair(2,4);
            indices[6] = std::make_pair(0,5);
            indices[7] = std::make_pair(1,5);
            indices[8] = std::make_pair(2,5);
        }
        
        // Block 5
        if(row >=3 && row <= 5 && column >= 3 && column <= 5) {
            indices[0] = std::make_pair(3,3);
            indices[1] = std::make_pair(4,3);
            indices[2] = std::make_pair(5,3);
            indices[3] = std::make_pair(3,4);
            indices[4] = std::make_pair(4,4);
            indices[5] = std::make_pair(5,4);
            indices[6] = std::make_pair(3,5);
            indices[7] = std::make_pair(4,5);
            indices[8] = std::make_pair(5,5);
        }
        
        // Block 8
        if(row >=6 && row <= 8 && column >= 3 && column <= 5) {
            indices[0] = std::make_pair(6,3);
            indices[1] = std::make_pair(7,3);
            indices[2] = std::make_pair(8,3);
            indices[3] = std::make_pair(6,4);
            indices[4] = std::make_pair(7,4);
            indices[5] = std::make_pair(8,4);
            indices[6] = std::make_pair(6,5);
            indices[7] = std::make_pair(7,5);
            indices[8] = std::make_pair(8,5);
        }
        
        
        
        // Block 3
        if(row <=2 && column >= 6 && column <= 8) {
            indices[0] = std::make_pair(0,6);
            indices[1] = std::make_pair(1,6);
            indices[2] = std::make_pair(2,6);
            indices[3] = std::make_pair(0,7);
            indices[4] = std::make_pair(1,7);
            indices[5] = std::make_pair(2,7);
            indices[6] = std::make_pair(0,8);
            indices[7] = std::make_pair(1,8);
            indices[8] = std::make_pair(2,8);
        }
        
        // Block 6
        if(row >=3 && row <= 5 && column >= 6 && column <= 8) {
            indices[0] = std::make_pair(3,6);
            indices[1] = std::make_pair(4,6);
            indices[2] = std::make_pair(5,6);
            indices[3] = std::make_pair(3,7);
            indices[4] = std::make_pair(4,7);
            indices[5] = std::make_pair(5,7);
            indices[6] = std::make_pair(3,8);
            indices[7] = std::make_pair(4,8);
            indices[8] = std::make_pair(5,8);
        }
        
        // Block 9
        if(row >=6 && row <= 8 && column >= 6 && column <= 8) {
            indices[0] = std::make_pair(6,6);
            indices[1] = std::make_pair(7,6);
            indices[2] = std::make_pair(8,6);
            indices[3] = std::make_pair(6,7);
            indices[4] = std::make_pair(7,7);
            indices[5] = std::make_pair(8,7);
            indices[6] = std::make_pair(6,8);
            indices[7] = std::make_pair(7,8);
            indices[8] = std::make_pair(8,8);
        }
        
        return indices;
    }
    
    
    template<typename IntType>
    std::set<IntType> sudoku<IntType>::allowable_part_sizes_using_set(size_t row, size_t column) {
        
        std::set<IntType> perm = {1,2,3,4,5,6,7,8,9};
        
        // column constraints
        for(size_t i=0;i<row;++i) {
            perm.erase( this->operator()(i,column) );
        }
        
        // row constraints
        for(size_t j=0;j<column;++j) {
            perm.erase( this->operator()(row,j) );
        }
        
        // block constraints
        auto indices = indices_of_fellow_block_entries(row, column);
        for(auto x : indices)
            perm.erase( this->operator()(x.first, x.second) );
        
        
        return perm;
        
    }
    template<typename IntType>
    dsl::shrinking_unordered_set<IntType> sudoku<IntType>::allowable_part_sizes(size_t row, size_t column) {
        
        dsl::shrinking_unordered_set<IntType> perm = {1,2,3,4,5,6,7,8,9};
        
        // column constraints
        for(size_t i=0;i<row;++i) {
            perm.erase( this->operator()(i,column) );
        }
        
        // row constraints
        for(size_t j=0;j<column;++j) {
            perm.erase( this->operator()(row,j) );
        }
        
        // block constraints
        auto indices = indices_of_fellow_block_entries(row, column);
        for(auto x : indices)
            perm.erase( this->operator()(x.first, x.second) );
        
        
        return perm;
        
    }
    
    
    template<typename IntType>
    ull sudoku<IntType>::number_partial_completions(size_t row, size_t column) {
        
        //std::cout << *this << std::endl;
        auto x = allowable_part_sizes(row, column);
        ull count = 0;
        
        if(x.size()) {
            
            // If last element in table, and can be completed, done.
            if(row == 8 && column == 8) {
                
                //this->operator()(row,column) = x[0];
                //std::cout << *this << std::endl;
                //std::cout << "Complete!" << std::endl;
                
                this->operator()(row, column) = 0;
                return 1;
            }
            auto row_save = row;
            auto col_save = column;
            
            
            // Check if at end of current row.
            // Row is not 8 since we checked for this earlier.
            ++column;
            if(column == 9) {
                column = 0;
                ++row;
            }
            // Loop through all possible values for given entry
            for(auto value : x) {
                
                this->operator()(row_save, col_save) = value;
                
                // check number of partial completions recursively.
                count += number_partial_completions(row,column);
                
                
            }
            this->operator()(row_save, col_save) = 0;
            
        }
        else {
            this->operator()(row, column) = 0;
            return 0;
        }
        
        return count;
        
    }
    
    
    template<typename IntType>
    std::vector<std::vector<std::vector<IntType>>> sudoku<IntType>::set_of_three_row_partial_completions(size_t row, size_t column) {
        
        //std::cout << *this << std::endl;
        auto x = allowable_part_sizes(row, column);
        ull count = 0;
        
        if(x.size()) {
            
            // If last element in table, and can be completed, done.
            if(row == 8 && column == 8) {
                
                this->operator()(row,column) = x[0];
                std::cout << *this << std::endl;
                std::cout << "Complete!" << std::endl;
                
                // Initialize three rows of size 9
                std::vector<std::vector<IntType>> rows(3, std::vector<IntType>(9) );
                std::copy( this->begin_row(6), this->begin_row(6)+9, std::begin(rows[0]));
                
                this->operator()(row, column) = 0;
                return 1;
            }
            auto row_save = row;
            auto col_save = column;
            
            
            // Check if at end of current row.
            // Row is not 8 since we checked for this earlier.
            ++column;
            if(column == 9) {
                column = 0;
                ++row;
            }
            // Loop through all possible values for given entry
            for(auto value : x) {
                
                this->operator()(row_save, col_save) = value;
                
                // check number of partial completions recursively.
                count += number_partial_completions(row,column);
                
                
            }
            this->operator()(row_save, col_save) = 0;
            
        }
        else {
            this->operator()(row, column) = 0;
            return 0;
        }
        
        return count;
        
    }
    
    
    template<typename IntType>
    template<typename URNG>
    typename sudoku<IntType>::object sudoku<IntType>::importance_sample_using_set(URNG& gen) {
        
        
        auto it = this->begin();
        auto itend = this->end();
        
        while(it != itend)
            *it++ = 0;
        
        /*
         // reset values
         for(size_t i2 = 0; i2 < 9; i2++) {
         for(size_t j2 = 0;j2 < 9; j2++)
         this->operator()(i2,j2) = 0;
         }
         */
        
        
        // Sample first three rows unbiased from PDC
        this->sample_first_three_rows(gen);
        
        // all but the first three rows.
        std::vector<double> weights(81-27);
        // Sample remaining rows using importance sampling, NO BACKTRACKING.
        size_t weight_index = 0;
        size_t rejection_counts = 0;
        
        bool flag = true;
        
        while(flag) {
            flag = false;
            weight_index = 0;
            // Loop through rows 3 through 8
            for(size_t i=3;i<9; ++i) {
                
                // Loop through columns 0 through 8
                for(size_t j=0;j<9;++j) {
                    
                    // Calculate R_{i,j}
                    auto x = allowable_part_sizes(i,j);
                    
                    // If not empty, fill in random value from the list
                    if(x.size()) {
                        std::uniform_int_distribution<short> unif(0,x.size()-1);
                        auto it = std::begin(x);
                        std::advance(it, unif(gen));
                        this->operator()(i,j) = *it;
                        weights[weight_index++] = (double)(x.size()) / (9.-j);
                    }
                    else { // if ANY of the conditions are violated, restart from scratch.  NO BACKTRACKING!
                        flag = true;
                        //std::cout << "row "<<i << " col "<<j<<std::endl;
                        
                        // reset values
                        auto it = this->begin();
                        it += 27; // point to first element in third row
                        auto itend = it+(i-2)*9; // point to one after last element in i-th row.
                        
                        while(it != itend)
                            *it++ = 0;
                        
                        /*
                         for(size_t i2 = 3; i2 <= i; i2++) {
                         for(size_t j2 = 0;j2 < 9; j2++)
                         this->operator()(i2,j2) = 0;
                         }
                         */
                        i=9;j=9;
                        
                    }
                }
            }
            ++rejection_counts;
            
            if(rejection_counts % 1000000 == 0)
                std::cout << rejection_counts << std::endl;
        }
        rejection_count.add_point( rejection_counts);
        //std::cout << "total rejections = "<< rejection_counts << std::endl;
        double wt = std::accumulate(std::begin(weights), std::end(weights), 1., std::multiplies<double>());
        
        final_randomization(gen);
        
        
        return sudoku<IntType>::object(*this, wt);
        
        
    }
    
    
    
    template<typename IntType>
    template<typename URNG>
    typename sudoku<IntType>::object sudoku<IntType>::importance_sample_with_first_three_rows(URNG& gen) {
        
        
        auto it = this->begin();
        auto itend = this->end();
        
        while(it != itend)
            *it++ = 0;
        
        /*
         // reset values
         for(size_t i2 = 0; i2 < 9; i2++) {
         for(size_t j2 = 0;j2 < 9; j2++)
         this->operator()(i2,j2) = 0;
         }
         */
        
        
        // Sample first three rows unbiased from PDC
        this->sample_first_three_rows(gen);
        
        // all but the first three rows.
        std::vector<double> weights(81-27);
        // Sample remaining rows using importance sampling, NO BACKTRACKING.
        size_t weight_index = 0;
        size_t rejection_counts = 0;
        
        bool flag = true;
        
        while(flag) {
            flag = false;
            weight_index = 0;
            // Loop through rows 3 through 8
            for(size_t i=3;i<9; ++i) {
                
                // Loop through columns 0 through 8
                for(size_t j=0;j<9;++j) {
                    
                    // Calculate R_{i,j}
                    auto x = allowable_part_sizes(i,j);
                    
                    // If not empty, fill in random value from the list
                    if(x.size()) {
                        std::uniform_int_distribution<short> unif(0,x.size()-1);
                        //auto it = std::begin(x);
                        //std::advance(it, unif(gen));
                        this->operator()(i,j) = x[unif(gen)];
                        weights[weight_index++] = (double)(x.size()) / (9.-j);
                    }
                    else { // if ANY of the conditions are violated, restart from scratch.  NO BACKTRACKING!
                        flag = true;
                        //std::cout << "row "<<i << " col "<<j<<std::endl;
                        
                        // reset values
                        auto it = this->begin();
                        it += 27; // point to first element in third row
                        auto itend = it+(i-2)*9; // point to one after last element in i-th row.
                        
                        while(it != itend)
                            *it++ = 0;
                        
                        /*
                         for(size_t i2 = 3; i2 <= i; i2++) {
                         for(size_t j2 = 0;j2 < 9; j2++)
                         this->operator()(i2,j2) = 0;
                         }
                         */
                        i=9;j=9;
                        
                    }
                }
            }
            ++rejection_counts;
            
            if(rejection_counts % 1000000 == 0)
                std::cout << rejection_counts << std::endl;
        }
        rejection_count.add_point( rejection_counts);
        //std::cout << "total rejections = "<< rejection_counts << std::endl;
        double wt = std::accumulate(std::begin(weights), std::end(weights), 1., std::multiplies<double>());
        
        final_randomization(gen);
        
        
        return sudoku<IntType>::object(*this, wt);
        
        
    }
    
    
    
    
    template<typename IntType>
    template<typename URNG>
    typename sudoku<IntType>::object sudoku<IntType>::importance_sample_no_partial_information(URNG& gen) {
        
        // reset values
        for(size_t i2 = 0; i2 < 9; i2++) {
            for(size_t j2 = 0;j2 < 9; j2++)
                this->operator()(i2,j2) = 0;
        }
        // Sample first three rows unbiased from PDC
        //this->sample_first_three_rows(gen);
        
        // all but the first three rows.
        std::vector<double> weights(81);
        // Sample remaining rows using importance sampling, NO BACKTRACKING.
        size_t weight_index = 0;
        size_t rejection_counts = 0;
        
        bool flag = true;
        
        while(flag) {
            flag = false;
            weight_index = 0;
            // Loop through rows 0 through 8
            for(size_t i=0;i<9; ++i) {
                
                // Loop through columns 0 through 8
                for(size_t j=0;j<9;++j) {
                    
                    // Calculate R_{i,j}
                    auto x = allowable_part_sizes(i,j);
                    
                    // If not empty, fill in random value from the list
                    if(x.size()) {
                        std::uniform_int_distribution<short> unif(0,x.size()-1);
                        auto it = std::begin(x);
                        std::advance(it, unif(gen));
                        
                        this->operator()(i,j) = *it;
                        weights[weight_index++] = x.size() / (9.-j);
                    }
                    else { // if ANY of the conditions are violated, restart from scratch.  NO BACKTRACKING!
                        flag = true;
                        //std::cout << "row "<<i << " col "<<j<<std::endl;
                        
                        //std::cout << *this << std::endl;
                        // reset values
                        for(size_t i2 = 0; i2 <= i; i2++) {
                            for(size_t j2 = 0;j2 < 9; j2++)
                                this->operator()(i2,j2) = 0;
                        }
                        //std::cout << *this << std::endl;
                        i=9;j=9;
                        
                    }
                }
            }
            ++rejection_counts;
            
            if(rejection_counts % 1000000 == 0)
                std::cout << rejection_counts << std::endl;
        }
        rejection_count2.add_point(rejection_counts);
        //std::cout << "total rejections = "<< rejection_counts << std::endl;
        double wt = std::accumulate(std::begin(weights), std::end(weights), 1., std::multiplies<double>());
        
        final_randomization(gen);
        return sudoku<IntType>::object(*this, wt);
        
        
    }
    
    
    
    
    /** importance sampler using PDC for first three rows and last two rows.
     @param gen is the random number generator
     @returns a random Sudoku matrix, which contains the table of values and the importance sampling weight.
     */
    template<typename IntType>
    template<typename URNG>
    typename sudoku<IntType>::object sudoku<IntType>::importance_sample_first_three_last_two(URNG& gen) {
        
        
        auto it = this->begin();
        auto itend = this->end();
        
        while(it != itend)
            *it++ = 0;
        
        /*
         // reset values
         for(size_t i2 = 0; i2 < 9; i2++) {
         for(size_t j2 = 0;j2 < 9; j2++)
         this->operator()(i2,j2) = 0;
         }
         */
        
        
        // Sample first three rows unbiased from PDC
        this->sample_first_three_rows(gen);
        
        // all but the first three rows and last two rows.
        std::vector<double> weights(81-27-18);
        
        double wt = 0.;
        
        // Sample rows 4--7 using importance sampling, NO BACKTRACKING.
        size_t weight_index = 0;
        size_t rejection_counts = 0;
        
        bool flag = true;
        
        while(flag) {
            flag = false;
            weight_index = 0;
            // Loop through rows 4 through 7
            for(size_t i=3;i<7; ++i) {
                
                // Loop through columns 0 through 8
                for(size_t j=0;j<9;++j) {
                    
                    // Calculate R_{i,j}
                    auto x = allowable_part_sizes(i,j);
                    
                    // If not empty, fill in random value from the list
                    if(x.size()) {
                        std::uniform_int_distribution<short> unif(0,x.size()-1);
                        //auto it = std::begin(x);
                        //std::advance(it, unif(gen));
                        this->operator()(i,j) = x[unif(gen)];
                        weights[weight_index++] = (double)(x.size()) / (9.-j);
                    }
                    else { // if ANY of the conditions are violated, restart from scratch.  NO BACKTRACKING!
                        flag = true;
                        //std::cout << "row "<<i << " col "<<j<<std::endl;
                        
                        // reset values
                        auto it = this->begin();
                        it += 27; // point to first element in third row
                        auto itend = it+(i-2)*9; // point to one after last element in i-th row.
                        
                        while(it != itend)
                            *it++ = 0;
                        
                        /*
                         for(size_t i2 = 3; i2 <= i; i2++) {
                         for(size_t j2 = 0;j2 < 9; j2++)
                         this->operator()(i2,j2) = 0;
                         }
                         */
                        i=9;j=9;
                        
                    }
                }
            }
            
            auto completable_rows = set_of_possible_last_two_rows();
            
            if(completable_rows.size()) {
                // Generate random set of completable rows, uniform over those possible.
                std::uniform_int_distribution<size_t> unif(0,completable_rows.size()-1);
                auto r_index = unif(dsl::generator_32);
                
                auto rit = this->begin_row(7);
                auto it = begin(completable_rows[ r_index ].first);
                
                // Fill in penultimate row
                for(size_t i=0;i<9;++i)
                    *rit++ = *it++;
                
                
                rit = this->begin_row(8);
                it = begin(completable_rows[ r_index ].second);
                
                // Fill in final row
                for(size_t i=0;i<9;++i)
                    *rit++ = *it++;
                
                wt = (completable_rows.size()/16.);
            }
            else {
                //std::cout << "last two row rejection"<<std::endl;
                flag = true;
                // reset values
                auto it = this->begin();
                it += 27; // point to first element in third row
                auto itend = it+(5)*9; // point to one after last element in i-th row.
                
                while(it != itend)
                    *it++ = 0;
                
                
            }
            
            ++rejection_counts;
            
            //if(rejection_counts % 1000000 == 0)
            //    std::cout << rejection_counts << std::endl;
            
            
        }
        
        rejection_count.add_point( rejection_counts);
        //std::cout << "total rejections = "<< rejection_counts << std::endl;
        
        wt *= std::accumulate(std::begin(weights), std::end(weights), 1., std::multiplies<double>());
        
        final_randomization(gen);
        
        return sudoku<IntType>::object(*this, wt);
        
    }
    
    
    
    /** importance sampler using PDC for first three rows and last three rows.
     @param gen is the random number generator
     @returns a random Sudoku matrix, which contains the table of values and the importance sampling weight.
     */
    template<typename IntType>
    template<typename URNG>
    typename sudoku<IntType>::object sudoku<IntType>::importance_sample_first_three_last_three(URNG& gen) {
        
        
        auto it = this->begin();
        auto itend = this->end();
        
        while(it != itend)
            *it++ = 0;
        
        /*
         // reset values
         for(size_t i2 = 0; i2 < 9; i2++) {
         for(size_t j2 = 0;j2 < 9; j2++)
         this->operator()(i2,j2) = 0;
         }
         */
        
        
        // Sample first three rows unbiased from PDC
        this->sample_first_three_rows(gen);
        
        // all but the first three rows and last two rows.
        std::vector<double> weights(81-27-27);
        
        double wt = 0.;
        
        // Sample rows 4,5,6 using importance sampling, NO BACKTRACKING.
        size_t weight_index = 0;
        size_t rejection_counts = 0;
        
        bool flag = true;
        
        while(flag) {
            flag = false;
            weight_index = 0;
            // Loop through rows 4 through 6
            for(size_t i=3;i<6; ++i) {
                
                // Loop through columns 0 through 8
                for(size_t j=0;j<9;++j) {
                    
                    // Calculate R_{i,j}
                    auto x = allowable_part_sizes(i,j);
                    
                    // If not empty, fill in random value from the list
                    if(x.size()) {
                        std::uniform_int_distribution<short> unif(0,x.size()-1);
                        //auto it = std::begin(x);
                        //std::advance(it, unif(gen));
                        this->operator()(i,j) = x[unif(gen)];
                        weights[weight_index++] = (double)(x.size()) / (9.-j);
                    }
                    else { // if ANY of the conditions are violated, restart from scratch.  NO BACKTRACKING!
                        flag = true;
                        //std::cout << "row "<<i << " col "<<j<<std::endl;
                        
                        // reset values
                        auto it = this->begin();
                        it += 27; // point to first element in third row
                        auto itend = it+(i-2)*9; // point to one after last element in i-th row.
                        
                        while(it != itend)
                            *it++ = 0;
                        
                        //std::cout << *this << std::endl;
                        /*
                         for(size_t i2 = 3; i2 <= i; i2++) {
                         for(size_t j2 = 0;j2 < 9; j2++)
                         this->operator()(i2,j2) = 0;
                         }
                         */
                        i=9;j=9;
                        
                    }
                }
            }
            ++rejection_counts;
        }
        
        
        //auto completable_rows = set_of_possible_last_three_rows();
        auto counts = number_partial_completions(6,0);
        //std::cout << counts << std::endl;
        //auto completable_rows = set_of_possible_last_two_rows();
        
        
        //if(completable_rows.size()) {
        if(counts) {
            
            d_points.add_point(counts);
            
            auto completable_rows = set_of_possible_last_three_rows();
            
            //std::cout << completable_rows << std::endl;
            //std::cout << counts << std::endl;
            
            // Generate random set of completable rows, uniform over those possible.
            std::uniform_int_distribution<size_t> unif(0,completable_rows.size()-1);
            auto r_index = unif(dsl::generator_32);
            
            auto rit = this->begin_row(6);
            auto it = begin(completable_rows[ r_index ][0]);
            
            // Fill in penultimate row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
            rit = this->begin_row(7);
            it = begin(completable_rows[ r_index ][1]);
            
            // Fill in penultimate row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
            
            rit = this->begin_row(8);
            it = begin(completable_rows[ r_index ][2]);
            
            // Fill in final row
            for(size_t i=0;i<9;++i)
                *rit++ = *it++;
            
            
            wt = (completable_rows.size()/3456.);
        }
        else {
            //std::cout << "last two row rejection"<<std::endl;
            flag = true;
            // reset values
            auto it = this->begin();
            it += 27; // point to first element in third row
            auto itend = it+(4)*9; // point to one after last element in i-th row.
            
            while(it != itend)
                *it++ = 0;
            
            
        }
        
        
        //if(rejection_counts % 1000000 == 0)
        //    std::cout << rejection_counts << std::endl;
        
        
        
        
        rejection_count.add_point( rejection_counts);
        //std::cout << "total rejections = "<< rejection_counts << std::endl;
        
        wt *= std::accumulate(std::begin(weights), std::end(weights), 1., std::multiplies<double>());
        
        this->permute_rows( dsl::random_permutation_shifted(3, 6, gen));
        
        final_randomization(gen);
        
        d_points.write_to_file(SudokuFolder+"rejection_counts.txt");
        
        return sudoku<IntType>::object(*this, wt);
        
    }
    
    
    /** importance sampler using PDC for first three rows and last three rows.
     @param gen is the random number generator
     @returns a random Sudoku matrix, which contains the table of values and the importance sampling weight.
     */
    template<typename IntType>
    template<typename URNG>
    typename sudoku<IntType>::object sudoku<IntType>::importance_sample(URNG& gen) {
        
        return importance_sample_first_three_last_two(gen);
    }
    
#endif
    
    
}







#endif
