//
//  DeSalvoMatrix.h
//  DeSalvoLibraryTestKit
//
//  Created by Stephen DeSalvo on 2/16/15.
//  Copyright (c) 2015 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvoLibraryTestKit_DeSalvoMatrix_h
#define DeSalvoLibraryTestKit_DeSalvoMatrix_h





#include <iostream>
#include <memory>
#include <numeric>
#include <initializer_list>

#include <vector>
#include <array>

#include "numerical.h"
#include "table.h"

namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    // See DeSalvoStandardLibraryDocumentation.h
    template<typename ValueType=double, typename WorkingPrecision=long double>
    class matrix : public table<ValueType, WorkingPrecision> {
        
        friend std::ostream& operator<<(std::ostream& out, const matrix& t) {
            ValueType* v = t.entry;
            out<<"{";
            for(size_t i=0;i<t.m-1;++i) {
                out<<"{"<<*v++;
                for(size_t j=0;j<t.n-1;++j)
                    out<<","<<*v++;
                out<<"},"<<std::endl;
            }
            
            out<<"{"<<*v++;
            for(size_t j=0;j<t.n-1;++j)
                out<<","<<*v++;
            out<<"}}";
            
            return out;
        };
        
    public:

        /** Initializes the "empty" matrix. */
        matrix() : dsl::table<ValueType,WorkingPrecision>(0,0) { };
        
        /** Initialize entries to value
            @param val is the initial value for all entries. */
        matrix(size_t number_of_rows, size_t number_of_columns=1,
               const ValueType& val = ValueType()) : dsl::table<ValueType,WorkingPrecision>(number_of_rows, number_of_columns) { };
        
        matrix(const matrix& initial_matrix) :
        dsl::table<ValueType,WorkingPrecision>(static_cast<dsl::table<ValueType,WorkingPrecision>>(initial_matrix)) {
        }
        
#if 0
        matrix(matrix&& initial_matrix) : m(initial_matrix.m), n(initial_matrix.n), entry(initial_matrix.entry) {
            initial_matrix.entry = nullptr;
            //std::cout << "Move performed" << std::endl;
        }
        
        /** Swap two matrix handles
            @param other matrix to swap. */
        void swap(matrix& other) {
            std::swap(m, other.m);
            std::swap(n, other.n);
            std::swap(this->entry, other.entry);
        }
        
        /** Assigns values via copy & swap idiom
         @param val is the initial value for all entries. */
        matrix& operator=(matrix other) {

            // NOT std::swap, see above
            swap(other);
            return *this;
        }
        
        /** Initialize matrix using notation {{1,2,3},{4,5,6},{7,8,9}}
            @param init is the initializer list. */
        matrix(std::initializer_list<ValueType> init)  {
            
            // Initialize t to first element in contiguous array
            auto t = std::begin(entry);
            
            // Iterators through each row
            auto start = std::begin(init);
            auto stop = std::end(init);
            
            m = stop - start;
            n = (*start).size();
            entry = new ValueType[m*n];
            
            // For each row ...
            while(start != stop) {
                auto st = begin(*start);
                auto sp = end(*start++);
                
                // Copy over values into t
                while(st != sp)
                    *t++ = *st++;
            }
        };
        
        /** Initialize using a row-major std::array of m*n values
            @tparam VType is any type which can be cast to ValueType
            @param init is an array of values to initilize. */
        template<typename VType, size_t m, size_t n>
        matrix(std::array<VType,n*m> init) : entry(new ValueType[n*m])  {
            
            //auto start = std::begin(init);
            //auto stop = std::end(init);
            
            //auto t = std::begin(entry);
            
            //while(start != stop) {
            //    *t++ = static_cast<ValueType>(*start++);
            //}
            
            std::copy_n(std::begin(init), m*n, std::begin(entry));
            
        }
        
        /** Initialize using a vector of vectors
         @tparam VType is any type which can be cast to ValueType
         @param v is an array of values to initilize. */
        template<typename VType>
        matrix(const std::vector<std::vector<VType>>& v) {
            //            matrixIterator tstart = begin();
            m = v.size();
            n = v[0].size();
            entry = new ValueType[n*m];
            
            ValueType* initializer = entry;
            for(size_t i=0;i<m;++i)
                for(size_t j=0;j<n;++j)
                    *initializer++ = static_cast<ValueType>(v[i][j]);
            //std::copy(std::cbegin(v), std::cend(v), entry);
        };
        
        
#endif
        /** virtual destructors deletes entry memory. */
        virtual ~matrix() { }; //delete [] entry; };
        
        
        
        /** Multiplication by a scalar, A = A*c
         @param value is the scalar c
         @returns reference to newly updated matrix B which is still (M x N)
         */
        matrix& operator*=(const ValueType& value) {
            std::for_each(this->begin(),this->end(),[&](ValueType& v) { v*=value; });
            return *this;
        }
        /** Multiplication by a scalar, A = A*c
         @param value is the scalar c
         @returns reference to newly updated matrix B which is still (M x N)
         */
        friend matrix operator*(const ValueType& value, matrix m) {
            return m*=value;
        }
        
        /** Multiplication by a scalar, A = A*c
         @param value is the scalar c
         @returns reference to newly updated matrix B which is still (M x N)
         */
        matrix& operator/=(const ValueType& value) {
            std::for_each(this->begin(),this->end(),[&](ValueType& v) { v/=value; });
            return *this;
        }
        /** Multiplication by a scalar, A = A*c
         @param value is the scalar c
         @returns reference to newly updated matrix B which is still (M x N)
         */
        friend matrix operator/(const ValueType& value, matrix m) {
            return m/=value;
        }

        
        
        
        /** (R x M) * (M x N) multiplication, B = A*B
         @param lefty is the (M x M) matrix A
         @returns reference to newly updated matrix B which is still (M x N)
         */
        friend matrix operator*(const matrix& lhs, const matrix& rhs) {
            
            if(lhs.n != rhs.m)
                throw std::runtime_error{"error dimensions: cannot multiply "+std::to_string(lhs.m)+"x"+std::to_string(lhs.n)+" with "+std::to_string(rhs.m)+"x"+std::to_string(rhs.n)};
            
            matrix clone(lhs.m,rhs.n);
            
            //for(auto& x : dnl::table_indices(rhs.m,rhs.n))
            // For each column j=0,...,n-1 in this matrix
            //for(auto& j : dnl::indices(rhs.n))
            for(int j=0;j<rhs.n;++j)
                // For each row i=0,...,m-1 in this matrix
                //for(auto& i : dnl::indices(lhs.m))
                for(int i=0;i<lhs.m;++i)
                    clone(i,j) = std::inner_product(lhs.cbegin_row(i), lhs.cend_row(i), rhs.cbegin_column(j), static_cast<WorkingPrecision>(0.));
            
            //std::cout << clone << std::endl;
            return clone;
        }
        
        
        /** (R x M) + (R x M) addition, C = A+B
         @param lefty is the (M x M) matrix A
         @returns reference to newly updated matrix B which is still (M x N)
         */
        matrix& operator+=(const matrix& rhs) {
            
            // Check for equal dimensions
            if(this->m != rhs.m || this->n != rhs.n)
                throw std::runtime_error{"error dimensions: cannot add "+std::to_string(this->m)+"x"+std::to_string(this->n)+" with "+std::to_string(rhs.m)+"x"+std::to_string(rhs.n)};
            
            // wish list: std::transform_n command, making my own instead
            dsl::transform_n(this->begin(), this->m*this->n, rhs.cbegin(), this->begin(), std::plus<WorkingPrecision>());
            
            return *this;
        }
        
        /** (R x M) + (R x M) addition, C = A+B
         @param lefty is the (M x M) matrix A
         @returns reference to newly updated matrix B which is still (M x N)
         */
        friend matrix operator+(matrix lhs, const matrix& rhs) {
            return lhs += rhs;
        }
        
        
        /** (R x M) + (R x M) addition, C = A+B
         @param lefty is the (M x M) matrix A
         @returns reference to newly updated matrix B which is still (M x N)
         */
        matrix& operator-=(const matrix& rhs) {
            
            auto& m = this->m;
            auto& n = this->n;
            
            // Check for equal dimensions
            if(m != rhs.m || n != rhs.n)
                throw std::runtime_error{"error dimensions: cannot subtract "+std::to_string(m)+"x"+std::to_string(n)+" with "+std::to_string(rhs.m)+"x"+std::to_string(rhs.n)};
            
            // wish list: std::transform_n command, making my own instead
            dsl::transform_n(this->begin(), m*n, rhs.cbegin(), this->begin(), std::minus<WorkingPrecision>());
            
            return *this;
        }
        
        /** (R x M) + (R x M) addition, C = A+B
         @param lefty is the (M x M) matrix A
         @returns reference to newly updated matrix B which is still (M x N)
         */
        friend matrix operator-(matrix lhs, const matrix& rhs) {
            return lhs -= rhs;
        }
        
        matrix operator-() const {
            matrix clone(*this);
            std::transform(this->cbegin(), this->cend(), clone.begin(), [](const ValueType& v) { return -v; });
            return clone;
        }
        
        matrix operator+() const {
            return *this;
        }
        
        friend bool operator==(const matrix& lhs, const matrix& rhs) {
            // First check dimensions, then do the more expensive check among all elements
            return lhs.m == rhs.m && lhs.n == rhs.n && std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
        }
        
        
        void transpose() {
            dsl::transpose(this->entry, this->entry+this->m*this->n,this->n);
            std::swap(this->m,this->n);
        }
        
        WorkingPrecision power_iteration(size_t max_iters=10000) {
            
            auto& n = this->n;
            auto& m = this->m;
            
            int i=0;
            matrix vector(n,1);
            for(auto& x : vector)
                x = i++;
            vector.normalize_columns_by_lp(2);
            
            vector = (*this)*vector;
            WorkingPrecision eig1 = vector.column_lp_norms()[0];
            WorkingPrecision eig2 = 0.;
            vector.normalize_columns_by_lp(2);
            
            //std::cout<< eig1<<" "<<eig2 <<std::endl;
            
            size_t iterations = 0;
            while(fabs(eig1-eig2) > 1.e-6 && iterations < max_iters) {
                eig2 = eig1;
                vector = (*this)*vector;
                eig1 =  vector.column_lp_norms()[0];
                vector.normalize_columns_by_lp(2);
                ++iterations;
            }
            if(iterations >= max_iters)
                std::cerr<<"Max iterations on power_ieration"<<std::endl;
            
            return eig1;
        }
        

        
        double second_largest_eigenvalue_of_stochastic_square_matrix(size_t max_iters=10000);
        

        
    private:
        //ValueType* entry;
        //size_t m, n;     // Dimension must be nonnegative
    };
    
    template<typename ValueType=double, typename WorkingPrecision=long double>
    bool operator!=(const matrix<ValueType,WorkingPrecision>& lhs, const matrix<ValueType,WorkingPrecision>& rhs) {
        return !(lhs == rhs);
    }

    template<typename ValueType = double, typename WorkingPrecision=long double>
    matrix<ValueType, WorkingPrecision> identity_matrix(size_t n) {
        matrix<ValueType,WorkingPrecision> mat(n,n,0);
        
        for(size_t i=0;i<n;++i)
            mat(i,i) = 1;
        
        return mat;
    }

    template<typename ValueType = double, typename WorkingPrecision = long double>
    matrix<ValueType, WorkingPrecision> all_ones(size_t m, size_t n) {
        return matrix<ValueType,WorkingPrecision>(m,n,1);
    }




    /** Use Wielandt Deflation, Algorithm 9.4 in Burden-Faires 7th Edition to find second largest eigenvalue by constructing a smaller matrix and then applying the power method on it.
     @tparam ValueType is the data type stored in the matrix
     @tparam WorkingPrecision is the type used for numerical calculations
     @param max_iters is the maximum number of iterations for the power method on the smaller matrix
    */
    template<typename ValueType, typename WorkingPrecision>
    double matrix<ValueType, WorkingPrecision>::second_largest_eigenvalue_of_stochastic_square_matrix(size_t max_iters)
    {
        
        auto& n = this->n;
        // Matrix must be square for this to work
        matrix B(n-1,n-1);
        
        // Doesn't work if first row and column are 0s.
        bool flag = true;
        for(int i=0;i<n;++i)
            flag &= ((*this)(i,0)==0);

        for(int j=1;j<n;++j)
            flag &= ((*this)(0,j)==0);
        
        if(flag) this->swap_rows(0,1);
        
        for(int i=0;i<n-1;++i)
            for(int j=0;j<n-1;++j)
                B(i,j) = (*this)(i+1,j+1) - (*this)(0,j+1);
        
        //std::cout << B << std::endl;
        
        return B.power_iteration();
        
    }
    
    


}








#endif
