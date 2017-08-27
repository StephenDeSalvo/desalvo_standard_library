
/** @file std_cout.h
 @author Stephen DeSalvo
 @date December, 2014
 @brief Overloads for operator<< for standard library containers
 
 This is a collection of overloads to operator<< that allows for printing of collections of objects.  The default perferred format is
 {1,2,3,4,5}, that is, elements separated by commas and the entire list enclosed in { }.
 
 Only include this file if you resign yourself to the exact same style as I prefer.  Otherwise consider writing your own or waiting for another version to come out which is a bit more general.
 
 @code
 // Sample code
 
 // Overloads of operator<< for standard library containers
 #include "std_cout.h"
 
 int main(int argc, const char * argv[])
 {
 std::vector<double> v {1.2, 3.14, 5.555, -1.234};
 dsl::print("vector: ");
 dsl::print(v, dsl::start_new_line);
 
 std::list<bool> switches {true, true, false, false, true, false, false, true };
 dsl::print("list: ");
 dsl::print(switches, dsl::start_new_line);
 
 std::set<int> s;
 s.insert(5); s.insert(1); s.insert(-123); s.insert(4);
 s.insert(5); s.insert(12); s.insert(7); s.insert(4);
 dsl::print("set: ");
 dsl::print(s, dsl::start_new_line);
 
 std::multiset<int> multi;
 multi.insert(5); multi.insert(1); multi.insert(-123); multi.insert(4);
 multi.insert(5); multi.insert(12); multi.insert(7); multi.insert(4);
 dsl::print("multiset: ");
 dsl::print(multi, dsl::start_new_line);
 
 std::valarray<size_t> vals {1,12,0,4,4,3,2,9,9,7};
 dsl::print("valarray: ");
 dsl::print(vals, dsl::start_new_line);
 
 std::vector< std::pair<int, double> > vp { {1,.01}, {2,.04}, {3,1.23}, {4,-.0184} };
 dsl::print("vector of pairs: ");
 dsl::print(vp, dsl::start_new_line);
 
 auto v {1, -1, 23, -756, 222, 5, 4, -3, 77, 18};
 dsl::print("default collection: ");
 dsl::print(v,dsl::start_new_line);
 
 return 0;
 }
 @endcode
 
 The output should be:
 @code
 vector: {1.2,3.14,5.555,-1.234}
 list: {1,1,0,0,1,0,0,1}
 set: {-123,1,4,5,7,12}
 multiset: {-123,1,4,4,5,5,7,12}
 valarray: {1,12,0,4,4,3,2,9,9,7}
 vector of pairs: {{1,0.01},{2,0.04},{3,1.23},{4,-0.0184}}
 @endcode
 
 */


#ifndef ComputeFinalGrades_DeSalvoOutputLibrary_h
#define ComputeFinalGrades_DeSalvoOutputLibrary_h


#include <iostream>
#include <valarray>
#include <vector>
#include <list>
#include <algorithm>
#include <set>
#include <string>
#include <initializer_list>
#include <array>
#include <utility>

// File-scoped variables which control how collections are printed out.
// Arbitrarily chosen, same for all collections
// Prints out elements of the form {x,x,...,x}, where x can also be an element of the form {y,y,...,}, ... .
std::string cout_separation = ",";
std::string cout_open_bracket = "{";
std::string cout_close_bracket = "}";
std::string no_ending = "";

    template<typename T, typename F>
    std::ostream& operator<<(std::ostream& out, const std::pair<T,F>& vec);

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec);
    
    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::initializer_list<T>& vec);
    
    template<typename T >
    std::ostream& operator<<(std::ostream& out, const std::multiset<T>& my_list);
    
    template<typename T >
    std::ostream& operator<<(std::ostream& out, const std::set<T>& my_list);
    
    template<typename T >
    std::ostream& operator<<(std::ostream& out, const std::valarray<T>& vec);
    
    template<typename T, size_t n>
    std::ostream& operator<<(std::ostream& out, const std::array<T,n>& vec);
    
    template<typename T >
    std::ostream& operator<<(std::ostream& out, const std::slice_array<T>& vec);
    
    template<typename T >
    std::ostream& operator<<(std::ostream& out, const std::list<T>& my_list);
    
    //std::ostream& operator<<(std::ostream& out, const typename std::multiset< std::pair<size_t,size_t> >::iterator& my_it);



