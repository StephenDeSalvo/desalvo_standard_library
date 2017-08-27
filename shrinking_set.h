/** @file shrinking_set.h
 @brief contains sets which start off with a prescribed set of elements and then shrink
 
 */


#ifndef DeSalvoLibraryTestKit_DeSalvoDataStructuresLibrary_h
#define DeSalvoLibraryTestKit_DeSalvoDataStructuresLibrary_h

#include <iostream>
#include <vector>
#include <functional>
#include <initializer_list>
#include <array>
#include "std_cout.h"
#include "numerical.h"

namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    /** @class shrinking_set_unordered
     @brief initialized with a set of objects, then efficiently erases and resets again
     
     @tparam T is the underlying type of objects
     
     @details The motivation for this class is from random sampling of Sudoku matrices and Latin squares, where
     one starts with a set of objects {1,2,...,n} and then erases elements one at a time according to some
     set of constraints. \n
     
     If you just use std::set, then there are a lot of memory management costs.  Instead, the idea is to always store the entire
     set of objects, and then swap out elements at the end.  For example, suppose we start with {1,2,...,9} and then eliminate the
     5, then 7, then 8 by swapping and updating a pointer.  The set would store its elements in a vector with the following order \n
     {1,2,3,4,5,6,7,8,9}\n
     {1,2,3,4,9,6,7,8,5}\n
     {1,2,3,4,9,6,8,7,5}\n
     {1,2,3,4,9,6,8,7,5}\n
     
     The size of the list and valid set of elements is kept track of by a pointer which points to one after the last element in the allowable list parts.  It starts out pointing to one after the 9, then at the 5, then at the 7, then at the 8.  To the user of the class, it will appear as though the list consists of the set of elements\n
     
     {1,2,3,4,5,6,7,8,9}\n
     {1,2,3,4,9,6,7,8}\n
     {1,2,3,4,9,6,8}\n
     {1,2,3,4,9,6}\n
     
     When reset is called, the pointer simply returns to one after the last position, or equivalent to std::end(...).  The initial order is not respected, and so when reset is called at this point the user of the class now has \n
     
     {1,2,3,4,9,6,8,7,5} \n
     
     which contains all elements, but in some other order.
     */
    template<typename T>
    class shrinking_set_unordered {
    
        /** output operator, outputs of the form {#,#,...,#}
            @param out is the stream
            @param s is the collection of objects
            @returns the stream for chaining
         @code
         #include <initializer_list>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create default set of size 0;
         dsl::shrinking_set<char> v;
         
         // Initialize set with letters a,b,c,d,e,f
         // Cannot just use initializer list, since by default each character is a const char*
         dsl::shrinking_set_unordered<char> v2(std::vector<char>({'f','e','c','d','a','b'}));
         
         dsl::print(v,"\n");
         dsl::print(v2);
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {}
         {f,e,c,d,a,b}
         @endcode
        */
        friend std::ostream& operator<<(std::ostream& out, const shrinking_set_unordered<T>& s) {
            out << "{";
            
            if(s.start != s.stop) {
                auto it = s.start;
                auto pen = s.stop-1;
                while(it != pen)
                    out << *it++ << ",";
                out << *it;
                
            }
            out << "}";
            return out;
        }
    public:
        
        /** Constructs empty collection
         @code
         #include <initializer_list>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create default set of size 0;
         dsl::shrinking_set_unordered<char> v;
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {}
         @endcode
         */
        shrinking_set_unordered() {
            start = std::begin(elements);
            stop = std::end(elements);
        }
        
        /** Constructs collection of objects using an initializer list of values in any order
         @param list is the collection of objects
         @code
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Initialize set with letters a,b,c,d,e,f in any order
         dsl::shrinking_set_unordered<char> v {'N','o','w',' ','i','s',' ','t','h','e',' ','w','i','n','t','e','r',' ','o','f'};
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {N,o,w, ,i,s,t,h,e,n,r,f}
         @endcode
         */
        shrinking_set_unordered(std::initializer_list<T> list) : elements(std::move(list)) {
            
            
            // remove duplicate entries
            auto it = dsl::unique_copy_nonconsecutive(std::begin(elements), std::end(elements), std::begin(elements));
            //auto it = std::unique_copy(start, stop, start);
            
            stop = elements.erase(it,std::end(elements));
            
            start = std::begin(elements);
            //stop = std::end(elements);
            
        };
        
        
        /**
         Constructs collection of objects using a collection stored in an object of type std::array<T,N>
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         std::array<int,5> array_of_ints {3,1,4,1,5};
         
         dsl::shrinking_set_unordered<int> v4(array_of_ints);
         dsl::print(v4,"\n");
         
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,1,4,5}
         @endcode
         */
        template<size_t N>
        shrinking_set_unordered(std::array<T,N> list) {
            
            auto it = std::begin(list);
            auto it_stop = std::end(list);
            
            // Iterator through collection of objects
            while(it != it_stop) {
                elements.push_back(*it++);
            }
            start = std::begin(elements);
            stop = std::end(elements);
            
            // remove duplicate entries
            //auto stop_it = std::unique_copy(start, stop, start);
            auto stop_it = dsl::unique_copy_nonconsecutive(start, stop, start);
            stop = elements.erase(stop_it,stop);
            
        };
        
        /** Constructs collection of objects using a collection stored using pointers
         @tparam F is a pointer type, with elements convertable to T
         @param list is the pointer to the collection of elements
         @param length is the number of elements starting at list to consider
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         double p[3] {3.14159265, 2.718281828, 2};
         dsl::shrinking_set_unordered<double> v5(p,3);
         dsl::print(v5,"\n");
         
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3.14159,2.71828,2}
         @endcode
         */
        template<typename F>
        shrinking_set_unordered(F* list, size_t length) : elements(list, list+length) {
            
            // remove duplicate entries
            auto it = dsl::unique_copy_nonconsecutive(std::begin(elements), std::end(elements), std::begin(elements));
            //auto it = std::unique_copy(start, stop, start);
            
            stop = elements.erase(it,std::end(elements));
            
            start = std::begin(elements);
            //stop = std::end(elements);
            
        };
        
        
        
        /** Constructs collection of objects using any object which can be copy constructed by an std::vector
            @tparam U is the type of the collection of objects
            @param list is the collection of objects
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         std::list<char> list_of_chars {'z','y','x','w'};
         dsl::shrinking_set_unordered<char> v2(list_of_chars);
         dsl::print(v2,"\n");
         
         std::set<char> set_of_chars {'r','o','f','l','m','a','o'};
         dsl::shrinking_set_unordered<char> v3(set_of_chars);
         dsl::print(v3,"\n");
         
         std::array<int,5> array_of_ints {3,1,4,1,5};
         dsl::shrinking_set_unordered<int> v4(array_of_ints);
         dsl::print(v4,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {z,y,x,w}
         {a,f,l,m,o,r}
         {3,1,4,5}
         @endcode
        */
        template<typename U>
        shrinking_set_unordered(U&& list) : elements(std::begin(list), std::end(list)) {
            
            
            // remove duplicate entries
            auto it = dsl::unique_copy_nonconsecutive(std::begin(elements), std::end(elements), std::begin(elements));
            //auto it = std::unique_copy(start, stop, start);
            
            stop = elements.erase(it,std::end(elements));
            start = std::begin(elements);
            
        };
        
        /** Constructs collection of objects using a range of values specified by input iterators
         @tparam InputIterator is any input iterator type
         @param it is an iterator referring to the first element
         @param it_stop is an iterator referring to one after the last element
         @code
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         char p[] = {'N','o','w',' ','i','s',' ','t','h','e',' ','w','i','n','t','e','r',' ','o','f',' ','o','u','r',' ','d','i','s','c','o','n','t','e','n','t'};
         
         // Initialize set with letters a,b,c,d,e,f in any order
         dsl::shrinking_set_unordered<char> v(std::begin(p), std::end(p));
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {N,o,w, ,i,s,t,h,e,n,r,f,u,d,c}
         @endcode
         
         Example 2:
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         std::list<char> list_of_chars {'z','y','x','w'};
         dsl::shrinking_set_unordered<char> v2(std::begin(list_of_chars), std::end(list_of_chars));
         dsl::print(v2,"\n");
         
         std::set<char> set_of_chars {'r','o','f','l','m','a','o'};
         dsl::shrinking_set_unordered<char> v3(std::begin(set_of_chars), std::end(set_of_chars));
         dsl::print(v3,"\n");
         
         std::array<int,5> array_of_ints {3,1,4,1,5};
         dsl::shrinking_set_unordered<int> v4(std::begin(array_of_ints), std::end(array_of_ints));
         dsl::print(v4,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {z,y,x,w}
         {a,f,l,m,o,r}
         {3,1,4,5}
         @endcode
         */
        template<typename InputIterator>
        shrinking_set_unordered(InputIterator it, InputIterator it_stop) {
            
            // Iterator through collection of objects, add to internal vector
            while(it != it_stop) {
                elements.push_back(*it++);
            }
            start = std::begin(elements);
            stop = std::end(elements);
            
            // remove duplicate entries
            //auto stop_it = std::unique_copy(start, stop, start);
            auto stop_it = dsl::unique_copy_nonconsecutive(start, stop, start);
            stop = elements.erase(stop_it,stop);

        };
        
        
        /** reinitialize using a vector of the same type
         @param list is another collection of objects
         @code
         #include <numeric>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector
         std::vector<int> v {1,9,2,8,3,7,4,6,5,5,4,3,2,1};
         
         // Initialize entries using iterators
         dsl::shrinking_set_unordered<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Create new vector of values {-10,-9,...,9,10}
         std::vector<int> v3(21);
         std::iota(std::begin(v3), std::end(v3), -10);
         
         // Reinitialize shrinking set with those values
         v2.reinitialize(v3);
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,9,2,8,3,7,4,6,5}
         {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10}
         @endcode
        */
        template<typename U>
        void reinitialize(U&& list) {
            
            elements.clear();
            std::copy( std::begin(list), std::end(list), std::back_inserter(elements));
            start = std::begin(elements);
            stop = std::end(elements);
            
            // remove duplicate entries
            auto stop_it = std::unique_copy(start, stop, start);
            stop = elements.erase(stop_it,stop);
        }
        
        /** reinitialize using another pair of input iterators 
            @tparam InputIterator is any input iterator type 
            @param it is an iterator referring to the first element
            @param it_stop is an iterator referring to one after the last element
         @code
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector
         std::vector<int> v {1,9,2,8,3,7,4,6,5,5,4,3,2,1};
         
         // Initialize entries using iterators
         dsl::shrinking_set_unordered<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Create new vector of values {-10,-9,...,9,10}
         std::vector<int> v3 {-10,10,-9,9,-8,8,-7,7,-6,6,-5,5,-4,4};
         
         // Reinitialize shrinking set with those values
         v2.reinitialize(std::begin(v3), std::end(v3));
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,9,2,8,3,7,4,6,5}
         {-10,10,-9,9,-8,8,-7,7,-6,6,-5,5,-4,4}
         @endcode
         */
        template<typename InputIterator>
        void reinitialize(InputIterator it, InputIterator it_stop) {
            
            // Clear contents, reinitilize
            elements.clear();
            std::copy( it, it_stop, std::back_inserter(elements));
            
            start = std::begin(elements);
            stop = std::end(elements);
            
            // remove duplicate entries
            auto stop_it = std::unique_copy(start, stop, start);
            stop = elements.erase(stop_it,stop);
        }
        
        /** Find a particular element inside of the container.
            @tparam U is any type which can be cast to type T
            @param t is the value of an element to search for
            @returns iterator to location, or to end of the container
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // dcacadaba
         auto letters {'d','c','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set_unordered<char> v(std::begin(letters), std::end(letters));
         dsl::print(v,"\n");
         
         // Return iterator to character c
         auto it = v.find('c');
         
         // print all letters from 'c' until the end.
         std::for_each(it, std::end(v), [](char c) { std::cout <<c<<",";});
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {d,c,a,b}
         c,a,b,
         @endcode
        */
        template<typename U>
        typename std::vector<T>::iterator find(U&& t) {
            return std::find(start, stop, t);
        }
        
        /** Erases an element from the list
            @tparam U is any type which can be cast to type T
            @param t is the value of an element to erase
            @returns true if element was in the list and erased, false otherwise
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         v.erase(3);
         v.erase(6);
         v.erase(9);
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,1,4,5,9,2,6,8}
         {8,1,4,5,2}
         @endcode
        */
        template<typename U>
        bool erase(U&& t) {
            
            // Search for the element first
            auto x = std::find(start, stop, t);
            
            // If element found, erase
            if(x != stop) {
                --stop;
                std::swap(*x, *stop);
                return true;
            }
            
            // else, element not found, return false.
            return false;
        }
        
        
        /** Removes all elements which return true when input into the unary predicate function
         @tparam UnaryPredicate is any function object type
         @param pred is an object with a operator(T)->bool function defined
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         // Instead of these lines ...
         //v.erase(3);
         //v.erase(6);
         //v.erase(9);
         
         v.remove_if( [](int a) { return a%3 == 0;});
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,1,4,5,9,2,6,8}
         {8,1,4,5,2}
         @endcode
         */
        template<typename UnaryPredicate>
        void remove_if(UnaryPredicate pred) {
            //auto start = std::begin(neighbors12); auto stop = std::end(neighbors12);
            
            // starting with the first element ...
            auto it = start;
            bool flag;
            
            // for each element in the collection ...
            while(it != stop) {
                
                // Check if the predicate is satisfied ...
                if(pred(*it))
                    flag = erase(*it);  // erase if possible
                else
                    flag = false;
                
                // If no element was was erased, increment to next.
                if(!flag)
                    ++it;
                
                // If an element was erased, start may now point to another value which could also be erased.
            }
            
        }

        
        /** Unerases the element erased.
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         // Instead of these lines ...
         //v.erase(3);
         //v.erase(6);
         //v.erase(9);
         
         v.remove_if( [](int a) { return a%3 == 0;});
         
         dsl::print(v,"\n");
         
         // Oops!  We wanted to keep the 9.
         v.unerase();
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,1,4,5,9,2,6,8}
         {8,1,4,5,2}
         {8,1,4,5,2,6}
         @endcode
        */
        void unerase() {
            
            // Check to make sure there is at least one erased element
            if(stop != std::end(elements))
                ++stop;
        }
        
        
        /** Random access operator
         @param index is the element index
         @returns the value at the given index
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         std::cout << "First elements: " << v[0] << std::endl;
         
         std::cout << "Sum of first three elements: " << v[0]+v[1]+v[2] << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,1,4,5,9,2,6,8}
         First elements: 3
         Sum of first three elements: 8
         @endcode
         */
        T& operator[](size_t index) { return elements[index]; };
        
        /** Resets the set to have all elements again
            @param flag specifies whether the list should be resorted or left in unsorted order
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Three little ducks went out one day ...
         dsl::shrinking_set_unordered<int> v {3,2,1};
         dsl::print(v,"\n");
         
         // over the bridge and far away ...
         v.erase(3);
         
         // Mother duck said, "Quack quack quack"
         // Two little ducks came back
         dsl::print(v,"\n");
         
         // Two little ducks went out one day, over the bridge and far away ...
         v.erase(1);
         
         // Mother duck said, "Quack quack quack"
         // One little duck came back
         dsl::print(v,"\n");
         
         // One little duck went out one day, over the bridge and far away ...
         v.erase(2);
         
         // Mother duck said, "Quack quack quack"
         // No little ducks came back
         dsl::print(v,"\n");
         
         // Sad mother duck went out one day, over the bridge and far away ...
         // Mother duck said, "Quack quack quack"
         v.reset();
         
         // All the little ducks came back! (In possibly different order!)
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,2,1}
         {1,2}
         {2}
         {}
         {2,1,3}
         @endcode
        */
        void reset(bool flag = false){
            
            // Simply reassign stop to the end of the continer
            stop = std::end(elements);
            
            // if flag is set to true, then perform sort
            if(flag)
                std::sort(start, stop);
        };
        
        /** returns the size of the set
            @returns the size of the set
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::cout << "size of v: "<<v.size() << std::endl;;
         
         v.remove_if([](int a) { return a%3==0;});
         
         std::cout << "After removing all multiples of 3\n";
         std::cout << "size of v: "<<v.size() << std::endl;
         
         return 0;
         }

         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         size of v: 9
         After removing all multiples of 3
         size of v: 6
         @endcode
        */
        size_t size() { return stop-start; };
        
        /** Check whether or not the container is empty
            @returns true if the container is empty
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::cout << "Now remove all multiples of 2, 3, or 5 \n";
         v.remove_if([](int a) { return a%3==0;});
         v.remove_if([](int a) { return a%2==0;});
         v.remove_if([](int a) { return a%5==0;});
         
         std::cout << "After removing all multiples of 2, 3, or 5\n";
         std::cout << "is v empty?  "<< (v.empty()? "yes" : "no" ) << std::endl;
         
         if(!v.empty()) dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,2,1,4,5,6,7,8,0}
         Now remove all multiples of 2, 3, or 5
         After removing all multiples of 2, 3, or 5
         is v empty?  no
         {7,1}
         @endcode
        */
        bool empty() { return start==stop; };
        
        /** returns iterator to the first element of the collection
         @returns iterator to the first element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::vector<int> v2;
         
         // copy all even elements to container v2
         std::copy_if(std::begin(v), std::end(v), std::back_inserter(v2),[](int a) { return a%2==0;});
         
         dsl::print(v2,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,2,1,4,5,6,7,8,0}
         {2,4,6,8,0}
         @endcode
         */
        typename std::vector<T>::iterator begin() const { return start; }
        
        /** returns iterator to the one after the last element of the collection 
         @returns iterator to the last element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::vector<int> v2;
         
         // copy all even elements to container v2
         std::copy_if(std::begin(v), std::end(v), std::back_inserter(v2),[](int a) { return a%2==0;});
         
         dsl::print(v2,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,2,1,4,5,6,7,8,0}
         {2,4,6,8,0}
         @endcode
         */
        typename std::vector<T>::iterator end() const { return stop; }
        
        
        /** returns iterator to the first element of the collection
         @returns iterator to the first element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         // Print out the squares of each value
         
         // C++14 syntax
         //std::for_each(std::cbegin(v), std::cend(v), [](int a) { return std::cout << a*a << ",";});
         
         std::for_each(v.cbegin(), v.cend(), [&](int a) { std::cout << a*a << ",";});
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,2,1,4,5,6,7,8,0}
         9,4,1,16,25,36,49,64,0,
         @endcode
         */
        typename std::vector<T>::const_iterator cbegin() const { return start; }
        
        /** returns iterator to the one after the last element of the collection
         @returns iterator to the last element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set_unordered<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         // Print out the squares of each value
         
         // C++14 syntax
         //std::for_each(std::cbegin(v), std::cend(v), [](int a) { return std::cout << a*a << ",";});
         
         std::for_each(v.cbegin(), v.cend(), [&](int a) { std::cout << a*a << ",";});
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {3,2,1,4,5,6,7,8,0}
         9,4,1,16,25,36,49,64,0,
         @endcode*/
        typename std::vector<T>::const_iterator cend() const { return stop; }
        
        
        
    private:
        std::vector<T> elements;
        typename std::vector<T>::iterator start;
        typename std::vector<T>::iterator stop;
    };
    
    
    
    /** @class shrinking_set
     @brief initialized with a set of objects, then efficiently erases and resets again keeping non-erased elements in sorted order
     
     @tparam T is the underlying type of objects
     
     @details The motivation for this class is from random sampling of Sudoku matrices and Latin squares, where
     one starts with a set of objects {1,2,...,n} and then erases elements one at a time according to some
     set of constraints. \n
     
     If you just use std::set, then there are a lot of memory management costs.  Instead, the idea is to always store the entire
     set of objects, and then rotate elements at the end to maintain sorted order.  For example, suppose we start with {1,2,...,9} and then eliminate the
     5, then 7, then 8 by rotating and updating a pointer.  The set would store its elements in a vector with the following order \n
     {1,2,3,4,5,6,7,8,9}\n
     {1,2,3,4,6,7,8,9,5}\n
     {1,2,3,4,6,8,9,7,5}\n
     {1,2,3,4,6,9,8,7,5}\n
     
     The size of the list and valid set of elements is kept track of by a pointer which points to one after the last element in the allowable list parts.  It starts out pointing to one after the 9, then at the 5, then at the 7, then at the 8.  To the user of the class, it will appear as though the list consists of the set of elements\n
     
     {1,2,3,4,5,6,7,8,9}\n
     {1,2,3,4,6,7,8,9}\n
     {1,2,3,4,6,8,9}\n
     {1,2,3,4,6,9}\n
     
     When reset is called, the pointer returns to one after the last position, or equivalent to std::end(...), and the initial order is restored, and so when reset is called at this point the user of the class now has \n
     
     {1,2,3,4,5,6,7,8,9} \n
     
     which contains all elements in the specified order.
     
     @code
     
     ddsl::shrinking_set_unordered<short> row({1,2,3,4,5,6,7,8,9});
     
     cout << row << endl;
     
     row.erase(3);
     row.erase(6);
     row.erase(6);
     row.erase(8);
     row.erase(7);
     row.erase(1);
     row.erase(2);
     row.erase(3);
     row.erase(4);
     row.erase(5);
     row.erase(6);
     row.erase(7);
     row.erase(8);
     row.erase(5);
     row.erase(6);
     row.erase(7);
     row.erase(8);
     cout << row << endl;
     
     row.reset(true);
     row.erase(9);
     row.erase(8);
     
     row.erase(7);
     row.erase(8);
     
     cout << row << endl;
     @endcode
     
     @code
     std::vector<short> v {1,2,3,4,5,6,7,8,9};
     
     //ddsl::shrinking_set_unordered<short> row(std::begin(v), std::end(v));
     ddsl::shrinking_set<short> row(std::begin(v), std::end(v));
     
     cout << row << endl;
     
     row.erase(3);
     row.erase(6);
     row.erase(6);
     row.erase(8);
     row.erase(1);
     row.erase(4);
     row.erase(2);
     cout << row << endl;
     
     row.reset();
     row.erase(9);
     row.erase(1);
     row.erase(8);
     row.erase(3);
     
     //row.erase(7);
     //row.erase(10);
     
     cout << row << endl;
     
     row.unerase();
     cout << row << endl;
     
     auto x = row.find(1);
     
     if(x != std::end(row))
     cout << *x << std::endl;
     else
     cout << "element not found " << std::endl;
     
     std::vector<int> v2 = dsl::range(100);
     
     auto x2 = dsl::binary_search_iterator(std::begin(v2), std::end(v2), 27);
     
     std::cout << *x2 << std::endl;
     
     row.remove_if([](int a) { return a <=5;});
     
     std::cout << row << std::endl;
     @endcode
     
     */
    template<typename T, typename Comparison=std::less<T> >
    class shrinking_set {
        
        /** output operator, outputs of the form {#1,#2,...,#n}, where #1<#2<...<#n, specified by the Comparison template
         @param out is the stream
         @param s is the collection of objects
         @returns the stream for chaining
         @code
         #include <initializer_list>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create default set of size 0;
         dsl::shrinking_set<char> v;
         
         // Initialize set with letters a,b,c,d,e,f
         // Cannot just use initializer list, since by default each character is a const char*
         dsl::shrinking_set<char> v2(std::vector<char>({'f','e','c','d','a','b'}));
         
         dsl::print(v,"\n");
         dsl::print(v2);
         
         return 0;
         }
         @endcode
         Should produce output
         @code 
         {}
         {a,b,c,d,e,f}
         @endcode
         */
        friend std::ostream& operator<<(std::ostream& out, const shrinking_set<T,Comparison>& s) {
            out << "{";
            
            auto start = s.begin();
            auto stop = s.end();
            
            if(start != stop) {
                auto it = start;
                auto pen = stop-1;
                while(it != pen) {
                    out << *it++ << ",";
                }
                out << *it;
                
            }
            out << "}";
            return out;
        }

    public:
        
        
        /** Constructs empty collection
         @code
         #include <initializer_list>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create default set of size 0;
         dsl::shrinking_set<char> v;
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {}
         @endcode
         */
        shrinking_set() {
            start = std::begin(elements);
            stop = std::end(elements);
        }
        
        /** Constructs collection of objects using an initializer list of values in any order
         @param list is the collection of objects
         @code
         
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Initialize set with letters a,b,c,d,e,f in any order
         dsl::shrinking_set<char> v3 {'f','e','c','d','a','b'};
         dsl::print(v3,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d,e,f}
         @endcode
        */
        shrinking_set(std::initializer_list<T>&& list) : elements(list) {
            start = std::begin(elements);
            stop = std::end(elements);
            std::sort(start, stop, less);
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        };
        
        /** Constructs collection of objects using any object which can be copy constructed by an std::vector
         @tparam U is the type of the collection of objects
         @param list is the collection of objects
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // abacadaba
         auto letters {'a','b','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set<char> v(letters);
         dsl::print(v,"\n");
         
         std::list<char> list_of_chars {'z','y','x','w'};
         
         dsl::shrinking_set<char> v2(list_of_chars);
         dsl::print(v2,"\n");
         
         std::set<char> set_of_chars {'l','m','a','o'};
         dsl::shrinking_set<char> v3(set_of_chars);
         dsl::print(v3,"\n");
         
         std::array<int,5> array_of_ints {3,1,4,1,5};
         dsl::shrinking_set<int> v4(array_of_ints);
         dsl::print(v4,"\n");
         
         double p[3] {3.14159265, 2.718281828, 2};
         dsl::shrinking_set<double> v5(p);
         dsl::print(v5,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d}
         {w,x,y,z}
         {a,l,m,o}
         {1,3,4,5}
         {2,2.71828,3.14159}
         @endcode
         */
        template<typename U>
        shrinking_set(U&& list) {
            
            std::copy(std::begin(list), std::end(list), std::back_inserter(elements));
            start = std::begin(elements);
            stop = std::end(elements);
            std::sort(start, stop, less);
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        };
        
        
        /** Constructs collection of objects using a range of values specified by input iterators
         @tparam InputIterator is any input iterator type
         @param it is an iterator referring to the first element
         @param it_stop is an iterator referring to one after the last element
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // abacadaba
         auto letters {'a','b','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set<char> v(std::begin(letters), std::end(letters));
         dsl::print(v,"\n");
         
         std::list<char> list_of_chars {'z','y','x','w'};
         
         dsl::shrinking_set<char> v2(std::begin(list_of_chars),std::end(list_of_chars));
         dsl::print(v2,"\n");
         
         std::set<char> set_of_chars {'l','m','a','o'};
         dsl::shrinking_set<char> v3(std::begin(set_of_chars),std::end(set_of_chars));
         dsl::print(v3,"\n");
         
         std::array<int,5> array_of_ints {3,1,4,1,5}; // random access iterator
         dsl::shrinking_set<int> v4(std::begin(array_of_ints),std::end(array_of_ints)-2);
         dsl::print(v4,"\n");
         
         double p[3] {3.14159265, 2.718281828, 2}; // random access iterator
         dsl::shrinking_set<double> v5(std::begin(p), std::end(p)-1);
         dsl::print(v5,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d}
         {w,x,y,z}
         {a,l,m,o}
         {1,3,4}
         {2.71828,3.14159}
         @endcode
         */
        template<typename InputIterator>
        shrinking_set(InputIterator it, InputIterator it_stop) {
            
            // push back elements from input iterators
            while(it != it_stop) {
                elements.push_back(*it++);
            }
            
            start = std::begin(elements);
            stop = std::end(elements);
            std::sort(start, stop, less);
            
            auto clean_it = std::unique_copy(start, stop, start);
            stop = elements.erase(clean_it,stop);
        };

        /** reinitialize using a vector of the same type
            @param list is another collection of objects
         @code
         #include <numeric>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector with entries {1,2,...,9}
         std::vector<int> v(9);
         std::iota(std::begin(v), std::end(v), 1);
         
         // Initialize entries using iterators
         dsl::shrinking_set<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Create new vector of values {-10,-9,...,9,10}
         std::vector<int> v3(21);
         std::iota(std::begin(v3), std::end(v3), -10);
         
         // Reinitialize shrinking set with those values
         v2.reinitialize(v3);
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,7,8,9}
         {-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10}
         @endcode
        */
        void reinitialize(const std::vector<T>& list) {
            
            //elements.resize(list.size());
            //std::copy(std::begin(list), std::end(list), std::begin(elements));
            
            elements = list;
            start = std::begin(elements);
            stop = std::end(elements);
            std::sort(start, stop, less);
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        }
        
        /** reinitialize using an rvalue reference of the same type
            @param list is an expiring collection of objects
         @code
         #include <numeric>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector with entries {1,2,...,9}
         std::vector<int> v(9);
         std::iota(std::begin(v), std::end(v), 1);
         
         // Initialize entries using iterators
         dsl::shrinking_set<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Reinitialize shrinking set with custom values
         v2.reinitialize({1,2,3,4,5});
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,7,8,9}
         {1,2,3,4,5}
         @endcode
        */
        void reinitialize(std::vector<T>&& list) {
            
            elements = list;
            /*elements.resize(list.size());
             std::copy(std::begin(list), std::end(list), std::begin(elements));*/
            start = std::begin(elements);
            stop = std::end(elements);
            std::sort(start, stop, less);
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        }

        
        /** reinitialize using another collection of objects which supplies at least an input iterator
         @tparam U is a collection of objects with an input iterator
         @param list is the collection of objects
         @code
         #include <numeric>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector with entries {1,2,...,9}
         std::vector<int> v(9);
         std::iota(std::begin(v), std::end(v), 1);
         
         // Initialize entries using iterators
         dsl::shrinking_set<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Reinitialize shrinking set with custom values
         v2.reinitialize({4,5,3,2,1});
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,7,8,9}
         {1,2,3,4,5}
         @endcode
         */
        template<typename U>
        void reinitialize(U&& list) {
            
            elements.clear();
            std::copy(std::begin(list), std::end(list), std::back_inserter(elements)); //std::begin(elements));
            start = std::begin(elements);
            stop = std::end(elements);
            std::sort(start, stop, less);
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
            
        }
        
        /** reinitialize using another pair of input iterators
         @tparam InputIterator is any input iterator type
         @param it is an iterator referring to the first element
         @param it_stop is an iterator referring to one after the last element
         @code
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector
         std::vector<int> v {1,9,2,8,3,7,4,6,5,5,4,3,2,1};
         
         // Initialize entries using iterators
         dsl::shrinking_set<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Create new vector of values {-10,-9,...,9,10}
         std::vector<int> v3 {-10,10,-9,9,-8,8,-7,7,-6,6,-5,5,-4,4};
         
         // Reinitialize shrinking set with those values
         v2.reinitialize(std::begin(v3), std::end(v3));
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,7,8,9}
         {-10,-9,-8,-7,-6,-5,-4,4,5,6,7,8,9,10}
         @endcode
         */
        template<typename InputIterator>
        void reinitialize(InputIterator it, InputIterator it_stop) {
            
            // Clear contents, reinitilize
            elements.clear();
            std::copy( it, it_stop, std::back_inserter(elements));
            
            start = std::begin(elements);
            stop = std::end(elements);
            
            std::sort(start, stop, less);
            
            // remove duplicate entries
            auto stop_it = std::unique_copy(start, stop, start);
            stop = elements.erase(stop_it,stop);
        }
        
        /** reinitializes without sorting, specialized for std::vector since it will be slightly faster than in general
            @param list must be sorted
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         std::vector<char> ordered_name() { return std::vector<char>{'a','l'};};
         
         int main(int argc, const char * argv[]) {
         
         // abacadaba
         auto letters {'a','b','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set<char> v(std::begin(letters), std::end(letters));
         dsl::print(v,"\n");
         
         std::vector<char> vector_of_chars {'g','p','s'};
         v.reinitialize_with_ordered(vector_of_chars);
         dsl::print(v,"\n");
         
         v.reinitialize_with_ordered(ordered_name());
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d}
         {g,p,s}
         {a,l}
         @endcode
        */
        void reinitialize_with_ordered(const std::vector<T>& list) {
            
            elements(list);
            start = std::begin(elements);
            stop = std::end(elements);
            
            // Skip the unnecessary sorting step
            //std::sort(start, stop, less);
            
            // Doesn't assume duplicates have been removed
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        }
        
        /** reinitialize using sorted expiring collection, specialized for std::vector since it will be slightly faster than in general
            @param list is an expiring, sorted collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         std::vector<char> ordered_name() { return std::vector<char>{'a','l'};};
         
         int main(int argc, const char * argv[]) {
         
         // abacadaba
         auto letters {'a','b','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set<char> v(std::begin(letters), std::end(letters));
         dsl::print(v,"\n");
         
         v.reinitialize_with_ordered(ordered_name());
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d}
         {a,l}
         @endcode
        */
        void reinitialize_with_ordered(std::vector<T>&& list) {
            
            elements = std::move(list);
            start = std::begin(elements);
            stop = std::end(elements);
            
            // Skip the unnecessary sorting step
            //std::sort(start, stop, less);
            
            // Doesn't assume duplicates have been removed
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        }
        
        /** reinitialize using another collection of objects which supplies at least an input iterator, the input list MUST ALREADY BE SORTED.
         @tparam U is a collection of objects with an input iterator
         @param list is the collection of objects
         @code
         #include <numeric>
         #include "desalvo/shrinking_set.h"
         #include "desalvo/std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create vector with entries {1,2,...,9}
         std::vector<int> v(9);
         std::iota(std::begin(v), std::end(v), 1);
         
         // Initialize entries using iterators
         dsl::shrinking_set<int> v2(std::begin(v), std::end(v));
         
         dsl::print(v2, "\n");
         
         // Reinitialize shrinking set with custom values
         v2.reinitialize({1,2,3,4,5});
         
         dsl::print(v2, "\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,7,8,9}
         {1,2,3,4,5}
         @endcode
         
         Example 2:
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // abacadaba
         auto letters {'a','b','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set<char> v(std::begin(letters), std::end(letters));
         dsl::print(v,"\n");
         
         std::vector<char> vector_of_chars {'s','t','u'};
         v.reinitialize_with_ordered(vector_of_chars);
         dsl::print(v,"\n");
         
         std::list<char> list_of_chars {'w','x','y','z'};
         
         v.reinitialize_with_ordered(list_of_chars);
         dsl::print(v,"\n");
         
         std::set<char> set_of_chars {'a','b','c','d'};
         v.reinitialize_with_ordered(set_of_chars);
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d}
         {s,t,u}
         {w,x,y,z}
         {a,b,c,d}
         @endcode
         */
        template<typename U>
        void reinitialize_with_ordered(U&& list) {
            
            elements.clear();
            std::copy(std::begin(list), std::end(list), std::back_inserter(elements)); //std::begin(elements));
            start = std::begin(elements);
            stop = std::end(elements);
            
            // same as reinitialize(U&& list) except skip the sort step.  It is the responsibility of the user of this function
            // to know that the list is already sorted, it does not check.
            //std::sort(start, stop, less);
            
            auto it = std::unique_copy(start, stop, start);
            stop = elements.erase(it,stop);
        }
        
        /** Find a particular element inside of the container.
         @tparam U is any type which can be cast to type T
         @param t is the value of an element to search for
         @returns iterator to location, or to end of the container
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // abacadaba
         auto letters {'a','b','a','c','a','d','a','b','a'};
         
         dsl::shrinking_set<char> v(std::begin(letters), std::end(letters));
         dsl::print(v,"\n");
         
         // Return iterator to character c
         auto it = v.find('c');
         
         // print all letters from 'c' until the end.
         std::for_each(it, std::end(v), [](char c) { std::cout <<c<<",";});
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {a,b,c,d}
         c,d,
         @endcode
         */
        template<typename U>
        typename std::vector<T>::iterator find(U&& t) const {
            return std::lower_bound(start, stop, t);
        }
        
        
        /** Erases an element from the list
         @tparam U is any type which can be cast to type T
         @param t is the value of an element to erase
         @returns true if element was in the list and erased, false otherwise
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         v.erase(3);
         v.erase(6);
         v.erase(9);
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,8,9}
         {1,2,4,5,8}
         @endcode
         */
        template<typename U>
        bool erase(U&& t) {
            //auto x = std::find(start, stop, t);
            //auto x = std::binary_search(start, stop, t,less);
            auto x = find(t);
            
            // if found
            if(x != stop) {
                std::rotate(x, x+1, stop); // keeps elements in sorted order
                --stop;
                return true;
            }
            return false;
        }

    
        /** Removes all elements which return true when input into the unary predicate function 
            @tparam UnaryPredicate is any function object type
            @param pred is an object with a operator(T)->bool function defined
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         // Instead of these lines ...
         //v.erase(3);
         //v.erase(6);
         //v.erase(9);
         
         v.remove_if( [](int a) { return a%3 == 0;});
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,8,9}
         {1,2,4,5,8}
         @endcode
        */
        template<typename UnaryPredicate>
        void remove_if(UnaryPredicate pred) {
            //auto start = std::begin(neighbors12); auto stop = std::end(neighbors12);
            
            // starting with the first element ...
            auto it = start;
            bool flag;
            
            // for each element in the collection ...
            while(it != stop) {
            
                // Check if the predicate is satisfied ...
                if(pred(*it))
                    flag = erase(*it);  // erase if possible
                else
                    flag = false;
            
                // If no element was was erased, increment to next.
                if(!flag)
                    ++it;
                
                // If an element was erased, start may now point to another value which could also be erased.
            }
            
        }
        
        /** Unerases the element erased.
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         // Instead of these lines ...
         //v.erase(3);
         //v.erase(6);
         //v.erase(9);
         
         v.remove_if( [](int a) { return a%3 == 0;});
         
         dsl::print(v,"\n");
         
         // Oops!  We wanted to keep the 9.
         v.unerase();
         
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,8,9}
         {1,2,4,5,8}
         {1,2,4,5,8,9}
         @endcode
         */
        void unerase() {
            
            // If at least one element has been erased from the container
            if(stop != std::end(elements)) {
                
                // return iterator to where unerased element should go
                auto x = std::find_if( start, stop, [&](T& t) { return t > *stop; });
                
                // rotate element back into the collection to maintain sorted order,
                if(x < stop) {
                    std::rotate(x, stop, stop+1);
                    ++stop;
                } // or if that element is larger than all others, simply update stop
                else if(x == stop)
                    ++stop;
            }
        }
        
        
        /** Random access operator
         @param index is the element index
         @returns the value at the given index
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,1,4,1,5,9,2,6,5,3,5,8};
         dsl::print(v,"\n");
         
         std::cout << "First elements: " << v[0] << std::endl;
         
         std::cout << "Sum of first three elements: " << v[0]+v[1]+v[2] << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3,4,5,6,8,9}
         First elements: 1
         Sum of first three elements: 6
         @endcode
         */
        T& operator[](size_t index) { return elements[index]; };
        
        
        /** Resets the set to have all elements again
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Three little ducks went out one day ...
         dsl::shrinking_set<int> v {3,2,1};
         dsl::print(v,"\n");
         
         // over the bridge and far away ...
         v.erase(3);
         
         // Mother duck said, "Quack quack quack"
         // Two little ducks came back
         dsl::print(v,"\n");
         
         // Two little ducks went out one day, over the bridge and far away ...
         v.erase(1);
         
         // Mother duck said, "Quack quack quack"
         // One little duck came back
         dsl::print(v,"\n");
         
         // One little duck went out one day, over the bridge and far away ...
         v.erase(2);
         
         // Mother duck said, "Quack quack quack"
         // No little ducks came back
         dsl::print(v,"\n");
         
         // Sad mother duck went out one day, over the bridge and far away ...
         // Mother duck said, "Quack quack quack"
         v.reset();
         
         // All the little ducks came back!
         dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {1,2,3}
         {1,2}
         {2}
         {}
         {1,2,3}
         @endcode
         */
        void reset(){
            stop = std::end(elements);
            std::sort(start, stop, less);
        };
        
        
        /** returns the size of the set
         @returns the size of the set
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::cout << "size of v: "<<v.size() << std::endl;;
         
         v.remove_if([](int a) { return a%3==0;});
         
         std::cout << "After removing all multiples of 3\n";
         std::cout << "size of v: "<<v.size() << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         size of v: 9
         After removing all multiples of 3
         size of v: 6
         @endcode
         */
        size_t size() const { return stop-start; };
        
        /** Check whether or not the container is empty
         @returns true if the container is empty
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::cout << "Now remove all multiples of 2, 3, or 5 \n";
         v.remove_if([](int a) { return a%3==0;});
         v.remove_if([](int a) { return a%2==0;});
         v.remove_if([](int a) { return a%5==0;});
         
         std::cout << "After removing all multiples of 2, 3, or 5\n";
         std::cout << "is v empty?  "<< (v.empty()? "yes" : "no" ) << std::endl;
         
         if(!v.empty()) dsl::print(v,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         Now remove all multiples of 2, 3, or 5
         After removing all multiples of 2, 3, or 5
         is v empty?  no
         {1,7}
         @endcode
         */
        bool empty() const { return start==stop; };
        
        /** returns iterator to the first element of the collection
         @returns iterator to the first element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::vector<int> v2;
         
         // copy all even elements to container v2
         std::copy_if(std::begin(v), std::end(v), std::back_inserter(v2),[](int a) { return a%2==0;});
         
         dsl::print(v2,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         {0,2,4,6,8}
         @endcode
         */
        typename std::vector<T>::iterator begin() const { return start; }
        
        /** returns iterator to the one after the last element of the collection 
         @returns iterator to the last element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         std::vector<int> v2;
         
         // copy all even elements to container v2
         std::copy_if(std::begin(v), std::end(v), std::back_inserter(v2),[](int a) { return a%2==0;});
         
         dsl::print(v2,"\n");
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         {0,2,4,6,8}
         @endcode
         */
        typename std::vector<T>::iterator end() const { return stop; }
        
        /** returns iterator to the first element of the collection
         @returns iterator to the first element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         // Print out the squares of each value
         
         // C++14 syntax
         //std::for_each(std::cbegin(v), std::cend(v), [](int a) { return std::cout << a*a << ",";});
         
         std::for_each(v.cbegin(), v.cend(), [&](int a) { std::cout << a*a << ",";});
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         0,1,4,9,16,25,36,49,64,
         @endcode
         */
        typename std::vector<T>::const_iterator cbegin() const { return start; }
        
        /** returns iterator to the one after the last element of the collection 
         @returns iterator to the last element of the collection
         @code
         #include "desalvo/std_cout.h"
         #include "desalvo/shrinking_set.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::shrinking_set<int> v {3,2,1,4,5,6,7,8,0};
         dsl::print(v,"\n");
         
         // Print out the squares of each value
         
         // C++14 syntax
         //std::for_each(std::cbegin(v), std::cend(v), [](int a) { return std::cout << a*a << ",";});
         
         std::for_each(v.cbegin(), v.cend(), [&](int a) { std::cout << a*a << ",";});
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {0,1,2,3,4,5,6,7,8}
         0,1,4,9,16,25,36,49,64,
         @endcode*/
        typename std::vector<T>::const_iterator cend() const { return stop; }
        
    
    private:
        Comparison less;
        std::vector<T> elements;
        typename std::vector<T>::iterator start;
        typename std::vector<T>::iterator stop;
        
    };
    
};





#endif
