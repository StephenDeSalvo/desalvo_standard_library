//
//  binary_integer.cpp
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 1/31/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

#include "binary_integer.h"
#include <string>
#include <sstream>

namespace desalvo_standard_library {
    
    
    /** Initialize using variable of type long long int
     @param a is the initial value
     */
    binary_integer::binary_integer(long long int a) {
        
        //unsigned long long int get_bit = 1;
        
        if(a < 0) {
            sign = true;
            a = -a;
        }
        else  sign = false;
        
        // If a = 0 or a = 1, simple solution
        if(a == 0) { bit.push_back(0); return; }
        if(a == 1) { bit.push_back(1); return; }
        
        // Now we get to assume a >= 2
        
        // get the max number of possible bits
        size_t number_of_bits = ceil(log2(a))+1;
        
        // resize bits to max possible.
        bit.resize(number_of_bits, false);
        
        // get bits one at a time.
        size_t index = 0;
        while(a > 0) {
            // get first bit of a
            bit[index] = a&1;
            
            ++index;
            // bit shift down by 1
            a>>=1;
        }
        
        // We may have added too many zeros.
        index = bit.size()-1;
        while(bit[index] == false) {
            bit.pop_back();
            --index;
        }
        // Note that we do not call the clean_up function in the constructor.
        // Constructors need to be handled with care since it is very important that they complete without a runtime error.
        
    };
    