/**
 Templated function for output of pair<T,F> format.
 @tparam T is the first element type
 @tparam F is the second element type
 @param out is the output stream
 @param vec is the pair<T,F> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 namespace dsl = desalvo_standard_library;
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, char> p {5, 'c'};
 std::pair<double, int> p2 {3.14159, -12};
 std::pair<std::pair<double, int>, char> p3 { {-1.23, 12},{'a'}};
 
 std::cout << p << std::endl;
 std::cout << p2 << std::endl;
 std::cout << p3 << std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {5,c}
 {3.14159,-12}
 {{-1.23,12},a}
 @endcode
 */
template<typename T, typename F>
std::ostream& operator<<(std::ostream& out, const std::pair<T,F>& vec) {
    
    out << cout_open_bracket;
    out << vec.first;
    out << cout_separation;
    out << vec.second;
    out << cout_close_bracket;
    return out;
    
};




/**
 Templated function for output of std::valarray<T> format.
 @tparam T is the type of each element in the std::valarray
 @param out is the output stream
 @param vec is the vector<T> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 int main(int argc, const char * argv[]) {
 
 std::valarray<double> v {1.1,2.2,3.3,4.1,5.6,6.3,7.8};
 std::valarray<std::pair<int, int>> v2{ {1,2},{2,3},{4,5},{-1,-1},{0,-2}};
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {1.1,2.2,3.3,4.1,5.6,6.3,7.8}
 {{1,2},{2,3},{4,5},{-1,-1},{0,-2}}
 @endcode
 */
template<typename T >
std::ostream& operator<<(std::ostream& out, const std::valarray<T>& vec) {
    
    auto start = begin(vec);
    auto stop = end(vec)-1;
    
    out<<cout_open_bracket; // {
    while(start != stop) {
        out<<*start++<<cout_separation; // x,x,x,...
    }
    
    out<<*(start)<<cout_close_bracket; // x}
    
    return out;
};

/**
 Templated function for output of vector<T> format.
 @tparam T is the type of each element in each of the vectors
 @param out is the output stream
 @param vec is the vector<T> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 int main(int argc, const char * argv[]) {
 
 std::vector<int> v {1,3,7};
 std::vector<int> v2{2,6,8,9,1};
 std::vector<int> v3{1,1,1,-1};
 std::vector<int> v4{0,0,-1,1};
 
 std::vector< std::vector<int> > sv {{1,2,3},{4,5,6},{7,8,9}};
 std::vector< std::vector<int> > sv2{v,v2,v3,v4};
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 std::cout << v3 << std::endl;
 std::cout << v4 << std::endl;
 
 std::cout << sv << std::endl;
 std::cout << sv2<< std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {1,3,7}
 {2,6,8,9,1}
 {1,1,1,-1}
 {0,0,-1,1}
 {{1,2,3},{4,5,6},{7,8,9}}
 {{1,3,7},{2,6,8,9,1},{1,1,1,-1},{0,0,-1,1}}
 @endcode
 */
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    
    unsigned long n = vec.size();
    
    out<<cout_open_bracket;
    if(n) {
        for(size_t i=0;i<n-1;i++)
            out<< vec[i] <<cout_separation;
        out<< vec[n-1];
    }
    
    out<<cout_close_bracket;
    return out;
}


