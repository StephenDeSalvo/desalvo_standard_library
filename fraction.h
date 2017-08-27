#ifndef DeSalvo_Standard_Library_fraction_h
#define DeSalvo_Standard_Library_fraction_h

//
//  DeSalvoFractionLibrary.h
//  DeSalvoLibraryTestKit
//
//  Created by Stephen DeSalvo on 12/3/14.
//  Copyright (c) 2014 Stephen DeSalvo. All rights reserved.
//

#include <iostream>

namespace desalvo_standard_library {
    /** @class Fraction
     @brief Fraction class for storing int/int with arithmetic operators defined
     
     This class attempts to mimic the usual mathematical notion of a fraction, allowing the programmer to write statements like a*b, a+=b, etc. for two Fraction objects a and b.
     */
    template<typename T>
    class Fraction {
    private:
        /** @var top is the numerator of the fraction */
        /** @var bottom is the denominator of the fraction */
        T top;
        T bottom;
        
        void reduce();
        
    public:
        // Constructors
        Fraction();
        
        template<typename F>
        Fraction<T>(F t);
        
        template<typename F, typename G>
        Fraction(F t, G b);
        //        Fraction(ull t, ull b);
        
        // member operators
        Fraction& operator+=(const Fraction& f);
        Fraction& operator-=(const Fraction& f);
        Fraction& operator*=(const Fraction& f);
        Fraction& operator/=(const Fraction& f);
        
        // Incremental operators
        Fraction& operator++(); // { top += bottom; return *this; };
        Fraction operator++(int);
        Fraction& operator--(); // { top -= bottom; return *this; };
        Fraction operator--(int);
        
        operator double();
        
        // comparison operators, all others are defined below
        friend bool operator==(const Fraction& lhs, const Fraction& rhs);
        friend bool operator< (const Fraction& lhs, const Fraction& rhs);
        
        friend std::ostream& operator<<(std::ostream& out, const Fraction& frac)
            {
                return out<<frac.top<<"/"<<frac.bottom;
            };
        
    };
    
    /** Default Constructor for Fraction initializes to 0/1. */
    template<typename T>
    inline Fraction<T>::Fraction() : top(0), bottom(1) { };
    
    /** Constructor initializer for int input
     @param t is the numerator
     */
    template<typename T>
    template<typename F>
    inline Fraction<T>::Fraction(F t) : top(static_cast<T>(t)), bottom(1) { };
    
    /** Constructor for numerator/denominator inputs
     @param t is the numerator
     @param b is the denominator
     */
    template<typename T>
    template<typename F, typename G>
    inline Fraction<T>::Fraction(F t, G b) : top(t), bottom(b) {
        reduce();
    };
    
    
    /** Operator for adding one fraction to another, overwriting the initial fraction
     @param f is the value to add to the object
     @returns a reference to the object being added to
     */
    template<typename T>
    Fraction<T>& Fraction<T>::operator+=(const Fraction<T>& f) {
        (top *=f.bottom) += f.top*bottom;
        bottom *= f.bottom;
        reduce();
        
        return *this;
    }
    
    /** Operator for subtracting one fraction to another, overwriting the initial fraction
     @param f is the value to subtract from the object
     @returns a reference to the object being subtracted from
     */
    template<typename T>
    Fraction<T>& Fraction<T>::operator-=(const Fraction<T>& f) {
        (top *=f.bottom) -= f.top*bottom;
        bottom *= f.bottom;
        reduce();
        
        return *this;
    }
    
    /** Operator for multiplying one fraction to another, overwriting the initial fraction
     @param f is the value to multiply the object
     @returns a reference to the object being multiplied
     */
    template<typename T>
    Fraction<T>& Fraction<T>::operator*=(const Fraction<T>& f) {
        top *= f.top;
        bottom *= f.bottom;
        reduce();
        
        return *this;
    }
    
    /** Operator for dividing by one fraction to another, overwriting the initial fraction
     @param f is the value to divide by the object
     @returns a reference to the object being divided by
     */
    template<typename T>
    Fraction<T>& Fraction<T>::operator/=(const Fraction<T>& f) {
        top *= f.bottom;
        bottom *= f.top;
        reduce();
        
        return *this;
    }
    
    /** The prefix increment operator for a fraction adds 1, or adds the denominator to the numerator
     @returns a reference to the Fraction object that called the operator
     */
    template<typename T>
    Fraction<T>& Fraction<T>::operator++() {
        top += bottom;
        reduce();
        
        return *this;
    };
    
    /** The postfix increment operator for a fraction adds 1, or adds the denominator to the numerator
     @param unused parameter to indicate postfix
     @returns a new Fraction object with the pre-incremented value
     */
    template<typename T>
    Fraction<T> Fraction<T>::operator++(int) {
        Fraction clone(*this);
        operator++();
        
        return clone;
    }
    
    /** The prefix decrement operator for a fraction subtracts 1, or subtracts the denominator to the numerator
     @returns a reference to the Fraction object that called the operator
     */
    template<typename T>
    Fraction<T>& Fraction<T>::operator--() {
        top -= bottom;
        reduce();
        
        return *this;
    };
    
    /** The postfix decrement operator for a fraction subtracts 1, or subtracts the denominator from the numerator
     @param unused parameter to indicate postfix
     @returns a new Fraction object with the pre-decremented value
     */
    template<typename T>
    Fraction<T> Fraction<T>::operator--(int) {
        Fraction clone(*this);
        operator--();
        return clone;
    }
    
    // Arithmetic Operations
    
    /** Adds two fractions together
     @param f is the left object
     @param f2 is the right object
     @returns a new fraction object consisting of the sum
     */
    template<typename T>
    Fraction<T> operator+(Fraction<T> f, const Fraction<T>& f2) {
        return f+=f2;
    };
    