    /** Input using a string of digits
     @param of_digits contains the digits in the usual notation, with a leading minus sign for negative numbers
     */
    binary_integer::binary_integer(const std::string& of_digits) : bit(1) {
    
        // initialize to 0
        if(of_digits == "" || of_digits == "0") {
            bit[0] = 0;
            return;
        }
        
        if(of_digits == "1") {
            bit[0] = 1;
            sign = 0;
            return;
        }
        
        if(of_digits == "-1") {
            bit[0] = 1;
            sign = 1;
            return;
        }
        
        // Add up one digit at a time.
        size_t n = of_digits.size();
        
        for(size_t i=n-1; i>0;--i) {
            binary_integer digit = char_to_int(of_digits[i]);
            
            // raise digit to its correct power
            for(size_t j=0;j<n-1-i;++j)
                digit *= 10;
            
            *this += digit;
            
        }

        // Determine sign
        if( of_digits[0] == '-')
            sign = 1;
        else
            sign = 0;
        
        // add in last digit
        if(sign == 0) {
            binary_integer digit = char_to_int(of_digits[0]);
            
            // raise digit to its correct power
            for(size_t j=0;j<n-1;++j)
                digit *= 10;
            
            *this += digit;
        }
        
    
    }
    
    
    /** Add two binary_integers together, a += b
     @param rhs is the right hand side of the equation a += b;
     @returns a reference to a in a+=b;
     */
    binary_integer& binary_integer::operator+=(const binary_integer& rhs) {
        
        
        // Need to convert cases with exactly one negative into those with positives
        bool convert_back = false;
        short twos_complement = 0;
        
        // If a is negative and b is positive, convert a to two's complement form
        if(sign == 1 && rhs.sign == 0) {
            
            // check for a += -a, and if so return 0;
            if(bit == rhs.bit) {
                bit.resize(1);
                bit[0] = 0;
                sign = 0;
                return *this;
            }
            
            // check if b < a, in which case after we do the arithmetic our result will be a negative value in two's complement form
            if(rhs.abs() < this->abs()) convert_back = true;
            
            
            // save size of bits, since 1s at end will be converted to 0s and cleaned up after to_twos_complement is run
            size_t save_size = bit.size();
            
            // convert to two's complement form for negative binary_integers
            this->to_twos_complement();
            
            // the to_twos_complement function will get rid of extra 0s, but we want to put them back in as 1s
            bit.resize(save_size, false);
            
            //this->print_as_bits(); std::cout << std::endl;
            
            // We will need this for addition, when adding numbers in two's complement form and filling in missing bits
            
            twos_complement = 1;
            
            // continue with adding bit by bit below, treating the bits as a positive number.
        }
        else {
            
            // for the case below, we assume that a is positive and b is negative, and since we cannot modify b we rearrange the arithmetic expression by making a copy which is very bad, but it is a quick solution.
            
            // This one is a bit complicated.  Basically, assume that we can do a += b but only when a is negative.
            // So when b is negative, we make a copy (bad, I know), and do b += a, and set the result equal to a.
            if(sign == 0 && rhs.sign == 1) {
                binary_integer clone(rhs);
                
                *this = (clone += *this);
                return *this;
            }
        }
        
        
        // Case when both are positive or both are negative, in which case we simply add the bits together.
        // Note that when both are negative, there is no need to convert to two's complement form.
        
        // Different cases, all keep rhs const
        
        // carry will be 0 or 1, but treated like an binary_integer.
        short carry = 0;
        
        size_t rhs_size = rhs.bit.size();
        size_t this_size = bit.size();
        
        
        // if rhs has smaller size, then add bit by bit
        if(rhs_size < this_size) {
            
            for(size_t i=0; i<rhs_size; ++i) {
                
                carry = bit[i] + rhs.bit[i] + carry;
                
                if(carry == 1) {
                    bit[i] = 1;
                    carry = 0;
                }
                else if(carry == 2) {
                    bit[i] = 0;
                    carry = 1;
                }
                else if(carry == 3) {
                    bit[i] = 1;
                    carry = 1;
                }
            }
            
            size_t index = rhs_size;
            
            // Keep carries along until they are exhausted
            while(carry == 1 && index < bit.size()) {
                carry += bit[index];
                
                
                if(carry == 1) {
                    bit[index] = 1;
                    carry = 0;
                }
                if(carry == 2) {
                    bit[index] = 0;
                    carry = 1;
                }
                
                ++index;
                
            }
            
            // We could still have a final carry, but so could the other case, so we leave this to the end
            
        }
        else { // this_size < rhs_size
            
            // resize the vector to have more values.
            bit.resize(rhs_size,twos_complement);
            
            for(size_t i=0;i<rhs_size; ++i) {
                
                carry = bit[i] + rhs.bit[i] + carry;
                
                if(carry == 1) {
                    bit[i] = 1;
                    carry = 0;
                }
                else if(carry == 2) {
                    bit[i] = 0;
                    carry = 1;
                }
                else if(carry == 3) {
                    bit[i] = 1;
                    carry = 1;
                }
            }
            
            // At this point, there is at most 1 extra carry to be done, handled below outside of this case
            
        }
        
        
        
        // If the final carry goes outside, AND it is a carry not from negative binary_integer arithmetic, then need to push_back another 1
        if(convert_back) {
            this->to_twos_complement();
            
        }
        else // carry is to extend precision of *positive* value, push back the 1 if possible.
            if(carry == 1 && twos_complement == 0) {
                bit.push_back(1);
            }
        
        
        clean_up();
        
        return *this;
        
        
    };
    
    
    
    
    
    
    
    /** Increment by 1
     @returns a reference to the incremented value
     */
    binary_integer& binary_integer::operator++() {
        return *this += binary_integer(1);
    };
    
    /** Increment by 1
     @returns the value before the increment
     */
    binary_integer binary_integer::operator++(int unused) {
        auto clone(*this);
        ++(*this);
        return clone;
    };
    
    /** Decrement by 1
     @returns a reference to the decremented value
     */
    binary_integer& binary_integer::operator--() {
        return *this -= binary_integer(1);
    };
    
    /** Decrement by 1
     @returns the value before the decrement
     */
    binary_integer binary_integer::operator--(int unused) {
        auto clone(*this);
        --(*this);
        return clone;
        
    };
    
    /** Compute the negative
     @returns the negative of the value
     */
    binary_integer binary_integer::operator-() const {
        auto clone(*this);
        clone.sign = !clone.sign;
        return clone;
    };
    