/**
 Templated function for output of std::list<T> format.
 @tparam is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 int main(int argc, const char * argv[]) {
 
 auto v  {1,2,3,4,5};
 auto v2  {0,0};
 auto v3  {-1,2,-1234};
 
 std::initializer_list<std::initializer_list<int>> sv {{1,2},{1,3,5,9},{0,0,-1}};
 auto sv2 {v,v2,v3};
 
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 std::cout << v2 << std::endl;
 
 std::cout << sv << std::endl;
 std::cout << sv2 << std::endl;
 
 return 0;
 }
@endcode
 Should produce output
 @code
 {1,2,3,4,5}
 {0,0}
 {0,0}
 {{1,2},{1,3,5,9},{0,0,-1}}
 {{1,2,3,4,5},{0,0},{-1,2,-1234}}
 @endcode
 */
template<typename T >
std::ostream& operator<<(std::ostream& out, const std::initializer_list<T>& my_list) {
    
    auto start = std::begin(my_list);
    auto stop = std::end(my_list)-1;
    
    out<<cout_open_bracket;
    while(start != stop) {
        out<<*start++<<cout_separation;
    }
    
    out<<*(start)<<cout_close_bracket;
    
    return out;
};


    


/**
 Templated function for output of std::array<T> format.
 @tparam T is the type of each element in the std::array
 @tparam n is the size of the std::array
 @param out is the output stream
 @param vec is the array<T> to output
 @return the output stream.
 @code
 int main(int argc, const char * argv[]) {
 
 std::array<int,5> v  {1,2,3,4,5};
 std::array<int,2> v2  {0,0};
 std::array<int,3> v3  {-1,2,-1234};
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 std::cout << v3 << std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {1,2,3,4,5}
 {0,0}
 {-1,2,-1234}
 @endcode
 */
template<typename T, size_t n>
std::ostream& operator<<(std::ostream& out, const std::array<T,n>& vec) {
    
    //unsigned long n = vec.size();
    
    out<<cout_open_bracket;
    if(n) {
        for(int i=0;i<n-1;i++)
            out<< vec[i] <<cout_separation;
        out<< vec[n-1];
    }
    out<<cout_close_bracket;
    
    return out;
};


/**
 Templated function for output of std::slice_array<T> format.
 @tparam T is the type of each element
 @param out is the output stream
 @param vec is the vector<T> to output
 @return the output stream.
 */
template<typename T >
std::ostream& operator<<(std::ostream& out, const std::slice_array<T>& vec) {
    
    std::valarray<T> v = vec;
    
    return out << v;
    
    /*
    auto start = std::begin(vec);
    auto stop = std::end(vec)-1;
    
    out<<cout_open_bracket;
    while(start != stop) {
        out<<*start++<<cout_separation;
    }
    
    out<<*(start)<<cout_close_bracket;
    
    return out;
     */
};


/**
 Templated function for output of std::list<T> format.
 @tparam is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 int main(int argc, const char * argv[]) {
 
 std::list<int> v  {1,2,3,4,5};
 std::list<int> v2  {0,0};
 std::list<int> v3  {-1,2,-1234};
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 std::cout << v3 << std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {1,2,3,4,5}
 {0,0}
 {-1,2,-1234}
 @endcode
 */
template<typename T >
std::ostream& operator<<(std::ostream& out, const std::list<T>& my_list) {
    
    auto start = std::begin(my_list);
    auto stop = --std::end(my_list);
    
    out<<cout_open_bracket;
    while(start != stop) {
        out<<*start++<<cout_separation;
    }
    
    out<<*(start)<<cout_close_bracket;
    
    return out;
};


/**
 Templated function for output of std::multiset<T> format.
 @tparam T is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 int main(int argc, const char * argv[]) {
 
 std::multiset<int> v  {1,2,3,4,5};
 std::multiset<int> v2  {0,0};
 std::multiset<int> v3  {-1,2,-1234};
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 std::cout << v3 << std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {1,2,3,4,5}
 {0,0}
 {-1234,-1,2}
 @endcode
 */
