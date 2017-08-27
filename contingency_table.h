//
//  contingency_tables.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 3/27/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_contingency_tables_h
#define DeSalvo_Standard_Library_contingency_tables_h

#include <utility>
#include <tuple>
#include <random>

#include "std_cout.h"
#include "statistics.h"
#include "numerical_table.h"
#include "numerical.h"
#include "time.h"
#include "file.h"
#include "sequence.h"

namespace desalvo_standard_library {
    
    
    int level = 0;
    
    // See documentation.h
    template<typename EntryType=int, typename SumType=EntryType, typename Vector = std::vector<SumType>, typename Table = numerical_table<EntryType,SumType>>
    class binary_contingency_table : public  dsl::finite_sequence<dsl::store::bidirectional, binary_contingency_table<EntryType,SumType,Vector,Table>, Table> {
        
        
        typedef dsl::finite_sequence<dsl::store::bidirectional, dsl::binary_contingency_table<EntryType,SumType,Vector,Table>,Table> finite;

        
        friend typename dsl::finite_sequence<dsl::store::bidirectional, binary_contingency_table<EntryType,SumType,Vector,Table>,Table>::iterator;
    public:
        
        /** Initializes table with row sums and column sums
            @param initial_row_sums contains the row sum values
            @param initial_column_sums contains the column sum values
        */
        binary_contingency_table(Vector initial_row_sums, Vector initial_column_sums) :
        row_sums(initial_row_sums), column_sums(initial_column_sums) { };

        /** ... */
        bool violates_columns(const Table& t) const {
            
            auto n = column_sums.size();
            auto cit = column_sums.cbegin();
            
            for(size_t j=0;j<n;++j){
                
                auto sum = std::accumulate(t.cbegin_column(j), t.cend_column(j), static_cast<SumType>(0));
                if(sum > *cit) return true;
                ++cit;
            }
            return false;
        }
        
        bool first_in_sequence(Table& t) const {
            
            auto m = row_sums.size();
            auto n = column_sums.size();
            
            t = Table(m,n);
            
            auto rit = std::begin(row_sums);
            
            for(size_t i=0;i<m;++i) {
                auto row = dsl::binary_row(n,*rit++);
                
                std::copy_n(row.begin(), n, t.begin_row(i));
            }
            
            /*
            auto rit = std::begin(row_sums);
            
            for(size_t i=0;i<m;++i) {
                auto row = dsl::binary_row(n,*rit++);
                //std::cout << "roW = \n" << row << std::endl;
                std::copy_n(row.begin(), n, t.begin_row(i));
                
                while(violates_columns(t)) {
                    std::next_permutation(t.begin_row(i), t.end_row(i), std::greater<SumType>());
                    
                    //std::cout << "W = \n" << zeros << std::endl;
                    //std::cout << "t = \n" << t << std::endl;
                }
                
                
            }
*/
            
            /*
             // attempt at speeding up the above
            auto r = row_sums;
            auto c = column_sums;
            
            auto orderr = dsl::ordering(r);
            auto orderc = dsl::ordering(c);
            
            std::sort(r.rbegin(), r.rend());
            std::sort(c.begin(), c.end());

            auto rit = std::begin(r);
            auto cit = std::begin(c);

            // fill efficiently first
            for(size_t i=0;i<m;++i) {
                cit = std::begin(c);
                for(size_t j=0;j<n;++j) {
                    if(*rit > 0 && *cit > 0) {
                        t(i,j) = 1;
                        --*rit; --*cit;
                    }
                    ++cit;
                }
                ++rit;
            }
            
            std::cout << "t = \n" << t << std::endl;
            
            t.permute_rows(orderr);
            t.permute_columns(orderc);

            std::cout << "t = \n" << t << std::endl;
             */
            //std::cout << "t = \n" << t << std::endl;
            
            size_t current_row = m-1;
            while(t.template column_sums<Vector>() != column_sums) {
                
                while(current_row > 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    current_row--;
                }
                
                if(current_row == 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    //std::cout << "No possible tables exist!" << std::endl;
                    return false; //Table(m,n);
                }
                
                current_row = m-1;
                
                //std::cout << "t = \n" << t << std::endl;
                
                
            }
            
            return true;
            
        }
        
        /** ... */
        bool next_in_sequence(Table& t) const {
            
            auto m = row_sums.size();
            auto n = column_sums.size();
            
            size_t current_row = m-1;
            do {
                
                while(current_row > 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    current_row--;
                }
                
                if(current_row == 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    first_in_sequence(t);
                    return false;
                }
                current_row = m-1;
                
                
            } while(t.template column_sums<Vector>() != column_sums);
            
            return true;
            
        }

        /** ... */
        template<typename Parameters = std::pair<Vector,Vector> >
        class object : public Table {
        public:
            object(Vector row_sums, Vector column_sums, Table t) : Table(t), p(row_sums, column_sums) {
            }
            
            object(Parameters initial_p) : Table(p.first.size(), p.second.size()) {
                
            }
            
            Table as_table() { return *this; }
            
            
            bool valid = false;

        private:
            Parameters p;
        };
        
        
        /** ... */
        template<typename Parameters = std::pair<Vector,Vector> >
        class generator {
        public:
            
            generator(Parameters initial_p) : p(initial_p) { };
            
            template<typename URNG=std::mt19937_64>
            typename binary_contingency_table::object<Parameters> operator()(URNG& gen = generator_64, double verbose=0);

        private:
            Parameters p;
            
            std::tuple<Vector,Vector,Table,bool> deterministic_fill(Vector row, Vector column, double verbose = 0);
            std::tuple<Vector,Vector,Table,bool> deterministic_fill(size_t i, size_t j, EntryType k, Vector row, Vector column, double verbose = 0);
            
            template<typename VectorOfFloats = std::vector<long double>>
            void update_q(VectorOfFloats& q);
            
            template<typename VectorOfFloats = std::vector<long double>>
            void update_q(VectorOfFloats& q, const Vector& row, const Vector& column);
            
            template<typename VectorFloat>
            long double rejection_function(size_t i, size_t j, const Table& k, const Vector& row, const Vector& column, const VectorFloat& q);
            long double rejection_function(size_t i, size_t j, EntryType k, const Vector& row, const Vector& column);
            long double row_rejection(size_t i, size_t j, EntryType k, const Vector& row, const Vector& column);
            long double column_rejection(size_t i, size_t j, EntryType k, const Vector& row, const Vector& column);
            
            
            // local check of GaleRyser condition after the completion of each column
            bool check_GaleRyser(const Vector& r, const Vector& c) {
                auto cc = dsl::conjugate(c);
                return (dsl::majorizes(r, cc) || dsl::majorizes(cc, r));
            }
            
            
            void print_parameters(const Parameters& p_local, std::ostream& output = std::cout ) {
                output << "r = "   << p_local.first << std::endl;
                output << "c = "   << p_local.second << std::endl;
            }
            
            template<typename Tuple>
            void print(const Tuple& p_local, std::ostream& output = std::cout) {
                output << "r = "     << std::get<0>(p_local) << std::endl;
                output << "c = "     << std::get<1>(p_local) << std::endl;
                output << "A = \n"   << std::get<2>(p_local) << std::endl;
                output << "error = " << std::get<3>(p_local) << std::endl;
            }
            
            void print(const Vector& r, const Vector& c, const Table& A, std::ostream& output = std::cout) {
                output << "r = "   << r << std::endl;
                output << "c = "   << c << std::endl;
                output << "A = \n" << A << std::endl;
            }
            
            void print(const Vector& r, const Vector& c, std::ostream& output = std::cout) {
                output << "r = "   << r << std::endl;
                output << "c = "   << c << std::endl;
            }

        };
        
        
        void reset_parameters_to(Vector replacement_row_sums, Vector replacement_column_sums) {
            row_sums = replacement_row_sums;
            column_sums = replacement_column_sums;
            
            // check if valid.
            is_emptyset = Satisfies_GaleRyser_Condition();
        }
        
        bool is_emptyset;
        
        
        template<typename Parameters = std::pair<Vector,Vector>, typename URNG=std::mt19937_64>
        object<Parameters> random(URNG& gen = generator_64, double verbose = 0) {
            generator<Parameters> generate(std::make_pair(row_sums, column_sums));
            return generate(gen,verbose);
        }
        
        
        
    private:
        Vector row_sums;
        Vector column_sums;
        
        bool Satisfies_GaleRyser_Condition() {
            // Check the Gale-Ryser condition
            return (dsl::majorizes(row_sums, dsl::conjugate(column_sums)) || dsl::majorizes(dsl::conjugate(column_sums),row_sums));
        }
        

    };
    