    /** Make a copy
     @returns a copy of the value
     */
    binary_integer binary_integer::operator+() const {
        return *this;
    };
    
    
    /** Negate all bits, including signed bit
     @returns the negation of the binary_integer
     */
    binary_integer binary_integer::operator~() const {
        auto clone(*this);
        clone.sign = !clone.sign;
        clone.bit.flip();
        clone.clean_up();
        return clone;
    };
    
    /** Subtract two binary_integers together, a -= b, store the result in a
     @param rhs is the right hand side of the equation a -= b;
     @returns a reference to a in a-=b;
     */
    binary_integer& binary_integer::operator-=(const binary_integer& rhs) {
        return *this += (-rhs);
    };
    
    
    /** Multiplies two binary_integer together, a *= b, using the elementary school algorithm.
     @param rhs is the right hand side of the equation a *= b
     @returns a reference to a in a *= b;
     */
    binary_integer& binary_integer::operator*=(const binary_integer& rhs) {
        
        // Here is an explicit example to demonstrate the algorithm
        //   10011
        // x  1101
        // -------
        //    1101
        //   11010
        //  000000
        // 0000000
        //11010000
        
        // Note that we are just bit shifting the bottom argument, and adding it when the bit of the top argument is 1
        
        // Also note that it doesn't matter whether the numbers are positive, negative, or any combination, we handle it at the end.
        
        // create copy of rhs which will be shifted
        
        
        binary_integer shifter(rhs.abs());
        
        // Create new value with running total
        binary_integer product(0);
        
        size_t this_size = bit.size();
        for(int i=0;i<this_size; ++i) {
            
            // If the bit is one, add the shifted value
            if(bit[i] == 1)
                product += shifter;
            
            // shift UP by 1 after each iteration, regardless of whether the bit is 1 or 0.
            shifter <<= 1;
            
        }
        
        // check if the signs differ, if so, the product is negative
        if(sign ^ rhs.sign)
            product.sign = 1;
        
        // reassign object to newly created one with the product
        *this = product;
        
        return *this;
        
    }
    
    
    
    /** Bit shift up operator, transforms 1011 --> 101100 when shifted by 2
     @param rhs is the amount to shift by, which should be small and positive
     @returns a reference to the object being shifted for chaining.
     */
    binary_integer& binary_integer::operator<<=(const binary_integer& rhs) {
        
        // shift the other direction
        if(rhs < 0) {
            return *this >>= -rhs;
        }
        
        // if rhs is positive, do shifting
        if(rhs >0 ) {
            // get current size, will need this for the index to copy over bits
            size_t this_size = bit.size();
            
            bit.resize(this_size + rhs.to_llint(), 0);
            
            // size of bits after shifting
            size_t new_size = bit.size();
            
            
            // Copy over elements to the end of the new vector
            for(size_t i=this_size-1; i>0; --i) {
                bit[new_size-1] = bit[i];
                --new_size;
            }
            
            // copy over final bit
            bit[new_size-1] = bit[0];
            --new_size;
            
            // reset all remaining bits
            for(size_t i=0;i<new_size; ++i)
                bit[i] = 0;
            
        }
        
        clean_up();
        // if rhs is 0, do nothing
        return *this;
        
    } ;
    
    /** Bit shift down operator, transforms 1011 --> 10 when shifted down by 2
     @param rhs is the amount to shift by, which should be small and positive
     @returns a reference to the object being shifted for chaining.
     */
    binary_integer& binary_integer::operator>>=(const binary_integer& rhs) {
        
        // If negative, shift in the other direction
        if(rhs < 0) {
            return *this <<= -rhs;
        }
        
        
        size_t this_size = bit.size();
        
        // if rhs is positive, do the shifting
        if(rhs > 0) {
            
            size_t n = rhs.to_llint();
            
            // if we shift away all values, we get 0.
            if(n >= this_size) {
                bit.resize(1);
                bit[0] = 0;
                return *this;
            }
            
            
            for(size_t i=0; i<this_size-n;++i) {
                
                bit[i] = bit[i+n];
                
            }
            bit.resize( this_size-n);
            
            
        }
        
        // if rhs is 0, do nothing
        
        return *this;
    } ;
    