template<typename T >
std::ostream& operator<<(std::ostream& out, const std::multiset<T>& my_list) {
    
    
    typename std::multiset<T>::const_iterator start = my_list.cbegin();
    typename std::multiset<T>::const_iterator stop = --my_list.cend();
    
    //    auto stop = --std::end(my_list);
    
    out<<cout_open_bracket;
    
    if(my_list.begin() != my_list.end()) {
        while(start != stop)
            out<<*(start++)<<cout_separation;
        
        out<<*(start);
        
    }
    
    out<<cout_close_bracket;
    
    return out;
};




/**
 Templated function for output of std::set<T> format.
 @tparam T is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 @code
 #include "desalvo/std_cout.h"
 
 int main(int argc, const char * argv[]) {
 
 std::set<int> v  {1,2,3,4,5};
 std::set<int> v2  {0,0};
 std::set<int> v3  {-1,2,-1234};
 
 std::cout << v << std::endl;
 std::cout << v2 << std::endl;
 std::cout << v3 << std::endl;
 
 return 0;
 }
 @endcode
 Should produce output
 @code
 {1,2,3,4,5}
 {0}
 {-1234,-1,2}
 @endcode
 */
template<typename T >
std::ostream& operator<<(std::ostream& out, const std::set<T>& my_list) {
    
    if(my_list.size()) {
        auto start = std::begin(my_list);
        auto stop = --std::end(my_list);
        
        out<<cout_open_bracket;
        while(start != stop) {
            out<<*start++<<cout_separation;
        }
        
        out<<*(start)<<cout_close_bracket;
    }
    else
        out << "{}";
    
    return out;
};

/**
 Templated function for output of std::multiset< std::pair<size_t, size_t> >::iterator format.  Not sure why I made this function ...

 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 */
//std::ostream& operator<<(std::ostream& out, const typename std::multiset< std::pair<size_t,size_t> >::iterator& my_it) {
    
  //  return out << *my_it;
    /*
    auto start = std::begin(my_list);
    auto stop = --std::end(my_list);
    
    out<<cout_open_bracket;
    while(start != stop) {
        out<<*start++<<cout_separation;
    }
    
    out<<*(start)<<cout_close_bracket;
    
    return out;
     */
//};


namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    /** outputs the elements in a container in a default, hopefully intuitive manner, without worrying about iterators or internal data structures.
     @tparam container accepts all objects of any type (but not function pointers), it is of type universal reference
     @param ending is a string to append to the output stream after the function finishes
     @param out is the output stream.
     @code
     #include "desalvo/std_cout.h"
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     std::multiset<int> v  {1,2,3,4,5};
     std::list<int> v2  {0,0};
     std::set<int> v3  {-1,2,-1234};
     std::vector<int> v4 {3,1,4,1,5,9};
     auto v5 {2,7,1,8,2,8};
     
     dsl::print(v,"\n");
     dsl::print(v2,"\n");
     dsl::print(v3,"\n");
     dsl::print(v4,"\n");
     dsl::print(v5,"\n");
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     {1,2,3,4,5}
     {0,0}
     {-1234,-1,2}
     {3,1,4,1,5,9}
     {2,7,1,8,2,8}
     @endcode
     */
    template<typename T, typename String = std::string>
    void print(T&& container, std::string ending = "", std::ostream& out = std::cout,
               String separation = std::string(","), String open_bracket = std::string("{"),
               String close_bracket = std::string("}")) {
        
        out << container;
        out << ending;
        
    }
    
    template<typename T, typename String = std::string>
    void print(T&& container, std::string begin_with = "{", std::string separate_by = ",", std::string end_with = "}", std::ostream& out = std::cout) {
        
        auto start = std::begin(container);
        auto stop = --std::end(container);
        
        out<<begin_with;
        while(start != stop) {
            out<<*start++<<separate_by;
        }
        
        out<<*(start)<<end_with;
        
        
    }
    

}

#endif