    /** Implements the deterministic fill algorithm, which checks to see if any entries can be filled in deterministically. */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    std::tuple<Vector,Vector,Table,bool> binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::deterministic_fill(Vector row, Vector column, double verbose) {
        
        auto m = row.size();
        auto n = column.size();
        
        bool errorflag=false;
        bool repeat = true;
        
        EntryType zero = static_cast<EntryType>(0);
        EntryType one  = static_cast<EntryType>(1);
        
        numerical_table<EntryType,SumType> B(m,n,EntryType(0));
        
        // As long as Zeros or the table
        while(repeat) {
            repeat = false;
            
            auto row_it = row.begin();
            
            // Check for rows
            for(size_t i=0;i<m;++i) {
                
                // total number of non-zero columns
                auto total = std::count_if(column.begin(), column.end(),[](SumType& a) { return a != 0; }); //static_cast<SumType>(0));
                auto row_sum = *row_it;
                
                //std::cout << "Checking row " << i+1 << " with Total[W[["<<i<<"]]]="<<total<<std::endl;
                
                if(row_sum > n - total) {
                    errorflag = true;
                    
                    if(verbose >= 1)
                        std::cout << "Error row sum of "<<i+1<<": "<<row_sum << " is larger than " << n-total << "entries remaining." << std::endl;
                    return std::make_tuple(row,column,B,errorflag);
                }
                
                // if a row is uniquely determined by the constraints
                if(row_sum > 0 && total == row_sum) {
                    
                    repeat = true;
                    
                    auto B_row = B.begin_row(i);
                    auto column_it = column.begin();
                    
                    for(size_t j=0;j<n;++j) {
                        
                        // if the column is non-zero, fill in a 1
                        if(*column_it != zero) {
                            *B_row = one;
                            --(*row_it);
                            --(*column_it);
                            //*table_row = one;
                        }
                        ++column_it;
                        ++B_row;
                        //++table_row;
                        
                    }
                }
                
                ++row_it;
                
            }
            
            
            /*std::cout << "r = " << row << std::endl;
             std::cout << "c = " << column << std::endl;
             std::cout << "A = \n" << B << std::endl;
             std::cout << "W = \n" << Zeros << std::endl;
             */
            
            auto column_it = column.begin();
            
            // Check for rows
            for(size_t j=0;j<n;++j) {
                
                
                auto total = std::count_if(row.begin(), row.end(),[](SumType& a) { return a != 0; }); //static_cast<SumType>(0));
                auto column_sum = *column_it;
                
                //std::cout << "Checking column " << j+1 << " with Total[W[["<<j<<"]]]="<<total<<std::endl;
                
                
                if(column_sum > n - total) {
                    if(verbose >= 1)
                        std::cout << "Error column sum of "<<j+1<<": "<<column_sum << " is larger than " << n-total << "entries remaining."<< std::endl;
                    errorflag = true;
                    return std::make_tuple(row,column,B,errorflag);
                }
                
                
                if(column_sum > 0 && total == column_sum) {
                    
                    repeat = true;
                    
                    //auto table_column = Zeros.begin_column(j);
                    auto B_column = B.begin_column(j);
                    auto row_it = row.begin();
                    
                    for(size_t i=0;i<m;++i) {
                        
                        // if row sum is non-zero
                        if(*row_it != zero) {
                            *B_column = one;
                            --(*row_it);
                            --(*column_it);
                            //*table_column = one;
                        }
                        ++row_it;
                        ++B_column;
                        //++table_column;
                        
                    }
                }
                
                /*std::cout << "r = " << row << std::endl;
                 std::cout << "c = " << column << std::endl;
                 std::cout << "A = \n" << B << std::endl;
                 std::cout << "W = \n" << Zeros << std::endl;
                 */
                
                ++column_it;
                
            }
            
        }
        
        //std::cout << "DONE." << std::endl;
        
        return std::make_tuple(row,column,B,errorflag);
        
    }
    
    
    /** Implements the deterministic fill algorithm, which checks to see if any entries can be filled in deterministically. */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    std::tuple<Vector,Vector,Table,bool> binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::deterministic_fill(size_t i, size_t j, EntryType k, Vector row, Vector column, double verbose) {
        
        auto m = row.size();
        auto n = column.size();
        
        bool errorflag=false;
        bool repeat = true;
        
        EntryType zero = static_cast<EntryType>(0);
        EntryType one  = static_cast<EntryType>(1);
        
        numerical_table<EntryType,SumType> B(m,n,EntryType(0));
        numerical_table<EntryType,SumType> Zeros(m,n,EntryType(0));
        
        // Make Zeros all 1s in columns 1, 2, ..., j-1, and then all 1s in column j for rows 1,2,...,i
        for(size_t jj=0;jj<j;++jj)
            for(size_t ii=0;ii<m;++ii)
                Zeros(ii,jj) = one;
        
        for(size_t ii=0;ii<=i;++ii)
            Zeros(ii,j) = one;
        
        B(i,j) = k;
        
        auto cit = std::begin(column);    auto rit = std::begin(row);
        std::advance(cit, j);             std::advance(rit, i);
        
        if(k != 0 && *cit != 0 && *rit != 0) {
            *cit -= k;
            *rit -= k;
        }
        
        if(verbose >= 2) {
            print(row,column);
            std::cout << "B = \n" << B << std::endl;
            std::cout << "W = \n" << Zeros << std::endl;
        }
        
        // As long as Zeros or the table
        while(repeat) {
            repeat = false;
            
            auto row_it = row.begin();
            
            // Check for rows
            for(size_t i=0;i<m;++i) {
                
                
                auto total = std::accumulate(Zeros.begin_row(i), Zeros.end_row(i),static_cast<SumType>(0));
                auto row_sum = *row_it;
                
                //std::cout << "Checking row " << i+1 << " with Total[W[["<<i<<"]]]="<<total<<std::endl;
                
                if(row_sum > n - total) {
                    errorflag = true;
                    
                    if(verbose >= 1)
                        std::cout << "Error row sum of "<<i+1<<": "<<row_sum << " is larger than " << n-total << "entries remaining." << std::endl;
                    return std::make_tuple(row,column,B,errorflag);
                }
                
                // If row sum is 0 and W is not all 1s in that row, make it so.
                if(row_sum == 0 && total  < n) {
                    repeat = true;
                    
                    for(auto& x : Zeros.row(i))
                        x = one;
                    
                }
                
                // if a row is uniquely determined by the constraints
                if(row_sum > 0 && total == n-row_sum) {
                    
                    repeat = true;
                    
                    auto table_row = Zeros.begin_row(i);
                    auto B_row = B.begin_row(i);
                    auto column_it = column.begin();
                    
                    for(size_t j=0;j<n;++j) {
                        
                        if(*table_row != one) {
                            *B_row = one;
                            --(*row_it);
                            --(*column_it);
                            *table_row = one;
                        }
                        ++column_it;
                        ++B_row;
                        ++table_row;
                        
                    }
                }
                
                ++row_it;
                
            }
            
            
            /*std::cout << "r = " << row << std::endl;
             std::cout << "c = " << column << std::endl;
             std::cout << "A = \n" << B << std::endl;
             std::cout << "W = \n" << Zeros << std::endl;
             */
            
            auto column_it = column.begin();
            
            // Check for rows
            for(size_t j=0;j<n;++j) {
                
                
                auto total = std::accumulate(Zeros.begin_column(j), Zeros.end_column(j),static_cast<SumType>(0));
                auto column_sum = *column_it;
                
                //std::cout << "Checking column " << j+1 << " with Total[W[["<<j<<"]]]="<<total<<std::endl;
                
                
                if(column_sum > n - total) {
                    if(verbose >= 1)
                        std::cout << "Error column sum of "<<j+1<<": "<<column_sum << " is larger than " << n-total << "entries remaining."<< std::endl;
                    errorflag = true;
                    return std::make_tuple(row,column,B,errorflag);
                }
                
                if(column_sum == 0 && total  < n) {
                    
                    repeat = true;
                    
                    for(auto& x : Zeros.column(j))
                        x = one;
                }
                
                if(column_sum > 0 && total == n-column_sum) {
                    
                    repeat = true;
                    
                    auto table_column = Zeros.begin_column(j);
                    auto B_column = B.begin_column(j);
                    auto row_it = row.begin();
                    
                    for(size_t i=0;i<m;++i) {
                        
                        if(*table_column != one) {
                            *B_column = one;
                            --(*row_it);
                            --(*column_it);
                            *table_column = one;
                        }
                        ++row_it;
                        ++B_column;
                        ++table_column;
                        
                    }
                }
                
                /*std::cout << "r = " << row << std::endl;
                 std::cout << "c = " << column << std::endl;
                 std::cout << "A = \n" << B << std::endl;
                 std::cout << "W = \n" << Zeros << std::endl;
                 */
                
                ++column_it;
                
            }
            
        }
        
        //std::cout << "DONE." << std::endl;
        
        //return std::make_tuple(row,column,B,Zeros,errorflag);
        
        return std::make_tuple(row,column,B,errorflag);
        
    }
    

    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    template<typename VectorOfFloats>
    void binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::update_q(VectorOfFloats& q) {
        
        // Only need to get the current set of values, no need for make a copy.
        const auto& r = std::get<0>(p);
        const auto& c = std::get<1>(p);
        
        auto n = c.size();
        auto m = r.size();
        
        auto c_it = c.begin();
        auto q_it = q.begin();
        
        long double cj_as_float = 0;
        SumType total = std::count_if(std::begin(r), std::end(r), [](const SumType& a) { return a == 0; });
        
        for(size_t j=0; j<n; ++j) {
            
            if(*c_it == 0)
                *q_it = 1.;
            else {
                cj_as_float = *c_it;
                //*q_it = cj_as_float / (cj_as_float + m - total);
                *q_it = cj_as_float / (m - total);
            }
            ++c_it;
            ++q_it;
        }
        return;
    }

    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    template<typename VectorOfFloats>
    void binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::update_q(VectorOfFloats& q, const Vector& r, const Vector& c) {
        
        // Only need to get the current set of values, no need for make a copy.
        //const auto& r = std::get<0>(p);
        //const auto& c = std::get<1>(p);
        
        auto n = c.size();
        auto m = r.size();
        
        auto c_it = c.begin();
        auto q_it = q.begin();
        
        long double cj_as_float = 0;
        SumType total = std::count_if(std::begin(r), std::end(r), [](const SumType& a) { return a == 0; });
        
        for(size_t j=0; j<n; ++j) {
            
            if(*c_it == 0)
                *q_it = 1.;
            else {
                cj_as_float = *c_it;
                //*q_it = cj_as_float / (cj_as_float + m - total);
                *q_it = cj_as_float / (m - total);
            }
            ++c_it;
            ++q_it;
        }
        return;
    }

    
    /** Implements the PDC algorithm using binary contingency table with forced zeros. */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    template<typename URNG>
    typename binary_contingency_table<EntryType,SumType,Vector,Table>::template object<Parameters>
    binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::operator()(URNG& gen, double verbose) {
        
        
        level++;
        //std::cout << "level up: " << level << std::endl;
        
        //binary_contingency_table_with_zeros bct(std::get<0>(p), std::get<1>(p), std::get<2>(p));
        
        dsl::time timer;
        
        dsl::file< dsl::file_type::output > output("/Users/stephendesalvo/Documents/Research/100_x_100_timings.txt");
        
        //EntryType zero = static_cast<EntryType>(0);
        EntryType one = static_cast<EntryType>(1);
        
        auto r = p.first;
        auto c = p.second;
        
        if(verbose >=1) {
            std::cout << "Initial input values: " << std::endl;
            print(r,c);
        }
        
        auto final_orderrows = dsl::ordering(r);
        auto final_ordercolumns = dsl::ordering(c);
        
        auto orderrows = final_orderrows;
        auto ordercolumns = final_ordercolumns;
        
        std::sort(std::begin(r), std::end(r));
        std::sort(std::begin(c), std::end(c));
    
        
        if(verbose >= 4) {
            std::cout << "Initial re-ordering: " << std::endl;
            std::cout << "row    reorder: " << final_orderrows << std::endl;
            std::cout << "column reorder: " << final_ordercolumns << std::endl;
        }

        if(verbose >= 1) {
            std::cout << "After initial re-ordering: " << std::endl;
            std::cout << "r = " << r << std::endl;
            std::cout << "c = " << c << std::endl;
        }

        
        auto m = r.size();
        auto n = c.size();
        
        Table A(m,n,EntryType(0));
        
        // valid until proven false!
        bool valid_flag = true;
        
        bool repeating_current_j = false;
        size_t repeat_count = 0;
        bool need_to_transpose = false;
        
        auto saver = r;
        auto savec = c;
        auto saveA = A;
        
        if( check_GaleRyser(r,c) == false) {
            
            if(verbose>=1.9) {
                std::cout << "Initial input fails GaleRyser condition" << std::endl;
                std::cout << "r = " << r << std::endl;
                std::cout << "c = " << c << std::endl;
            }
            
            binary_contingency_table::object<Parameters> obj(p.first, p.second, A);
            obj.valid = false;
            
            return obj;
        }
        
        
        
        
        for(size_t j=0;j<n-1;++j) {
            
            
            if(!repeating_current_j) {
                
                //std::cout << "j = " << j << std::endl;
                
                if(j>0 && check_GaleRyser(r,c) == false) {
                
                    if(verbose>=1.9) {
                        std::cout << "Failed GaleRyser condition at column j = " << j << std::endl;
                        std::cout << "r = " << r << std::endl;
                        std::cout << "c = " << c << std::endl;
                        for(auto& x : A.column(j)) std::cout << x << ",";
                        std::cout << std::endl;
                    }
                    
                    --j; // redo the column;
                    
                    r = saver;
                    c = savec;
                    A = saveA;
                    
                    repeating_current_j = true;
                    
                    if(verbose>=1.9) {
                        std::cout << "Restarting with state: " <<  std::endl;
                        std::cout << "r = " << r << std::endl;
                        std::cout << "c = " << c << std::endl;
                        std::cout << "A = \n" << A << std::endl;
                    }
                }
                else {
                    repeating_current_j = false;
                }
                
                
                
                
                
                auto r_it = std::begin(r);
                auto c_it = std::begin(c); std::advance(c_it, j);
                
                orderrows = dsl::ordering(r);
                ordercolumns = dsl::ordering(c);
                
                //std::sort(std::begin(r), std::end(r));
                //std::sort(std::begin(c), std::end(c));
                
                if(verbose >=4) {
                    std::cout << "before permute: " << std::endl;
                    std::cout << "r = " << r << std::endl;
                    std::cout << "c = " << c << std::endl;
                    std::cout << "orderrows = " << orderrows << std::endl;
                    std::cout << "ordercolumnss = " << ordercolumns << std::endl;
                    std::cout << "A = \n" << A << std::endl;
                    //std::cout << "W = \n" << W << std::endl;
                }
                
                if(!std::is_sorted(std::begin(r), std::end(r))) {
                    A.permute_rows(orderrows);
                    dsl::permute_by(final_orderrows, orderrows);
                    permute_by(r, orderrows);
                }
                
                if(!std::is_sorted(std::begin(c), std::end(c))) {
                    A.permute_columns(ordercolumns);
                    dsl::permute_by(final_ordercolumns, ordercolumns);
                    permute_by(c, ordercolumns);
                }
                
                if(verbose >=4) {
                    std::cout << "after permute: " << std::endl;
                    std::cout << "r = " << r << std::endl;
                    std::cout << "c = " << c << std::endl;
                    std::cout << "A = \n" << A << std::endl;
                    //std::cout << "W = \n" << W << std::endl;
                }
                
                if(verbose >= 4) {
                    std::cout << "final row re-ordering: " << final_orderrows << std::endl;
                    std::cout << "final column re-ordering: " << final_ordercolumns << std::endl;
                }
                
                if(verbose >= 1.5) {
                    std::cout << "Working on column: " << j << " " << timer.toc() << std::endl;
                    output <<"Working on column: " << j << " " << timer.toc() << std::endl;
                    timer.reset();
                    
                    if(verbose >= 1.7) {
                        std::cout << "r = " << r << std::endl;
                        std::cout << "c = " << c << std::endl;
                        
                    }
                }
            }
            else
                ++repeat_count;
            
            
            

            // if we've repeated the same column more than 1000 times, take the transpose
            if(repeat_count >= 1000) {
                if(verbose >= 2)
                    std::cout << "Transposing ... " << std::endl;
                repeat_count = 0;
                need_to_transpose = !need_to_transpose;
                
                // m is the number of rows
                dsl::transpose(A.begin(), A.end(), m);
                std::swap(r,c);
                std::swap(m,n);
                
                std::cout << "r = " << r << std::endl;
                std::cout << "c = " << c << std::endl;
                
                // start columns over
                j=0;
                
            }
            
            
            saver = r;
            savec = c;
            saveA = A;
            
            
            // vector of probabilities, figure out how many rows are zero too
            std::vector<long double> q(n);
            
            update_q(q);
        
            // advance iterator to j-th column
            auto c_it = std::begin(c); std::advance(c_it, j);
            
            auto r_it = std::begin(r);
            
            for(size_t i=0;i<m-1;++i) {
                
                // if row or column sum is 0 skip entry
                if(*c_it != 0 && *r_it != 0) {
                    
                    if(verbose >=2) {
                        std::cout << "current values: " << std::endl;
                        print(r,c,A);
                    }
                    
                    if(verbose >=2) std::cout << "Sampling next at index (i,j) = (" << i << "," << j << ")" << std::endl;
                    
                    
                    // Calculate rejection probabilities.
                    std::pair<long double, long double> outcomes;
                    
                    // Assume bit is 0/1, then fill in all resulting deterministic bits
                    auto var0 = deterministic_fill(i,j,0,r,c,verbose);
                    auto var1 = deterministic_fill(i,j,1,r,c,verbose);
                    
                    if(verbose >= 2) {
                        std::cout << "Assuming bit is 0" << std::endl;
                        print(var0);
                        
                        std::cout << "Assuming bit is 1" << std::endl;
                        print(var1);

                    }
                    
                    // Calculate rejection function using all deterministic bits
                    outcomes.first  = rejection_function(i,j,std::get<2>(var0),std::get<0>(var0),std::get<1>(var0),q);
                    outcomes.second = rejection_function(i,j,std::get<2>(var1),std::get<0>(var1),std::get<1>(var1),q);
                    
                    if(verbose >=2) std::cout << "probs = " << outcomes << std::endl;
                    
                    if(verbose >= 3) std::cout << "At index (i,j) = (" << i << "," << j << ") probabilities = (" << outcomes.first << "," << outcomes.second << ")" << std::endl;
                    
                    
                    // I should make tol a function of m,n,r,c
                    long double tol = std::pow<long double>(10.,-15.);
                    bool repeat_flag = true;
                    
                    // if bit cannot be 0
                    if(outcomes.first < tol && outcomes.second >= tol) {
                        repeat_flag = false;

                        // replace r and c based on deterministic fill values
                        r = std::get<0>(var1);
                        c = std::get<1>(var1);
                        A += std::get<2>(var1); // *update* A with newly filled ones
                        
                    // if bit cannot be 1
                    } else if(outcomes.first >= tol && outcomes.second < tol) {
                        
                        repeat_flag = false;
                        r = std::get<0>(var0);
                        c = std::get<1>(var0);
                        A += std::get<2>(var0);

                    // if bit cannot be either 0 or 1, ERROR!  May need to restart algorithm.
                    } else if(outcomes.first < tol && outcomes.second < tol) {
                        if(verbose >=3 )
                            std::cout << "WARNING!  Both rejection probabilities less than tolerance ... leaving in 0, possible undesireable consequences." << std::endl;

                    // if bit can be either 0 or 1
                    } else {
                        
                        // both rejection functions of reasonable size, normalize.
                        auto norm = std::max(outcomes.first, outcomes.second);
                        
                        outcomes.first  /= norm;
                        outcomes.second /= norm;
                        
                        if(verbose >=2) std::cout << "probs scaled = " << outcomes << std::endl;
                        
                        
                        long double threshold;
                        
                        auto q_it = q.begin(); std::advance(q_it, j);
                        
                        long double qj = static_cast<long double>(*q_it);
                        
                        std::bernoulli_distribution bern(qj / (1.+qj));
                        std::uniform_real_distribution<long double> u(0.,1.);
                        
                        bool b;
                        
                        while(repeat_flag) {
                            
                            repeat_flag = false;
                            
                            b = bern(gen);
                            
                            if(verbose >=2) std::cout << "Random bit is " << b << std::endl;
                            
                            // rejection probability depends on outcome
                            threshold = (b == 0 ? outcomes.first : outcomes.second);
                            
                            if(u(gen) <= threshold ) {
                                repeat_flag = false;
                                if(verbose >=3) std::cout << "A("<<i<<","<<j<<") = " << b << " accepted." << std::endl;
                                
                                // if accepted bit is 0, fill in deterministic values
                                if(b == 0) {
                                    r = std::get<0>(var0);
                                    c = std::get<1>(var0);
                                    A += std::get<2>(var0);
                                }
                                else { // if accepted bit is 1, fill in deterministic values
                                    r = std::get<0>(var1);
                                    c = std::get<1>(var1);
                                    A += std::get<2>(var1);
                                }
                                
                            }
                            else { // bit not accepted, resample
                                repeat_flag = true;
                                if(verbose >=3) std::cout << "A("<<i<<","<<j<<") = " << b << " rejected." << std::endl;
                            }
                        }
                    }
                    
                }
                ++r_it;
            }
        }
            
        
        if(std::accumulate(r.cbegin(), r.cend(), static_cast<SumType>(0)) != std::accumulate(c.cbegin(), c.cend(), static_cast<SumType>(0))) {
            if(verbose >= 2) {
                std::cout << "ERROR: Not all row sums and column sums are zero" << std::endl;
            }
            valid_flag = false;
        }
    
        
        auto rit = std::get<0>(p).begin();
        
        for(size_t i=0; i<m; ++i) {
            auto row_sum = std::accumulate(A.begin_row(i), A.end_row(i),static_cast<SumType>(0));
            if(*rit != row_sum){
                if(verbose >=2)
                    std::cout << "Error row " << i+1 << " sum: " << row_sum << " does not match " << *rit << std::endl;
                valid_flag = false;
            }
            ++rit;
        }
        
        auto cit = std::get<1>(p).begin();
        for(size_t j=0; j<n; ++j) {
            auto column_sum = std::accumulate(A.begin_column(j), A.end_column(j),static_cast<SumType>(0));
            if(*cit != column_sum){
                if(verbose >=2)
                    std::cout << "Error column " << j+1 << " sum: " << column_sum << " does not match " << *cit << std::endl;
                valid_flag = false;
            }
            ++cit;
        }
        
        if(need_to_transpose) {
            std::cout << "Transposing back " << std::endl;
            dsl::transpose(A.begin(), A.end(), m);
            
        }

        if(verbose >= 4) {
            std::cout << "A = \n" << A << std::endl;
            std::cout << "final row    re-ordering: " << final_orderrows << std::endl;
            std::cout << "final column re-ordering: " << final_ordercolumns << std::endl;
        }
        
        //auto rinv = dsl::permutation_inverse_direct_form(final_orderrows);
        //auto cinv = dsl::permutation_inverse_direct_form(final_ordercolumns);
        
        //A.permute_rows(final_orderrows);
        //A.permute_columns(final_ordercolumns);
        
            if(!std::is_sorted(std::begin(final_orderrows), std::end(final_orderrows))) {
                A.permute_rows(final_orderrows);
            }
            
            if(!std::is_sorted(std::begin(final_ordercolumns), std::end(final_ordercolumns))) {
                A.permute_columns(final_ordercolumns);
            }
            
        
        if(verbose >= 4) {
            std::cout << "After final re-ordering" << std::endl;
            std::cout << "A = \n" << A << std::endl;
        }
        
        binary_contingency_table::object<Parameters> obj(p.first, p.second, A);
        
        obj.valid = valid_flag;
        
        level--;
        //std::cout << "level down: " << level << std::endl;

        
        return obj;
        
    }
    
    
    template<
    typename EntryType=unsigned int, typename SumType=EntryType,
    typename Vector = std::vector<SumType>,
    typename Table=numerical_table<EntryType,SumType>,
    typename Parameters=std::pair<Vector,Vector>,
    typename URNG=std::mt19937_64 >
    typename binary_contingency_table<EntryType,SumType,Vector,Table>::template object<Parameters> random_binary_contingency_table(Vector row_sums, Vector column_sums, URNG& gen=generator_64, double verbose = 0)  {
        
        //typename binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::template object<Parameters> bct(row_sums, column_sums, Zeros);
        binary_contingency_table<EntryType,SumType,Vector,Table> bct(row_sums, column_sums);
        
        unsigned int counts = 0;
        unsigned int max_iterations = 100000;
        
        auto obj = bct.random(gen,verbose);
        
        while(!obj.valid && counts < max_iterations){
            obj = bct.random(gen,verbose);
            ++counts;
        }
        
        if(counts == max_iterations)
            std::cout << "Warning!  Maximum iterations " << max_iterations << " reached."<<std::endl;
        
        return obj;
    }

    
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    template<typename VectorFloat>
    long double binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::rejection_function(size_t i, size_t j, const Table& A, const Vector& row, const Vector& column, const VectorFloat& q) {
        
        //return 1.;
        
        // we use set because if any row or column has a 1, we simply calculate the rejection function once
        std::set<size_t> row_indices;
        std::set<size_t> column_indices;
        
        long double prob = 1.;
        
        // Trying to make this work.
        auto m = row.size();
        auto n = column.size();
        
        numerical_table<EntryType,SumType> zeros(m,n,EntryType(0));
        
        
        // 1 1 1 0 0 0 0
        // 1 1 1 0 0 0 0
        // 1 1 1 * 0 0 0
        // 1 1 1 0 0 0 0
        // 1 1 1 0 0 0 0
        // Make Zeros all 1s in columns 1, 2, ..., j-1, and then all 1s in column j for rows 1,2,...,i
        for(size_t jj=0;jj<j;++jj)
            for(size_t ii=0;ii<m;++ii)
                zeros(ii,jj) = 1;

        // 1 1 1 1 0 0 0
        // 1 1 1 1 0 0 0
        // 1 1 1 1 0 0 0
        // 1 1 1 0 0 0 0
        // 1 1 1 0 0 0 0

        for(size_t ii=0;ii<=i;++ii)
            zeros(ii,j) = 1;
        
        
        // 1 1 1 1 0 d 0
        // 1 1 1 1 0 d 0
        // 1 1 1 1 d d d
        // 1 1 1 d 0 d 0
        // 1 1 1 d 0 d 0
        for(size_t ii=0;ii<m;++ii)
            for(size_t jj=0;jj<n;++jj)
                if(A(ii,jj)==1) {
                
                    zeros(ii,jj) = 1;
                    row_indices.insert(ii);
                    column_indices.insert(jj);
                    prob *= q[jj];
                }
        
        // but exclude the bit sampled in correct proportion
        if(A(i,j) == 1)
            prob /= q[j];
        
        auto rit = std::begin(row);
        for(size_t ii=0;ii<m;++ii)
            if(*rit ==0)
                for(size_t jj=0;jj<n;++jj)
                    zeros(ii,jj) = 1;

        auto cit = std::begin(column);
        for(size_t jj=0;jj<n; ++jj)
            if(*cit ==0)
                for(size_t ii=0;ii<m;++ii)
                    zeros(ii,jj) = 1;

        
        //std::cout << "zeros = \n" << zeros << std::endl;
        //std::cout << "A = \n" << A << std::endl;
        /*
        std::vector<long double> q(n,1.);
        
        auto cit = column.cbegin();
        std::advance(cit,j);
        
        // calculate column rejections and q vector
        for(size_t jj=j;jj<n;++jj) {
            SumType total = std::accumulate(zeros.cbegin_column(jj), zeros.cend_column(jj), static_cast<SumType>(0));
        
            if(*cit > 0) {
                long double cj = *cit;
                //long double p = cj/(cj+m-total);
                q[jj] = cj/(cj+m-total);
        
                //prob *= std::exp(std::lgamma(m-total+1.)-std::lgamma(cj+1)-std::lgamma(m-total-cj+1)+cj*log(p)+(m-total-cj)*log(1.-p));
            }
            ++cit;
        }
        
        
        // if a 1 appears in A, update entry rejection probability and mark it for row/column rejection
        for(size_t ii=0;ii<m;++ii)
            for(size_t jj=0;jj<n;++jj)
                if(A(ii,jj)==1) {
                    prob *= 1./q[jj];
                }
        
        //prob *= q[j];
        */
        
        
        long double eps = 10.E-15;
        long double cj = 0.;
        
        // For each column for which a bit was determined
        for(auto& j : column_indices) {
            
            SumType total = std::accumulate(zeros.cbegin_column(j), zeros.cend_column(j), static_cast<SumType>(0));
            
            // not sure which set of j are included, so just do it manually each time.  Ok for random access vectors
            auto cit = std::begin(column);
            std::advance(cit,j);
            
            //long double p = cj/(cj+m-total);
            
            // If the column sum is nonzero, calculate probability, otherwise assume it is 1.
            if(*cit > 0) {
                cj = *cit;
                prob *= std::exp(std::lgamma(m-total+1.)-std::lgamma(cj+1)-std::lgamma(m-total-cj+1)+cj*log(1.-q[j])+(m-total-cj)*log(q[j]));
            }
            
        }
        
        auto q_local = q;
        
        auto last = std::copy_if(std::begin(q), std::end(q), std::begin(q_local), [&](long double a) { return a < 1.-eps; });
        
        q_local.erase(last, std::end(q_local));
        
        poisson_binomial<std::vector<long double>> pb(q_local);
        
        // For each row for which a bit was determined
        for(auto& i : row_indices) {
            
            // initialize row iterator to row i
            auto rit = std::begin(row); std::advance(rit,i);
            
            if(*rit > 0)
                prob *= pb.pdf(*rit);
            
        }
        
        //std::cout << "prob = " << prob << std::endl;
        return prob;
        
        //return 1.;
         
        
        
    }
    
    
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::rejection_function(size_t i, size_t j, EntryType k, const Vector& row, const Vector& column) {
        
        //return row_rejection(i,j,k,row,column)*column_rejection(i,j,k,row,column)  > 0 ? 1-(1./(i+j+k+1.01)) : 0;
        return row_rejection(i,j,k,row,column)*column_rejection(i,j,k,row,column); //  > 0 ? 1-(1./(i+j+k+1.01)) : 0;
        
    }
    
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::row_rejection(size_t i, size_t j, EntryType k, const Vector& row, const Vector& column) {
        
        
        auto rit = row.cbegin(); std::advance(rit,i);
        
        if(*rit == 0) {
            return 1.-k;
        }
        
        auto n = column.size();
        auto m = std::count_if(row.begin(), row.end(), [](SumType a) { return a!=0; });
        
        // only make q as long as the number of non-zero column sums.  Start with full size then resize later.
        std::vector<long double> q(n,0.);
        
        auto cit = column.begin();
        std::advance(cit, j);
        long double cj = *cit;
        
        // Since we won't use all n elements, this will index the vector q
        size_t index = 0;
        
        
        for(size_t jj=j+1;jj<n;++jj) {
            
            cj = *cit;
            
            
            if(cj >0) {
                
                q[index++] = cj / (cj + m);
            }
            
            ++cit;
        }
        
        q.resize(index);
        
        
        poisson_binomial<std::vector<long double>> pb(q);
        
        long double prob = pb.pdf(*rit-k);
        
        //std::cout << "evaluate row distr at " << *rit << "-" << k << " = " << *rit - k << std::endl;
        //std::cout << "row_rejection("<<i<<","<<j<<","<<k<<","<<row<<","<<column<<") = " << prob << std::endl;
        
        return prob;
        
        
    }
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table<EntryType,SumType,Vector,Table>::generator<Parameters>::column_rejection(size_t i, size_t j, EntryType k, const Vector& row, const Vector& column) {
        
        auto m = row.size();
        //auto m = std::count_if(row.begin(), row.end(), [](SumType a) { return a!=0; });

        //SumType total = std::accumulate(zeros.cbegin_column(j), zeros.cend_column(j), static_cast<SumType>(0));
        
        auto cit = column.cbegin();
        std::advance(cit,j);
        
        long double prob = 0.;
        
        if(*cit == 0) {
            return 1.-k;
        }
        else if(*cit == m-i) {
            return k;
        }
        else {
            long double cj = *cit;
            
            long double p = cj/(cj+m-i);
            
            unsigned int cprime = cj-k;
            
            prob = std::exp(std::lgamma(m-i+1.)-std::lgamma(cprime+1)-std::lgamma(m-i-cprime+1)+cprime*log(p)+(m-i-cprime)*log(1.-p));
            
            //std::cout << "evaluate column distr at " << cj << "-" << k << " = " << cj - k << std::endl;
            //std::cout << "column_rejection("<<i<<","<<j<<","<<k<<","<<row<<","<<column<<") = " << prob << std::endl;
        }
        
        return prob;
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    // ********************** WITH ZEROS BELOW *****************************
    
    
    
    
    
    
    
    
    template<typename EntryType=unsigned int, typename SumType=EntryType, typename Vector = std::vector<SumType>,typename Table = numerical_table<EntryType,SumType> >
    class binary_contingency_table_with_zeros : public  dsl::finite_sequence<dsl::store::bidirectional, binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>, Table> {
        
        typedef dsl::finite_sequence<dsl::store::bidirectional, dsl::binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>,Table> finite;
        
        
        friend typename dsl::finite_sequence<dsl::store::bidirectional, binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>,Table>::iterator;
        
    public:
        binary_contingency_table_with_zeros(Vector initial_row_sums, Vector initial_column_sums, Table initial_zeros) :
        row_sums(initial_row_sums), column_sums(initial_column_sums), zeros(initial_zeros) { };
        
        
        bool violates_zeros(const Table& t) const {
            
            //std::cout << "zeros = \n" << zeros << std::endl;
            //std::cout << "t = \n" << t << std::endl;
            
            auto it = zeros.cbegin();
            return !(std::find_if(t.cbegin(), t.cend(), [&](EntryType a) { return (*it++ != 0) && (a != 0); }) == t.cend());
        }
        
        bool violates_columns(const Table& t) const {
            
            auto n = column_sums.size();
            auto cit = column_sums.cbegin();
            
            for(size_t j=0;j<n;++j){
                
                auto sum = std::accumulate(t.cbegin_column(j), t.cend_column(j), static_cast<SumType>(0));
                if(sum > *cit) return true;
                ++cit;
            }
            return false;
        }
        
    
        bool first_in_sequence(Table& t) const {
            
            auto m = row_sums.size();
            auto n = column_sums.size();
            
            t = Table(m,n);
            
            auto rit = std::begin(row_sums);
            
            for(size_t i=0;i<m;++i) {
                auto row = dsl::binary_row(n,*rit++);
                
                std::copy_n(row.begin(), n, t.begin_row(i));
            }
            
            /*
            
            auto rit = std::begin(row_sums);
            
            for(size_t i=0;i<m;++i) {
                auto row = dsl::binary_row(n,*rit++);
                //std::cout << "roW = \n" << row << std::endl;
                auto zit = zeros.cbegin_row(i);
                
                std::copy_n(row.begin(), n, t.begin_row(i));
                
                while(std::find_if(t.cbegin_row(i), t.cend_row(i), [&](EntryType a) { return (*zit++ != 0) && (a != 0); }) != t.cend_row(i) || violates_columns(t)) {
                    std::next_permutation(t.begin_row(i), t.end_row(i), std::greater<SumType>());
                    zit = zeros.cbegin_row(i);

                    //std::cout << "W = \n" << zeros << std::endl;
                    std::cout << "t = \n" << t << std::endl;
                }
             
                
            }
            */
            
            
            //std::cout << "W = \n" << zeros << std::endl;
            //std::cout << "t = \n" << t << std::endl;
            
            size_t current_row = m-1;
            
            //bool flag1 ; //= t.template column_sums<Vector>() != column_sums;
            //bool flag2 ; //= violates_zeros(t);
            
            while(t.template column_sums<Vector>() != column_sums || violates_zeros(t) ) {
                
                while(current_row > 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    current_row--;
                }
                
                if(current_row == 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    //std::cout << "No possible tables exist!" << std::endl;
                    return false;
                }
                
                current_row = m-1;
                
                //std::cout << "t = \n" << t << std::endl;
                //flag1 = t.template column_sums<Vector>() != column_sums;
                //if(!flag1){
                    //std::cout << "t = \n" << t << std::endl;
                    //std::cout << "W = \n" << zeros << std::endl;
                    
                //    flag2 = violates_zeros(t);
                //}
                
                
                //std::cout << "t = \n" << t << std::endl;
                
            }
            
            return true;
            
        }
    
    
        
        bool next_in_sequence(Table& t) const {
            
            auto m = row_sums.size();
            auto n = column_sums.size();
            
            size_t current_row = m-1;
            
            //bool flag1;// = t.template column_sums<Vector>() != column_sums;
            //bool flag2;// = violates_zeros(t);
            
            do {
                
                /*flag1 = t.template column_sums<Vector>() != column_sums;
                if(!flag1) {
                    std::cout << "t = \n" << t << std::endl;
                    std::cout << "W = \n" << zeros << std::endl;
                    flag2 = violates_zeros(t);
                }*/
                while(current_row > 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    current_row--;
                }
                
                if(current_row == 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
                    // t = first_in_sequence();
                    return false;
                }
                current_row = m-1;
                
                
            } while(t.template column_sums<Vector>() != column_sums || violates_zeros(t) );
            
            return true;
            
        }
        
        
        
        
        template<typename Parameters = std::tuple<Vector,Vector,Table> >
        class object : public Table {
        public:
            
            Table as_table() { return *this; }
            
            object(Vector row_sums, Vector column_sums, Table zeros) : numerical_table<EntryType,SumType>(row_sums.size(), column_sums.size()), p(row_sums, column_sums, zeros) {
                
                //std::get<0>(p) = row_sums;
                //std::get<1>(p) = column_sums;
                //std::get<2>(p) = zeros;
                
                //p = std::make_tuple<Vector,Vector,Table>(row_sums, column_sums, zeros);
                
            }

            
            object(Vector row_sums, Vector column_sums, Table zeros, Table completed) : numerical_table<EntryType,SumType>(SumType(row_sums.size()), SumType(column_sums.size()),EntryType(0)), p(row_sums, column_sums, zeros) {
                
                std::copy(completed.begin(), completed.end(), this->begin());
                
                //std::cout << completed << std::endl;
                //std::cout << *this << std::endl;
                
                //std::get<0>(p) = row_sums;
                //std::get<1>(p) = column_sums;
                //std::get<2>(p) = zeros;
                
                //p = std::make_tuple<Vector,Vector,Table>(row_sums, column_sums, zeros);
                
            }

            
            
            bool valid = false;
            
        private:
            Parameters p;
        };
        
        template<typename Parameters = std::tuple<Vector,Vector,Table> >
        class generator {
        public:
            
            generator(Parameters initial_p) : p(initial_p) { };
            
            template<typename URNG=std::mt19937_64>
            typename binary_contingency_table_with_zeros::object<Parameters> operator()(URNG& gen = generator_64, double verbose=0);
            
            
            
        private:
            Parameters p;
            
            std::tuple<Vector,Vector,Table,Table,bool> deterministic_fill(Vector row, Vector column, Table Zeros,double verbose = 0);
            std::tuple<EntryType,EntryType> next_index(const Vector& row, const Vector& column, const Table& zeros);
            long double rejection_function(EntryType i, EntryType j, const Table& A, Vector row, Vector column, Table zeros);
            long double rejection_function(EntryType i, EntryType j, EntryType k, const Vector& row, const Vector& column, const Table& zeros);
            long double row_rejection(EntryType i, EntryType j, EntryType k, const Vector& row, const Vector& column, const Table& zeros);
            long double column_rejection(EntryType i, EntryType j, EntryType k, const Vector& row, const Vector& column, const Table& zeros);
            
            template<typename VectorOfFloats = std::vector<long double>>
            void update_q(VectorOfFloats& q);
            
            
            void print_parameters(const Parameters& p_local, std::ostream& output = std::cout ) {
                output << "r = "   << std::get<0>(p_local) << std::endl;
                output << "c = "   << std::get<1>(p_local) << std::endl;
                output << "W = \n" << std::get<2>(p_local) << std::endl;
            }
            
            template<typename Tuple>
            void print_rcAWerr(const Tuple& p_local, std::ostream& output = std::cout) {
                output << "r = "     << std::get<0>(p_local) << std::endl;
                output << "c = "     << std::get<1>(p_local) << std::endl;
                output << "A = \n"   << std::get<2>(p_local) << std::endl;
                output << "W = \n"   << std::get<3>(p_local) << std::endl;
                output << "error = " << std::get<4>(p_local) << std::endl;
            }
            
            void print_rcAW(const Vector& r, const Vector& c, const Table& A, const Table& W, std::ostream& output = std::cout) {
                output << "r = "   << r << std::endl;
                output << "c = "   << c << std::endl;
                output << "A = \n" << A << std::endl;
                output << "W = \n" << W << std::endl;
            }

            
        };
        
        
        void reset_parameters_to(Vector replacement_row_sums, Vector replacement_column_sums) {
            row_sums = replacement_row_sums;
            column_sums = replacement_column_sums;
            
            // check if valid.
            is_emptyset = Satisfies_GaleRyser_Condition();
        }
        
        bool is_emptyset;
        
        
        template<typename Parameters = std::tuple<Vector,Vector,Table>, typename URNG=std::mt19937_64>
        object<Parameters> random(URNG& gen = generator_64, double verbose=0) {
            generator<Parameters> generate(std::make_tuple(row_sums, column_sums,zeros));
            return generate(gen,verbose);
        }
        
    private:
        Vector row_sums;
        Vector column_sums;
        Table zeros;
        
        bool Satisfies_GaleRyser_Condition() {
            // Check the Gale-Ryser condition
        }
    };
    
    
    template<
    typename EntryType=unsigned int, typename SumType=EntryType,
    typename Vector = std::vector<SumType>,
    typename Table=numerical_table<EntryType,SumType>,
    typename Parameters=std::tuple<Vector,Vector,Table>,
    typename URNG=std::mt19937_64 >
    typename binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::template object<Parameters> random_binary_contingency_table_with_zeros(Vector row_sums, Vector column_sums, Table Zeros, URNG& gen=generator_64, double verbose = 0)  {
        
        //typename binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::template object<Parameters> bct(row_sums, column_sums, Zeros);
        binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table> bct(row_sums, column_sums, Zeros);
        
        unsigned int counts = 0;
        unsigned int max_iterations = 100000;
        
        auto obj = bct.random(gen,verbose);
        
        while(!obj.valid && counts < max_iterations){
            obj = bct.random(gen,verbose);
            ++counts;
        }
        
        if(counts == max_iterations)
            std::cout << "Warning!  Maximum iterations " << max_iterations << " reached."<<std::endl;
        
        return obj;
    }
    
    template<
    typename EntryType=unsigned int, typename SumType=EntryType,
    typename Vector = std::vector<SumType>,
    typename Table=numerical_table<EntryType,SumType>,
    typename Parameters=std::tuple<Vector,Vector,Table>,
    typename URNG=std::mt19937_64 >
    void random_binary_contingency_table_with_zeros_for_thread(typename binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::template object<Parameters>& obj, Vector row_sums, Vector column_sums, Table Zeros, URNG& gen=generator_64, double verbose = 0)  {
        
        //typename binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::template object<Parameters> bct(row_sums, column_sums, Zeros);
        binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table> bct(row_sums, column_sums, Zeros);
        
        unsigned int counts = 0;
        unsigned int max_iterations = 100000;
        
        obj = bct.random(gen,verbose);
        
        while(!obj.valid && counts < max_iterations){
            obj = bct.random(gen,verbose);
            ++counts;
        }
        
        if(counts == max_iterations)
            std::cout << "Warning!  Maximum iterations " << max_iterations << " reached."<<std::endl;
        
        return;
    }
    
    
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    template<typename VectorOfFloats>
    void binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::update_q(VectorOfFloats& q) {
        
        // Only need to get the current set of values, no need for make a copy.
        const auto& c = std::get<1>(p);
        const auto& W = std::get<2>(p);

        auto n = c.size();
        auto m = std::get<0>(p).size();
        
        auto c_it = c.begin();
        auto q_it = q.begin();
        
        long double cj_as_float = 0;
        SumType total = 0;
        
        for(size_t j=0; j<n; ++j) {
            
            if(*c_it == 0)
                *q_it = 1.;
            else {
                
                cj_as_float = *c_it;
                total = std::accumulate(W.cbegin_column((int)(j)), W.cend_column((int)(j)), static_cast<SumType>(0));
                
                //*q_it = cj_as_float / (cj_as_float + m - total);
                *q_it = cj_as_float / (m - total);
            }
            ++c_it;
            ++q_it;
        }
        return;
    }
    
    
    /** Implements the PDC algorithm using binary contingency table with forced zeros. */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    template<typename URNG>
    typename binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::template object<Parameters>
    binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::operator()(URNG& gen, double verbose) {
        
        //binary_contingency_table_with_zeros bct(std::get<0>(p), std::get<1>(p), std::get<2>(p));
        
        //EntryType zero = static_cast<EntryType>(0);
        EntryType one = static_cast<EntryType>(1);
        
        auto r = std::get<0>(p);
        auto c = std::get<1>(p);
        auto W = std::get<2>(p);
        
        level++;
        //std::cout << "level up: " << level << std::endl;
        //std::cout << "W level = " << std::accumulate(W.begin_row(0), W.end_row(0), 0) << std::endl;
        
        
        auto m = r.size();
        auto n = c.size();
        
        Table A(m,n,EntryType(0));
        
        // valid until proven false!
        bool valid_flag = true;
        
        
        if(verbose >=1) {
            std::cout << "Initial input values: " << std::endl;
            print_rcAW(r,c,A,W);
        }
        
        // Check for deterministic filling of entries
        auto vars = this->deterministic_fill(r,c,W);
        
        if(verbose >=1) {
            std::cout << "After deterministic fill" << std::endl;
            print_rcAWerr(vars);
        }
        
        Table ones(m,n,static_cast<EntryType>(1));
        
        bool error = std::get<4>(vars);
        
        if(error) {
            std::cout << "Initial deterministic fill failed to yield a completable table." << std::endl;
        }
        else {
        
            r = std::get<0>(vars);
            c = std::get<1>(vars);
            A = std::get<2>(vars);
            W = std::get<3>(vars);
            
            // vector of probabilities
            std::vector<long double> q(n);
            SumType total = 0;
            long double cj_as_float;
            
            while(W != ones) {
                
                // get vector of probabilities
                update_q(q);
                
                // get next (i,j) coordinate
                auto indices = next_index(r,c,W);
                auto& i = std::get<0>(indices);
                auto& j = std::get<1>(indices);
                
                if(verbose >=2) std::cout << "Sampling next at index (i,j) = (" << std::get<0>(indices) << "," << std::get<1>(indices) << ")" << std::endl;
                

                // Whether it is 0 or 1, it is no longer in play after this iteration.
                W(i,j) = one;
                
                if(verbose >=2) std::cout << "Testing bit to be 0" << std::endl;
                
                // Test for deterministic fill error if the bit is 0.
                auto vars0 = this->deterministic_fill(r,c,W);
                
                if(verbose >=2){
                    //std::cout << "Testing bit to be 0" << std::endl;
                    std::get<4>(vars0)? std::cout << "cannot be 0" : std::cout << "can be 0"; std::cout << std::endl;
                    
                    print_rcAW(std::get<0>(vars0),std::get<1>(vars0),A+std::get<2>(vars0),std::get<3>(vars0));
                 /* std::cout << "r = " << std::get<0>(vars0) << std::endl;
                    std::cout << "c = " << std::get<1>(vars0) << std::endl;
                    std::cout << "A = \n" << A+std::get<2>(vars0) << std::endl;
                    std::cout << "W = \n" << std::get<3>(vars0) << std::endl;
                 */
                }
                
                
                // Test for deterministic fill error if the bit is 1.
                
                // subtract 1 from the row and column for which the bit at A[i,j] is set to 1.
                Vector r1 = r; auto rit1 = r1.begin(); std::advance(rit1, i); --*rit1;
                Vector c1 = c; auto cit1 = c1.begin(); std::advance(cit1, j); --*cit1;
                
                if(verbose >=2) std::cout << "Testing bit to be 1" << std::endl;
                
                auto vars1 = this->deterministic_fill(r1,c1,W);
                
                if(verbose >=2){
                    //std::cout << "Testing bit to be 1" << std::endl;
                    std::get<4>(vars1)? std::cout << "cannot be 1" : std::cout << "can be 1"; std::cout << std::endl;
                    auto temp = A(i,j);
                    A(i,j) = one;
                    print_rcAW(std::get<0>(vars1), std::get<1>(vars1), A+std::get<2>(vars1), std::get<3>(vars1));
                 
                    /* std::cout << "r = " << std::get<0>(vars1) << std::endl;
                    std::cout << "c = " << std::get<1>(vars1) << std::endl;
                    std::cout << "A = \n" << A+std::get<2>(vars1) << std::endl;
                    std::cout << "W = \n" << std::get<3>(vars1) << std::endl;
                     */
                    
                    A(i,j) = temp;
                }
                
                auto error0 = std::get<4>(vars0);
                auto error1 = std::get<4>(vars1);
                
                if(error0 && error1) {
                    if(verbose >=2) {
                        std::cout << "No longer completable at entry: (i,j) = (" <<i << "," << j<<")"<<std::endl;
                        print_rcAW(std::get<0>(vars0),std::get<1>(vars0),A+std::get<2>(vars0),std::get<3>(vars0));
                    }
                } else if(error0) {
                    if(verbose>=2) std::cout << "Bit can only be 1" << std::endl;
                    A(i,j) = one;
                    A += std::get<2>(vars1);
                    r = std::get<0>(vars1);
                    c = std::get<1>(vars1);
                    W = std::get<3>(vars1);
                } else if(error1) {
                    if(verbose>=2) std::cout << "Bit can only be 0" << std::endl;
                    A += std::get<2>(vars0);
                    r = std::get<0>(vars0);
                    c = std::get<1>(vars0);
                    W = std::get<3>(vars0);
                } else {
                    
                    
                    if(verbose>=2) std::cout << "Bit can be either" << std::endl;
                    
                    // Calculate rejection probabilities.
                    std::pair<long double, long double> outcomes {1.,1.};
                    
                    // outcomes.first = rejection_function(i,j,0,r,c,W);
                    // outcomes.second = rejection_function(i,j,1,r,c,W);
                    
                    // This is where I need to modify.  It has to take deterministic second half into account.
                    auto& x = std::get<2>(vars0);
                    
                    for(size_t i=0;i<m;++i)
                        for(size_t j=0;j<n;++j)
                            if(x(i,j) == one)
                                outcomes.first *= rejection_function(i,j,0,r,c,W);

                    auto& y = std::get<2>(vars1);
                    
                    // ***** Need to rewrite rejection_function so that it accepts a matrix of values with 1s where the rejection should be applied. *****
                    
                    for(size_t i=0;i<m;++i)
                        for(size_t j=0;j<n;++j)
                            if(y(i,j) == one)
                                outcomes.second *= rejection_function(i,j,1,r,c,W);
                    
                    
                    
                    if(verbose >=2) {
                        std::cout << "probs = " << outcomes << std::endl;
                    }
                    
                    if(verbose >= 3) std::cout << "At index (i,j) = (" << i << "," << j << ") probabilities = (" << outcomes.first << "," << outcomes.second << ")" << std::endl;
                    
                    long double tol = std::pow<long double>(10.,-15.);
                    
                    bool repeat_flag = true;
                    
                    if(outcomes.first < tol && outcomes.second >= tol) {
                        A(i,j) = one;
                        repeat_flag = false;
                        A += std::get<2>(vars1);
                        r = std::get<0>(vars1);
                        c = std::get<1>(vars1);
                        W = std::get<3>(vars1);
                    } else if(outcomes.first >= tol && outcomes.second < tol) {
                        //A(i,j) = 0;
                        repeat_flag = false;
                        A += std::get<2>(vars0);
                        r = std::get<0>(vars0);
                        c = std::get<1>(vars0);
                        W = std::get<3>(vars0);
                    } else if(outcomes.first < tol && outcomes.second < tol) {
                        if(verbose >= 2) std::cout << "WARNING!  Both rejection probabilities less than tolerance ... leaving in 0, possible undesireable consequences." << std::endl;
                        A += std::get<2>(vars0);
                        r = std::get<0>(vars0);
                        c = std::get<1>(vars0);
                        W = std::get<3>(vars0);
                    } else {
                        
                        // both rejection functions of reasonable size, normalize.
                        
                        auto norm = std::max(outcomes.first, outcomes.second);
                        
                        outcomes.first  /= norm;
                        outcomes.second /= norm;
                        
                        if(verbose >=2) {
                            std::cout << "probs scaled = " << outcomes << std::endl;
                        }

                        
                        long double threshold;
                        
                        auto q_it = q.begin(); std::advance(q_it, j);
                        
                        long double qj = static_cast<long double>(*q_it);
                        
                        std::bernoulli_distribution bern(qj / (1.+qj));
                        std::uniform_real_distribution<long double> u(0.,1.);
                        
                        bool b;
                        
                        while(repeat_flag) {
                            
                            repeat_flag = false;
                            
                            b = bern(gen);
                            
                            if(verbose >=2) std::cout << "Random bit is " << b << std::endl;
                            
                            threshold = (b == 0 ? outcomes.first : outcomes.second);
                            
                            if(u(gen) <= threshold ) {
                                repeat_flag = false;
                                if(verbose >=3) std::cout << "A("<<i<<","<<j<<") = " << b << " accepted." << std::endl;
                                if(b == 0) {
                                    A += std::get<2>(vars0);
                                    r = std::get<0>(vars0);
                                    c = std::get<1>(vars0);
                                    W = std::get<3>(vars0);
                                }
                                else {
                                    A(i,j) = 1;
                                    A += std::get<2>(vars1);
                                    r = std::get<0>(vars1);
                                    c = std::get<1>(vars1);
                                    W = std::get<3>(vars1);
                                }
                                    
                            }
                            else {
                                repeat_flag = true;
                                if(verbose >=3) std::cout << "A("<<i<<","<<j<<") = " << b << " rejected." << std::endl;
                            }
                        }
                        
                    }
                }
                
                //if(A(i,j) == one) {
                //    auto rit = r.begin(); std::advance(rit, i); *rit = *rit - A(i,j);
                //    auto cit = c.begin(); std::advance(cit, j); *cit = *cit - A(i,j);
                //}
                
                if(verbose >= 1) {
                    std::cout << "r = " << r << std::endl;
                    std::cout << "c = " << c << std::endl;
                    std::cout << "A = \n" << A << std::endl;
                    std::cout << "W = \n" << W << std::endl;
                }
                
            }
            
            if(verbose >= 1) {
                std::cout << "r = " << r << std::endl;
                std::cout << "c = " << c << std::endl;
                std::cout << "A = \n" << A << std::endl;
                std::cout << "W = \n" << W << std::endl;
            }
            
            
            
            
            if(std::accumulate(r.cbegin(), r.cend(), static_cast<SumType>(0)) != std::accumulate(c.cbegin(), c.cend(), static_cast<SumType>(0))) {
                std::cout << "ERROR: Not all row sums and column sums are zero" << std::endl;
                valid_flag = false;
            }
            
            
            
            
            auto rit = std::get<0>(p).begin();
            
            for(size_t i=0; i<m; ++i) {
                auto row_sum = std::accumulate(A.begin_row(i), A.end_row(i),static_cast<SumType>(0));
                if(*rit != row_sum){
                    if(verbose >=2)
                        std::cout << "Error row " << i+1 << " sum: " << row_sum << " does not match " << *rit << std::endl;
                    valid_flag = false;
                }
                ++rit;
            }

            auto cit = std::get<1>(p).begin();
            for(size_t j=0; j<n; ++j) {
                auto column_sum = std::accumulate(A.begin_column(j), A.end_column(j),static_cast<SumType>(0));
                if(*cit != column_sum){
                    if(verbose >=2)
                        std::cout << "Error column " << j+1 << " sum: " << column_sum << " does not match " << *cit << std::endl;
                    valid_flag = false;
                }
                ++cit;
            }
    
        }
        
        binary_contingency_table_with_zeros::object<Parameters> obj(std::get<0>(p),std::get<1>(p),std::get<2>(p), A);

        obj.valid = valid_flag;
        
        level--;
        //std::cout << "level down: " << level << std::endl;

        
        return obj;
        
    }
    
    
    
    /** Implements the deterministic fill algorithm, which checks to see if any entries can be filled in deterministically.
     
     Assumptions:
     1.  zeros is not all 1s. 
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    std::tuple<EntryType,EntryType> binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::next_index(const Vector& row, const Vector& column, const Table& zeros) {
        
        auto m = row.size();
        auto n = column.size();
        
        //auto Wit = zeros.cbegin();
        
        auto rit = row.cbegin();
        auto cit = column.cbegin();
        
        std::tuple<EntryType,EntryType,EntryType> min_indices;
        
        // could possibly be zero, but fine.
        if(*zeros.cbegin() == 0)
            min_indices = std::make_tuple(0,0,std::min(*rit, *cit));
        else
            min_indices = std::make_tuple(0,0,0);
        
        for(size_t i=0; i<m; ++i) {
            cit = column.begin();
            auto Wit = zeros.cbegin_row(i);
            for(size_t j=0; j<n; ++j) {
                if(*Wit == 0) {

                    auto local_min = std::min(*rit, *cit);
                    
                    if( std::get<2>(min_indices) < local_min) {
                        
                        //std::get<0>(min_indices) = *rit;
                        //std::get<1>(min_indices) = *cit;
                        //std::get<2>(min_indices) = local_min;
                        min_indices = std::make_tuple(i,j,local_min);
                    }
                }
                ++cit;
                ++Wit;
            };
            ++rit;
        }
        
        return std::make_pair(std::get<0>(min_indices), std::get<1>(min_indices));
    }
    
    
    
    /** Implements the deterministic fill algorithm, which checks to see if any entries can be filled in deterministically. */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    std::tuple<Vector,Vector,Table,Table,bool> binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::deterministic_fill(Vector row, Vector column, Table Zeros, double verbose) {
        
        auto m = row.size();
        auto n = column.size();
        
        bool errorflag=false;
        bool repeat = true;
        
        EntryType zero = static_cast<EntryType>(0);
        EntryType one  = static_cast<EntryType>(1);
        
        numerical_table<EntryType,SumType> B(m,n,EntryType(0));
        
        // As long as Zeros or the table
        while(repeat) {
            repeat = false;
        
            auto row_it = row.begin();
            
            // Check for rows
            for(size_t i=0;i<m;++i) {
                
                
                auto total = std::accumulate(Zeros.begin_row(i), Zeros.end_row(i),static_cast<SumType>(0));
                auto row_sum = *row_it;
                
                //std::cout << "Checking row " << i+1 << " with Total[W[["<<i<<"]]]="<<total<<std::endl;
                
                if(row_sum > n - total) {
                    errorflag = true;
                    
                    if(verbose >= 1)
                        std::cout << "Error row sum of "<<i+1<<": "<<row_sum << " is larger than " << n-total << "entries remaining." << std::endl;
                    return std::make_tuple(row,column,B,Zeros,errorflag);
                }
                
                // If row sum is 0 and W is not all 1s in that row, make it so.
                if(row_sum == 0 && total  < n) {
                    repeat = true;
                    
                    for(auto& x : Zeros.row(i))
                        x = one;
                    
                }
                
                // if a row is uniquely determined by the constraints
                if(row_sum > 0 && total == n-row_sum) {
                    
                    repeat = true;
                    
                    auto table_row = Zeros.begin_row(i);
                    auto B_row = B.begin_row(i);
                    auto column_it = column.begin();
                    
                    for(size_t j=0;j<n;++j) {
                        
                        if(*table_row != one) {
                            *B_row = one;
                            --(*row_it);
                            --(*column_it);
                            *table_row = one;
                        }
                        ++column_it;
                        ++B_row;
                        ++table_row;
                        
                    }
                }
                
                ++row_it;
                
            }
            
            
            /*std::cout << "r = " << row << std::endl;
            std::cout << "c = " << column << std::endl;
            std::cout << "A = \n" << B << std::endl;
            std::cout << "W = \n" << Zeros << std::endl;
            */
            
            auto column_it = column.begin();
            
            // Check for rows
            for(size_t j=0;j<n;++j) {
                
                
                auto total = std::accumulate(Zeros.begin_column(j), Zeros.end_column(j),static_cast<SumType>(0));
                auto column_sum = *column_it;
                
                //std::cout << "Checking column " << j+1 << " with Total[W[["<<j<<"]]]="<<total<<std::endl;

                
                if(column_sum > n - total) {
                    if(verbose >= 1)
                        std::cout << "Error column sum of "<<j+1<<": "<<column_sum << " is larger than " << n-total << "entries remaining."<< std::endl;
                    errorflag = true;
                    return std::make_tuple(row,column,B,Zeros,errorflag);
                }
                
                if(column_sum == 0 && total  < n) {
                    
                    repeat = true;
                    
                    for(auto& x : Zeros.column(j))
                        x = one;
                }
                
                if(column_sum > 0 && total == n-column_sum) {
                    
                    repeat = true;
                    
                    auto table_column = Zeros.begin_column(j);
                    auto B_column = B.begin_column(j);
                    auto row_it = row.begin();
                    
                    for(size_t i=0;i<m;++i) {
                        
                        if(*table_column != one) {
                            *B_column = one;
                            --(*row_it);
                            --(*column_it);
                            *table_column = one;
                        }
                        ++row_it;
                        ++B_column;
                        ++table_column;
                        
                    }
                }
                
                /*std::cout << "r = " << row << std::endl;
                std::cout << "c = " << column << std::endl;
                std::cout << "A = \n" << B << std::endl;
                std::cout << "W = \n" << Zeros << std::endl;
                */
                
                ++column_it;
                
            }
            
        }

        //std::cout << "DONE." << std::endl;
        
        return std::make_tuple(row,column,B,Zeros,errorflag);
        
    }
    
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::rejection_function(EntryType i, EntryType j, const Table& A, Vector row, Vector column, Table zeros) {
        
        auto m = row.size();
        auto n = column.size();
        
        auto rit = std::begin(row);
        auto cit = std::begin(column);
        
        long double prob = 1.;
        
        // we use set because if any row or column has a 1, we simply calculate the rejection function once
        std::set<size_t> row_indices;
        std::set<size_t> column_indices;
        
        std::vector<long double> q(n,0.);
        long double cj = *cit;
        
        // find the q-vector
        for(size_t j=0;j<n;++j) {
        
            // if cj > 0, calculate probability based on number of nonzero column entries
            if(*cit > 0) {
                auto total = std::accumulate(zeros.cbegin_column(j), zeros.cend_column(j), static_cast<SumType>(0));
                
                cj = *cit;
                q[j] = cj / (cj + m - total);
                
            }
            else
                q[j] = 1.;
            
            ++cit;
        }
        
        // Figure out which entires of the table are needed to compute rejection
        for(size_t i=0;i<m;++i) {
            for(size_t j=0;j<n;++j) {
                
                // if a 1 appears in A, update row and column sums
                if(A(i,j) == 1) {
                    if(*cit > 0 && *rit > 0) {
                        --(*cit);
                        --(*rit);
                        row_indices.insert(i);
                        column_indices.insert(j);
                        zeros(i,j) = 1;
                        prob *= q[j];
                    }
                    else { // for some reason, a 1 was put in A which would violate the restrictions
                        std::cout << "ERROR in rejection_function" << std::endl;
                    }
                }
                
                ++cit;
            }
            ++rit;
        }
        
        SumType zero = 0;
        SumType one = 1;
        
        long double eps = 10E-14;

        // For each column for which a bit was determined
        for(auto& j : column_indices) {
            
            SumType total = std::accumulate(zeros.cbegin_column(j), zeros.cend_column(j), zero);
            
            // not sure which set of j are included, so just do it manually each time.  Ok for random access vectors
            cit = column.cbegin();
            std::advance(cit,j);
            
            long double cj = *cit;
            long double p = cj/(cj+m-total);
            
            // If the column sum is nonzero, calculate probability, otherwise assume it is 1.
            if(q[j] < 1-eps)
                prob *= std::exp(std::lgamma(m-total+1.)-std::lgamma(cj+1)-std::lgamma(m-total-cj+1)+cj*log(1.-q[j])+(m-total-cj)*log(q[j]));
            
        }
        
        
        // For each row for which a bit was determined
        for(auto& i : row_indices) {
            
            // initialize row iterator to row i
            rit = row.cbegin(); std::advance(rit,i);
            
            poisson_binomial<std::vector<long double>> pb(q);
            
            prob *= pb.pdf(*rit);
            
        }
        
        return prob;
    }
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::rejection_function(EntryType i, EntryType j, EntryType k, const Vector& row, const Vector& column, const Table& zeros) {
        
        //return row_rejection(i,j,k,row, column, zeros)*column_rejection(i,j,k,row,column,zeros) > 0 ? 1.-(1./(i+j+k+1.01)) : 0;
        return row_rejection(i,j,k,row, column, zeros)*column_rejection(i,j,k,row,column,zeros); // > 0 ? 1.-(1./(i+j+k+1.01)) : 0;
        
    }
    

    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::row_rejection(EntryType i, EntryType j, EntryType k, const Vector& row, const Vector& column, const Table& zeros) {
        
        auto n = column.size();
        auto m = row.size();
        
        // only make q as long as the number of non-zero column sums.  Start with full size then resize later.
        std::vector<long double> q(n,0.);
        
        auto cit = column.begin();
        long double cj = *cit;
        
        // Since we won't use all n elements, this will index the vector q
        size_t index = 0;
        
        for(size_t j=0;j<n;++j) {
            
            
            
            cj = *cit;

            
            if(cj >0) {
                auto total = std::accumulate(zeros.cbegin_column(j), zeros.cend_column(j), static_cast<SumType>(0));
            
                q[index++] = cj / (cj + m - total);
            }
            
            ++cit;
        }
        
        q.resize(index);
        
        auto rit = row.cbegin(); std::advance(rit,i);
        
        poisson_binomial<std::vector<long double>> pb(q);
        
        long double prob = pb.pdf(*rit-k);
        
        //std::cout << "evaluate row distr at " << *rit << "-" << k << " = " << *rit - k << std::endl;
        //std::cout << "row_rejection("<<i<<","<<j<<","<<k<<","<<row<<","<<column<<") = " << prob << std::endl;
        
        return prob;
        
        
    }
    
    /** calculates the rejection probability via a product of row rejection and column rejection.
     
     Assumptions:
     1.  zeros is not all 1s.
     
     */
    template<typename EntryType, typename SumType, typename Vector, typename Table>
    template<typename Parameters>
    long double binary_contingency_table_with_zeros<EntryType,SumType,Vector,Table>::generator<Parameters>::column_rejection(EntryType i, EntryType j, EntryType k, const Vector& row, const Vector& column, const Table& zeros) {
        
        unsigned int m = row.size();
        SumType total = std::accumulate(zeros.cbegin_column(j), zeros.cend_column(j), static_cast<SumType>(0));
        
        auto cit = column.cbegin();
        std::advance(cit,j);
        
        long double cj = *cit;
        long double p = cj/(cj+m-total);

        unsigned int cprime = cj-k;
        
        long double prob = std::exp(std::lgamma(m-total+1.)-std::lgamma(cprime+1)-std::lgamma(m-total-cprime+1)+cprime*log(p)+(m-total-cprime)*log(1.-p));
        
        //std::cout << "evaluate column distr at " << cj << "-" << k << " = " << cj - k << std::endl;
        //std::cout << "column_rejection("<<i<<","<<j<<","<<k<<","<<row<<","<<column<<") = " << prob << std::endl;
        
        
        return prob;

    }

}



#endif







#if 0

// too slow to find the first one.
bool first_in_sequence(Table& t) const {
    
    auto m = row_sums.size();
    auto n = column_sums.size();
    
    t = Table(m,n);
    
    auto rit = std::begin(row_sums);
    
    for(size_t i=0;i<m;++i) {
        auto row = dsl::binary_row(n,*rit++);
        
        std::copy_n(row.begin(), n, t.begin_row(i));
    }
    
    //std::cout << "t = \n" << t << std::endl;
    
    size_t current_row = m-1;
    while(t.template column_sums<Vector>() != column_sums) {
        
        while(current_row > 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
            current_row--;
        }
        
        if(current_row == 0 && !std::next_permutation(t.begin_row(current_row), t.end_row(current_row), std::greater<SumType>())) {
            //std::cout << "No possible tables exist!" << std::endl;
            return false; //Table(m,n);
        }
        
        current_row = m-1;
        
        //std::cout << "t = \n" << t << std::endl;
        
        
    }
    
    return true;
    
}

#endif