    /** Performs bit-wise & on all bits, filling in 0s when necessary
     @param rhs is the right hand side of a &= b;
     @returns a reference for chaining
     */
    binary_integer& binary_integer::operator&=(const binary_integer& rhs) {
        
        // because we are filling in 0s, the resulting size is the min of the two
        size_t the_size = std::min( bit.size(), rhs.bit.size() );
        for(size_t i=0;i<the_size;++i)
            bit[i] = bit[i] & rhs.bit[i];
        
        bit.resize(the_size);
        
        // Apply operation to the signed bit as well
        sign = sign & rhs.sign;
        
        // Get rid of any zeros that appeared at the end
        clean_up();
        
        return *this;
    };
    
    /** Performs bit-wise or | operation
     @param rhs is the right hand side argument in a |= b;
     @returns a reference for chaining
     */
    binary_integer& binary_integer::operator|=(const binary_integer& rhs) {
        
        // size is going to be the max of the two
        size_t this_size = bit.size();
        size_t rhs_size = rhs.bit.size();
        
        // if rhs has more bits, resize *this to match, get bits at same time
        if(this_size < rhs_size) {
            bit.resize(rhs_size);
            for(size_t i = this_size; i<rhs_size; ++i)
                bit[i] = rhs.bit[i];
        }
        
        size_t the_size = std::min(this_size, rhs_size);
        
        // Perform or operation on first set of bits
        for(size_t i=0;i<the_size;++i)
            bit[i] = bit[i] | rhs.bit[i];
        
        
        // Apply operation to the signed bit as well
        sign = sign | rhs.sign;
        
        // no need to call clean_up, assuming we started with leading 1s, we will still have a leading 1
        
        return *this;
    };
    
    /** Performs bit-wise xor ^ operation
     @param rhs is the right hand side argument in a ^= b;
     @returns a reference for chaining
     */
    binary_integer& binary_integer::operator^=(const binary_integer& rhs) {
        
        // size is going to be the max of the two
        size_t this_size = bit.size();
        size_t rhs_size = rhs.bit.size();
        
        // if rhs has more bits, resize *this to match, get bits at same time
        if(this_size < rhs_size) {
            bit.resize(rhs_size);
            for(size_t i = this_size; i<rhs_size; ++i)
                bit[i] = rhs.bit[i]; // 0^1 = 1 and 0^0 = 0, so just get rhs bit
        }
        
        size_t the_size = std::min(this_size, rhs_size);
        
        // Perform or operation on first set of bits
        for(size_t i=0;i<the_size;++i)
            bit[i] = bit[i] ^ rhs.bit[i];
        
        // Apply operation to the signed bit as well
        sign = sign ^ rhs.sign;
        
        // Get rid of any zeros that appeared at the end
        clean_up();
        
        return *this;
    };
    
    
    /** Convert to a long long int value
     @returns the binary_integer as a long long int
     */
    long long int binary_integer::to_llint() const {
        
        long long int a = 0;
        
        unsigned long long int power_of_2 = 1;
        
        // build up value by successive powers of 2
        for(const auto& x : bit) {
            a += x*power_of_2;
            power_of_2 *= 2;
            //            std::cout << power_of_2 << std::endl;
        }
        
        if(sign == 1) a = -a;
        
        return a;
        
    }
    