    // f + 5
    //Fraction<T> Fraction::operator+(int a) {
    //}
    
    //ThingA operator+(string s, int d);
    
    /** Subtracts two fractions together
     @param f is the left object
     @param f2 is the right object
     @returns a new fraction object consisting of the difference
     */
    template<typename T>
    Fraction<T> operator-(Fraction<T> f, const Fraction<T>& f2) { return f-=f2; };
    
    /** Multiplies two fractions together
     @param f is the left object
     @param f2 is the right object
     @returns a new fraction object consisting of the product
     */
    template<typename T>
    Fraction<T> operator*(Fraction<T> f, const Fraction<T>& f2) { return f*=f2; };
    
    /** Divides two fractions together
     @param f is the left object
     @param f2 is the right object
     @returns a new fraction object consisting of the quotient
     */
    template<typename T>
    Fraction<T> operator/(Fraction<T> f, const Fraction<T>& f2) { return f/=f2; };
    
    // comparison operators
    
    /** Cross multiplies and compares the resulting integers.  WARNING!  This is not the best method, as there may be overflow.  It would be more numerically stable to reduce the fractions to lowest terms and compare the numerators and denominators directly.
     @param lhs is the left object
     @param rhs is the right object
     @returns true if the cross products are the same, false otherwise
     */
    template<typename T>
    bool operator==(const Fraction<T>& lhs, const Fraction<T>& rhs) {
        // Assumes the fraction is in reduced form
        return ( (lhs.top == rhs.top) && (lhs.bottom == rhs.bottom) );
        //return (lhs.top*rhs.bottom == lhs.bottom*rhs.top);
    }
    
    /** Cross multiplies and compares the resulting integers.  WARNING!  This is not the best method, as there may be overflow.  It would be more numerically stable to reduce the fractions to lowest terms and compare the numerators and denominators directly.
     @param lhs is the left object
     @param rhs is the right object
     @returns true if the cross products correspond to strict inequality, false otherwise
     */
    template<typename T>
    bool operator< (const Fraction<T>& lhs, const Fraction<T>& rhs){
        // Handle trivial cases first
        // We assume only numerators can be negative
        // We assume fractions are in reduced form
        
        // a/b < c/b iff a < b
        if(lhs.bottom == rhs.bottom) return lhs.top < rhs.top;
        
        // a/b < a/c iff b > c
        else if(lhs.top == rhs.top) return lhs.bottom > rhs.bottom;
        
        // check using arithmetic.
        else return (lhs.top*rhs.bottom < lhs.bottom*rhs.top);
    }
    
    /** Tests for inequality
     @param lhs is the left object
     @param rhs is the right object
     @returns !(lhs==rhs)
     */
    template<typename T>
    bool operator!=(const Fraction<T>& lhs, const Fraction<T>& rhs){return !operator==(lhs,rhs);};
    /** @returns !(rhs<lhs) */
    template<typename T>
    bool operator> (const Fraction<T>& lhs, const Fraction<T>& rhs){return  operator< (rhs,lhs);};
    /** @returns !(lhs>rhs) */
    template<typename T>
    bool operator<=(const Fraction<T>& lhs, const Fraction<T>& rhs){return !operator> (lhs,rhs);};
    /** @returns !(lhs<rhs) */
    template<typename T>
    bool operator>=(const Fraction<T>& lhs, const Fraction<T>& rhs){return !operator< (lhs,rhs);};
    
    /** Output operator which prints out top/bottom
     @param out is the stream object
     @param frac is the Fraction object to output
     @returns a reference to the stream object for chaining
     *//*
    template<typename T>
    std::ostream& operator<<(std::ostream& out, const Fraction<T>& frac) {
        return out<<frac.top<<"/"<<frac.bottom;
    };*/
    
    
    /** Calculate the gcd of two unsigned integers
     @param a is the left side
     @param b is the right side
     @returns gcd(a,b), i.e., the greatest common divisor. */
    template<typename T>
    T gcd(T a, T b) {
        
        if(a==0 || b==0) return 0;
        // The readable version of the code is boring
        /*
         if( a == b )
         return a;
         else if(a > b)
         return gcd(a-b, b);
         else
         return gcd(a, b-a);
         */
        
        // Sweet.  +1 for recurison, +1 for use of ternary operator
        // -1000 for writing unreadable code.
        return a==b ?  a : a>b?  gcd(a-b,b) :  gcd(a,b-a) ;
        
    }
    
    /** Reduces the fraction to lowest terms using the gcd function above */
    template<typename T>
    void Fraction<T>::reduce() {
        
        // Get unsigned values of top and bottom
        // Never mix signed and unsigned values, since unsigned ints cannot get promoted to signed ints.
        // The /= expressions below will convert everything to unsigned int types and the resulting expression will also be unsigned, which will not work when top or bottom are negative.
        
        if(top == 0 && bottom != 0)
            bottom = 1;
        else if(top == 0 && bottom == 0)
        {
            std::cout<<" Warning, 0/0 " << std::endl;
        }
        else if(bottom == 0) {
            std::cout << "Warning, divide by 0" << std::endl;
        }
        else {
            
            T local_top    = std::abs(top);
            T local_bottom = std::abs(bottom);
            
            // call gcd to obtain greatest common divisor.
            local_top = gcd(local_top, local_bottom);
            
            top /= local_top;
            bottom /= local_top;
            
            // if -a/-b or a/-b
            if( (top < 0 && bottom < 0) || (bottom < 0 && top > 0) ) {
                top = -top;
                bottom = -bottom;
            }
        }
        
    }
    
    /** Conversion to double */
    template<typename T>
    Fraction<T>::operator double() {
        return static_cast<double>(top)/bottom;
    }
    
}





#endif
