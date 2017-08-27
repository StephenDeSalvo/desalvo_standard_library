//
//  binary_integer.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 1/31/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_binary_integer_h
#define DeSalvo_Standard_Library_binary_integer_h

#include <iostream>
#include <vector>
#include <cmath>

namespace desalvo_standard_library {
    
    
    /** @class binary_integer
     @brief Stores an binary_integer value using bits
     
     This class is designed to mimic the int data type
     
     */
    class binary_integer {
        
        class binary_integer_string;
        
        friend std::ostream& operator<<(std::ostream& out, const binary_integer::binary_integer_string& a);
        friend std::ostream& operator<<(std::ostream& out, const binary_integer& a);
        friend std::istream& operator>>(std::istream& in, binary_integer& a);
    public:
        
        // Constructors
        /** Initialize to 0 */ binary_integer() : sign(false), bit(1,0) { }
        binary_integer(long long int a);
        binary_integer(const std::string& of_digits);
        
        
        // Arithmetic mutator operators
        binary_integer& operator+=(const binary_integer& rhs);
        binary_integer& operator-=(const binary_integer& rhs);
        binary_integer& operator*=(const binary_integer& rhs);
        //binary_integer& operator/=(const binary_integer& rhs) { return *this; };
        //binary_integer& operator%=(const binary_integer& rhs) { return *this; };
        
        // Increment and Decrement operators
        binary_integer& operator++();
        binary_integer operator++(int unused);
        binary_integer& operator--();
        binary_integer operator--(int unused);
        
        // Negation and Copy operators
        binary_integer operator-() const;
        binary_integer operator+() const;
        
        // Bit-wise mutator operators
        binary_integer& operator&=(const binary_integer& rhs);
        binary_integer& operator|=(const binary_integer& rhs);
        binary_integer& operator^=(const binary_integer& rhs);
        binary_integer& operator<<=(const binary_integer& rhs);
        binary_integer& operator>>=(const binary_integer& rhs);
        
        // Negation operator
        binary_integer operator~() const;
        
        // Comparison operators
        bool operator< (const binary_integer& rhs) const;
        bool operator==(const binary_integer& rhs) const;
        
        // Simpe print functions
        void print_as_int()  const;
        void print_as_bits() const;
        
        // Absolute value operator
        binary_integer abs() const;
        
        // Conversion *function*, purposely not written as a conversion operator.
        long long int to_llint() const;
        
        std::vector<bool> bit;
    private:
        
        bool sign;
        
        void clean_up();
        void to_twos_complement();
        
        
        
    };
    
    // Arithemtic operators
    binary_integer operator+(binary_integer lhs, const binary_integer& rhs);
    binary_integer operator-(binary_integer lhs, const binary_integer& rhs);
    binary_integer operator*(binary_integer lhs, const binary_integer& rhs);
    /*
     binary_integer operator/(binary_integer lhs, const binary_integer& rhs) { return lhs; };
     binary_integer operator%(binary_integer lhs, const binary_integer& rhs) { return lhs; };
     */
    
    
    // Bit-wise operators
    binary_integer operator& (binary_integer lhs, const binary_integer& rhs);
    binary_integer operator| (binary_integer lhs, const binary_integer& rhs);
    binary_integer operator^ (binary_integer lhs, const binary_integer& rhs);
    binary_integer operator<<(binary_integer lhs, const binary_integer& rhs);
    binary_integer operator>>(binary_integer lhs, const binary_integer& rhs);
    
    // Comparison operators
    bool operator!= (const binary_integer& lhs, const binary_integer& rhs);
    bool operator>  (const binary_integer& lhs, const binary_integer& rhs);
    bool operator<= (const binary_integer& lhs, const binary_integer& rhs);
    bool operator>= (const binary_integer& lhs, const binary_integer& rhs);
    
    
    // Helper functions for the binary_integer classes
    int char_to_int(char c1);
    char digit_to_char(int a);
    
    
    
}



#endif