    /** Converts the binary_integer to two's complement notation, assuming the bits are to be interpretted as a negative binary_integer.
     */
    void binary_integer::to_twos_complement() {
        bit.flip();
        
        bool save_sign = sign;
        
        sign = 0;
        *this += 1;
        
        // If we started with a positive number, two's complement makes it a negative number
        //if(save_sign == 0)
        sign = !save_sign;
        //else // if we started with a negative number, we are just changing the representation of the bits so it stays negative
        //    sign = save_sign;
    }
    
    
    /** Compares a < b for binary_integer types
     @param rhs is the right hand sidein a < b
     @returns true if a < b, i.e., if a is strictly less than b
     */
    bool binary_integer::operator< (const binary_integer& rhs) const {
        
        // Check for quick resolution using signs
        if(sign == 1 && rhs.sign == 0) return true;
        if(sign == 0 && rhs.sign == 1) return false;
        
        
        // Same sign means we have to check bit by bit
        
        size_t this_size = bit.size();
        size_t rhs_size = rhs.bit.size();
        
        bool flip_it = false;
        
        // If both are negative, then reverse the eventual decision
        if(sign == 1 && rhs.sign == 1) flip_it = true;
        
        // If the sizes are different, easy answer
        if(this_size < rhs_size)      return flip_it ? false : true;
        else if(this_size > rhs_size) return flip_it ? true : false;
        else {
            
            // same size, compare one by one
            for(size_t i=this_size-1; i>0; --i) {
                
                // check for disagreement
                if( bit[i] ^ rhs.bit[i]) {
                    if(bit[i]) return flip_it ? true : false;
                    else       return flip_it ? false : true;
                }
                
            }
            
            // check last bit
            if( bit[0] ^ rhs.bit[0]) {
                if(bit[0]) return flip_it ? true : false;
                else       return flip_it ? false : true;
            }
            
        }
        
        // If we made it through, they are equal, return false regardless
        return false;
        
    };
    
    /** Compares for equality, a == b
     @param rhs is the right hand side of a == b
     @returns true if a equals b, false otherwise
     */
    bool binary_integer::operator== (const binary_integer& rhs) const {
        
        size_t this_size = bit.size();
        size_t rhs_size = rhs.bit.size();
        
        // signs need to match, assuming -0 is not a valid state.
        if(sign != rhs.sign) return false;
        
        // check sizes first for easy decision
        if(this_size < rhs_size)      return false;
        else if(this_size > rhs_size) return false;
        else {
            
            // same size, compare one by one
            for(size_t i=this_size-1; i>0; --i) {
                // check for disagreement
                if( bit[i] ^ rhs.bit[i]) return false;
            }
            
            // check last bit
            if( bit[0] ^ rhs.bit[0]) return false;
        }
        
        // Made it through all tests, no disagreement between bits!
        return true;
        
    };
    
    /** Converts the binary_integer to its absolute value, simply by changing the sign bit
     @returns a new binary_integer with the absolute value of the origin
     */
    binary_integer binary_integer::abs() const {
        binary_integer clone(*this);
        clone.sign = false;
        return clone;
    }
    
    /** Eliminates any extra 0s */
    void binary_integer::clean_up() {
        
        size_t index = bit.size()-1;
        
        while(index > 0 && bit[index] == 0) {
            bit.pop_back();
            --index;
        }
        
    }
    
    /** Prints the number as a long long int */
    void binary_integer::print_as_int()  const {
        
        auto a = to_llint();
        
        std::cout << a;
        
    };
    
    
    /** Prints the number in terms of its bits */
    void binary_integer::print_as_bits() const {
        
        if(sign == true) std::cout << "-";
        else std::cout << " ";
        
        
        std::cout << "(";
        
        // queue up starting and ending bit, most significant bit first
        auto start = bit.rbegin();
        auto stop = bit.rend()-1;
        
        // loop through bits one at a time
        while(start != stop) std::cout << *start++; // << ",";
        
        std::cout << *start;
        
        std::cout << ")_2";
        
    };
    
    /** Compares for inequality, a != b
     @param rhs is the right hand side of a != b
     @returns false if a equals b, true otherwise
     */
    bool operator!= (const binary_integer& lhs, const binary_integer& rhs) { return !(lhs == rhs);}
    
    /** Checks whether a > b
     @param rhs is the right hand side of a > b
     @returns true if a > b
     */
    bool operator>  (const binary_integer& lhs, const binary_integer& rhs) { return rhs < lhs;};
    
    /** Checks whether a <= b
     @param rhs is the right hand side of a <= b
     @returns true if a <= b
     */
    bool operator<= (const binary_integer& lhs, const binary_integer& rhs) { return !(lhs > rhs);};
    
