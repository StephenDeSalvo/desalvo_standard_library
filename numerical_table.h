//
//  numerical_table.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 12/11/16.
//  Copyright © 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef numerical_table_h
#define numerical_table_h

#include "table.h"

namespace desalvo_standard_library {
    
    template<typename ValueType=double, typename WorkingPrecision=ValueType>
    class numerical_table : public table<ValueType, WorkingPrecision> {
    public:
        
        numerical_table(size_t m = 0, size_t n = 0, ValueType val = 0) : table<ValueType,WorkingPrecision>(m,n,val) { };
        
        template<typename Table>
        numerical_table(Table tab) : table<ValueType,WorkingPrecision>(tab) {
        
            
        
        };
        
        //template<typename Iterator>
        //numerical_table(Iterator start, Iterator stop) : table<ValueType,WorkingPrecision>(start, stop) { };
        
        //template<typename InputIterator>
        //numerical_table(size_t number_of_rows, size_t number_of_columns, InputIterator start) : table<ValueType,WorkingPrecision>(number_of_rows, number_of_columns, start) {
        //}
        
        numerical_table& operator+=(const numerical_table& rhs) {
            std::transform(this->begin(), this->end(), rhs.cbegin(), this->begin(), std::plus<WorkingPrecision>());
            return *this;
        }
        
        numerical_table& operator-=(const numerical_table& rhs) {
            std::transform(this->begin(), this->end(), rhs.cbegin(), this->begin(), std::minus<WorkingPrecision>());
            return *this;
        }
        
        template<typename T>
        numerical_table& operator+=(const T& value) {
            for(auto& x : *this) x += value;
            return *this;
        }
        
        template<typename T>
        numerical_table& operator-=(const T& value) {
            for(auto& x : *this) x -= value;
            return *this;
        }
        
        template<typename T>
        numerical_table& operator*=(const T& value) {
            for(auto& x : *this) x *= value;
            return *this;
        }
        
        template<typename T>
        numerical_table& operator/=(const T& value) {
            for(auto& x : *this) x /= value;
            return *this;
        }
        
        numerical_table operator-() {
            auto s = *this;
            for(auto& x : s) x = -x;
            return s;
        }
        
        template<typename ValueTypeLocal=long double, typename WorkingPrecisionLocal=long double>
        numerical_table operator+() {
            numerical_table<ValueTypeLocal,WorkingPrecisionLocal> s(this->r);
            for(auto& x : s) x = -x;
            return s;
        }
        
    private:
        
    };
    
    template<typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator+(
                                                           numerical_table<ValueType, WorkingPrecision>  lhs,
                                                           const numerical_table<ValueType, WorkingPrecision>& rhs) {
        
        return lhs += rhs;
    }
    
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator+(
                                                           numerical_table<ValueType, WorkingPrecision>  lhs,
                                                           const T& value) {
        for(auto& x : lhs) x += value;
        return lhs;
    }
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator+(const T& value,
                                                           numerical_table<ValueType, WorkingPrecision>  lhs) {
        for(auto& x : lhs) x += value;
        return lhs;
    }
    
    
    template<typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator-(
            numerical_table<ValueType, WorkingPrecision>  lhs,
      const numerical_table<ValueType, WorkingPrecision>& rhs) {
        
        return lhs -= rhs;
    }
    
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator-(
                                                           numerical_table<ValueType, WorkingPrecision>  lhs,
                                                           const T& value) {
        for(auto& x : lhs) x -= value;
        return lhs;
    }
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator-(const T& value,
                                                           numerical_table<ValueType, WorkingPrecision>  lhs) {
        for(auto& x : lhs) x = value - x;
        return lhs;
    }
    
    
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator*(
                                                           numerical_table<ValueType, WorkingPrecision>  lhs,
                                                           const T& value) {
        for(auto& x : lhs) x *= value;
        return lhs;
    }
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator*(const T& value,
                                                           numerical_table<ValueType, WorkingPrecision>  lhs) {
        for(auto& x : lhs) x *= value;
        return lhs;
    }
    
    template<typename T, typename ValueType=double, typename WorkingPrecision=long double>
    numerical_table<ValueType, WorkingPrecision> operator/(
                                                           numerical_table<ValueType, WorkingPrecision>  lhs,
                                                           const T& value) {
        for(auto& x : lhs) x /= value;
        return lhs;
    }
    
    
    
    
}

#endif /* numerical_table_h */