    /** Checks whether a >= b
     @param rhs is the right hand side of a >= b
     @returns true if a >= b
     */
    bool operator>= (const binary_integer& lhs, const binary_integer& rhs) { return !(lhs < rhs);};
    
    /** Computes a + b, returns new binary_integer
     @param rhs is the right hand side of a + b
     @returns a + b
     */
    binary_integer operator+(binary_integer lhs, const binary_integer& rhs) { return lhs += rhs; };
    
    
    /** Computes a - b, returns new binary_integer
     @param rhs is the right hand side of a - b
     @returns a - b
     */
    binary_integer operator-(binary_integer lhs, const binary_integer& rhs) { return lhs -= rhs; };
    
    
    /** Computes a * b, returns new binary_integer
     @param rhs is the right hand side of a * b
     @returns a * b
     */
    binary_integer operator*(binary_integer lhs, const binary_integer& rhs) { return lhs *= rhs; };
    
    
    /** Computes a & b bitwise, returns new binary_integer
     @param rhs is the right hand side of a & b
     @returns a & b
     */
    binary_integer operator& (binary_integer lhs, const binary_integer& rhs) { return lhs &= rhs; };
    
    
    /** Computes a | b, returns new binary_integer
     @param rhs is the right hand side of a | b
     @returns a | b
     */
    binary_integer operator| (binary_integer lhs, const binary_integer& rhs) { return lhs |= rhs; };
    
    
    /** Computes a ^ b, returns new binary_integer
     @param rhs is the right hand side of a ^ b
     @returns a ^ b
     */
    binary_integer operator^ (binary_integer lhs, const binary_integer& rhs) { return lhs ^= rhs; };
    
    
    /** Bit shifts a up by b, returns new binary_integer
     @param rhs is the right hand side of a << b
     @returns a shifted up by b
     */
    binary_integer operator<<(binary_integer lhs, const binary_integer& rhs) { return lhs <<= rhs; };
    
    /** Bit shifts a down by b, returns new binary_integer
     @param rhs is the right hand side of a >> b
     @returns a shifted down by b
     */
    binary_integer operator>>(binary_integer lhs, const binary_integer& rhs) { return lhs >>= rhs; };
    
    
    
    
    
    /** @class binary_integerString
     @brief Arbitrary Precision class for binary_integers
     
     I'm finally going to just make an arbitrary precision class for binary_integers.
     */
    class binary_integer::binary_integer_string {
        //friend std::ostream& operator<<(std::ostream& out, const binary_integer_string& a);
        friend binary_integer_string operator+(int j, const binary_integer_string& i);
        friend std::ostream& operator<<(std::ostream& out, const binary_integer_string& a);
        friend std::ostream& operator<<(std::ostream& out, const binary_integer& a);
        
        
    public:
        binary_integer_string() : digits("0") { };
        binary_integer_string(int n);
        
        // Need to modify to include sign.
        binary_integer_string(std::string dig) : digits(dig) { reverse(digits.begin(), digits.end()); };
        binary_integer_string(const binary_integer& b);
        virtual ~binary_integer_string() { };
        
        
        binary_integer_string MultiplyBy2();
        
        
        binary_integer_string operator+(const binary_integer_string& i);
        binary_integer_string operator+(int i);
        binary_integer_string operator+(unsigned int i);
        binary_integer_string operator+(unsigned long long i);
        
        friend std::ostream& operator<<(std::ostream& out, const binary_integer_string& a);
        
        
    private:
        binary_integer_string(std::string dig, bool reversed) : digits(dig) { if(!reversed) reverse(digits.begin(), digits.end()); };
        std::string digits;
    };
    
    
    int char_to_int(char c) {
        // ASCII for '0' is 48.
        return c-48;
    }
    
    char digit_to_char(int a) {
        
        return (char)(48+a);
    }
 
    
    binary_integer::binary_integer_string binary_integer::binary_integer_string::operator+(const binary_integer::binary_integer_string& a) {
        
        // How to add two strings of digits together.
        // Do it the kindergaaarten vay!
        
        // Variables
        int asciizero = 48;
        std::string newDigits;
        int carry = 0;
        
        // Stores the chars for each digit and their sum
        int c1, c2;
        int sum;
        
        size_t asize = a.digits.size();
        size_t bsize = digits.size();
        
        const char* smaller;
        const char* larger;
        
        if(asize <= bsize) {
            smaller = &(a.digits[0]);
            larger = &digits[0];
        }
        else {
            // swap asize and bsize
            size_t temp = asize;
            asize = bsize;
            bsize = temp;
            
            smaller = &(digits[0]);
            larger = &(a.digits[0]);
        }
        
        // Loop through each digit and add with carries when appropriate
        for(size_t i=0; i<asize; ++i) {
            //c1 = a.digits[i];
            //c2 = digits[i];
            c1 = *smaller;
            c2 = *larger;
            
            sum = (c1-asciizero + c2-asciizero) + carry;
            
            newDigits += std::to_string(sum%10);
            carry = sum/10;
            
            ++smaller;
            ++larger;
        }
        
        // Carries could continue indefinitely
        for(size_t i=asize; i<bsize; ++i) {
            //c2 = digits[i];
            c2 = *larger;
            //cout<<c1<<" "<<c2<<endl;
            sum = c2-asciizero+carry;
            
            newDigits += std::to_string(sum%10);
            carry = sum/10;
            
            ++larger;
        }
        
        // Supposing we need to add another digit
        if(carry)
            newDigits += std::to_string(carry);
        
        
        return binary_integer::binary_integer_string(newDigits, 1);
        
    }
    
    
    
    binary_integer::binary_integer_string binary_integer::binary_integer_string::MultiplyBy2() {
        
        size_t s = digits.size();
        
        binary_integer::binary_integer_string b;
        b.digits.resize(0);
        unsigned int raw = 0;
        unsigned int digit = 0;
        unsigned int carry = 0;
        unsigned int d = 0;
        
        for(size_t i=0; i<s; ++i) {
            
            // Convert digit from char to int
            d = char_to_int(digits[i]);
            
            raw = d*2+carry;
            digit = raw%10;
            carry = raw/10;
            
            b.digits += digit_to_char(digit);
            
        }
        
        if(carry)
            b.digits=b.digits+digit_to_char(carry);
        
        //for(size_t i=0, n=b.digits.size()) {
        
        this->digits = b.digits;
        
        return b;
        
        
    }
    
    
    binary_integer::binary_integer_string::binary_integer_string(const binary_integer& b) {
        
        
        //    if(b==0) {
        if(b==0) {
            this->digits = "0";
        }
        else if(b < 0) {
            binary_integer c(-b);
            
            binary_integer::binary_integer_string temp("1");
            for(size_t i=0, n=c.bit.size(); i<n; ++i) {
                
                if(c.bit[i])
                    *this = *this + temp;
                
                temp.MultiplyBy2();
            }
            digits += "-";
            
        }
        else {
            binary_integer::binary_integer_string temp("1");
            for(size_t i=0, n=b.bit.size(); i<n; ++i) {
                
                if(b.bit[i])
                    *this = *this + temp;
                
                temp.MultiplyBy2();
            }
            
        }
        
    }
    
    
    std::ostream& operator<<(std::ostream& out, const binary_integer::binary_integer_string& a) {
        
        std::string outputString = a.digits;
        
        reverse(outputString.begin(), outputString.end());
        
        return out<<outputString;
    }
    
    
    std::ostream& operator<<(std::ostream& out, const binary_integer& a) {
        
        binary_integer::binary_integer_string b(a);
        out<<b;
        /*
         for(size_t i=0, n=a.bits.size();i<n; ++i) {
         out<<a.bits[n-i-1];
         }
         */
        return out;
        
    }
    
    
    std::istream& operator>>(std::istream& in, binary_integer& a) {
        
        // Get digits from the console in the form of a string.
        std::string digits;
        
        std::getline(in, digits);
        
        a = binary_integer(digits);

        return in;
    }
    
    
    
    
    
    
}
