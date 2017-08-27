
/** @file std_cin.h
 @author Stephen DeSalvo
 @date December, 2014
 @brief Overloads for operator>> for standard library containers
 
 This is a collection of overloads to operator>> that allows for loading in of collections of objects from a file.  The default format is
 {1,2,3,4,5}, that is, elements separated by commas and the entire list enclosed in { }.
 
 Only include this file if you resign yourself to the exact same style as I prefer.  Otherwise consider writing your own or waiting for another version to come out which is a bit more general.
 
 */


#ifndef ComputeFinalGrades_DeSalvoInputLibrary_h
#define ComputeFinalGrades_DeSalvoInputLibrary_h


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
/*std::string cout_separation = ",";
std::string cout_open_bracket = "{";
std::string cout_close_bracket = "}";
std::string no_ending = "";
*/
char unused;

    template<typename T, typename F>
    std::istream& operator>>(std::istream& in, std::pair<T,F>& vec);

    template<typename T>
    std::istream& operator>>(std::istream& in, std::vector<T>& vec);
    
    template<typename T>
    std::istream& operator>>(std::istream& in, std::initializer_list<T>& vec);

    template<typename T >
    std::istream& operator>>(std::istream& in, std::multiset<T>& my_list);
    
    template<typename T >
    std::istream& operator>>(std::istream& in, std::set<T>& my_list);
    
    template<typename T >
    std::istream& operator>>(std::istream& in, std::valarray<T>& vec);
    
    template<typename T, size_t n>
    std::istream& operator>>(std::istream& in, std::array<T,n>& vec);
    
    template<typename T >
    std::istream& operator>>(std::istream& in, std::slice_array<T>& vec);
    
    template<typename T >
    std::istream& operator>>(std::istream& in, std::list<T>& my_list);
    
    //std::istream& operator>>(std::istream& in, typename std::multiset< std::pair<size_t,size_t> >::iterator& my_it);


/**
 Templated function for output of pair<T,F> format.
 @tparam T is the first element type
 @tparam F is the second element type
 @param out is the output stream
 @param vec is the pair<T,F> to output
 @return the output stream.
 @code
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T, typename F>
std::istream& operator>>(std::istream& in, std::pair<T,F>& vec) {
    
    in >> unused;
    in >> vec.first;
    in >> unused;
    in >> vec.second;
    in >> unused;
    return in;
    
};


/**
 Templated function for output of std::valarray<T> format.
 @tparam T is the type of each element in the std::valarray
 @param out is the output stream
 @param vec is the vector<T> to output
 @return the output stream.
 @code
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T >
std::istream& operator>>(std::istream& in, std::valarray<T>& vec) {
    
    // Load in values to an std::vector first, since it has push_back, then transfer to vec aftwards.
    std::vector<T> v;
    T local_value;
    
    // {#,#,...,#}
    in>>unused; // grab openening {
    while(unused != '}' && in >> local_value) {
        v.push_back(local_value);
        in >> unused;
    }

    auto n = v.size();
    vec.resize(n);
    
    // copy values over...is there a more efficient way to do this?
    for(size_t i=0;i<n;++i)
        vec[i] = v[i];
    
    return in;
};

/**
 Templated function for output of vector<T> format.
 @tparam T is the type of each element in each of the vectors
 @param out is the output stream
 @param vec is the vector<T> to output
 @return the output stream.
 @code
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& vec) {
    
    // Load in values to an std::vector first, since it has push_back, then transfer to vec aftwards.
    T local_value;
    
    // {#,#,...,#}
    in>>unused; // grab openening {
    while(unused != '}' && in >> local_value) {
        vec.push_back(local_value);
        in >> unused;
    }
    
    return in;
}


/*
 Templated function for output of std::initializer_list<T> format.
 @tparam is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 *//*
template<typename T >
std::istream& operator>>(std::istream& in, std::initializer_list<T>& my_list) {
    
    auto start = std::begin(my_list);
    auto stop = std::end(my_list)-1;
    
    in>>unused;
    while(start != stop) {
        in>>*start++>>unused;
    }
    
    in>>*(start)>>unused;
    
    return in;
};*/



/**
 Templated function for output of std::array<T> format.
 @tparam T is the type of each element in the std::array
 @tparam n is the size of the std::array
 @param out is the output stream
 @param vec is the array<T> to output
 @return the output stream.
 @code
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T, size_t n>
std::istream& operator>>(std::istream& in, std::array<T,n>& vec) {
    
    // Load in values to an std::vector first, since it has push_back, then transfer to vec aftwards.
    T local_value;

    // index for array, we assume the input is the same size as the array itself
    size_t i = 0;
    // {#,#,...,#}
    in>>unused; // grab openening {
    
    // while not in error state, and not having reached the end of values, and not having exceeded the allowable storage
    while(unused != '}' && i < n && in >> local_value) {
        vec[i++] = local_value;
        in >> unused;
    }
    
    return in;
}


/**
 Templated function for output of std::slice_array<T> format.
 @tparam T is the type of each element
 @param out is the output stream
 @param vec is the vector<T> to output
 @return the output stream.
 */
template<typename T >
std::istream& operator>>(std::istream& in, std::slice_array<T>& vec) {
    
    std::valarray<T> v = vec;
    
    return in >> v;
    
    /*
    auto start = std::begin(vec);
    auto stop = std::end(vec)-1;
    
    in>>cout_open_bracket;
    while(start != stop) {
        in>>*start++<<cout_separation;
    }
    
    in>>*(start)<<cout_close_bracket;
    
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
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T >
std::istream& operator>>(std::istream& in, std::list<T>& my_list) {
    
    // Load in values to local value and push_back into list
    T local_value;
    
    // {#,#,...,#}
    in>>unused; // grab openening {
    
    // While not in error state, and not having reached the final }
    while(unused != '}' && in >> local_value) {
        my_list.push_back(local_value); // grab #
        in >> unused; // grab ,
    }
    
    return in;
}


/**
 Templated function for output of std::multiset<T> format.
 @tparam T is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 @code
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T >
std::istream& operator>>(std::istream& in, std::multiset<T>& my_list) {
    
    T local_value;
    
    in>>unused;
    while(unused != '}' && in >> local_value) {
        my_list.insert(local_value);
        in>>unused;
    }
    
    return in;
};




/**
 Templated function for output of std::set<T> format.
 @tparam T is the type of each element
 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 @code
 // Program to generate rectangular version of Hilbert matrix
 #include "std_cin.h"
 #include "std_cout.h"
 
 
 namespace dsl = desalvo_standard_library;
 
 // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
 class Point2D {
 // output operator: std::cout << pt << std::endl;
 friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
 return out << "(" << pt.x << "," << pt.y << ")";
 }
 
 // input operator: std::cin >> pt;
 friend std::istream& operator>>(std::istream& in, Point2D& pt) {
 char unused;
 
 // format is: (x,y)
 // More generally: (char)x(char)y(char)
 in >> unused;   // (
 in >> pt.x;     //  x
 in >> unused;   //   ,
 in >> pt.y;     //    y
 in >> unused;   //     )
 
 return in;
 }
 public:
 // Initialize value of point
 Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
 
 // Default constructor, calls more general constructor, new C++11.
 Point2D() : Point2D(0,0) { };
 private:
 double x, y;
 };
 
 int main(int argc, const char * argv[]) {
 
 std::pair<int, int> my_pair;
 std::vector<int> v;
 std::set<double> v2;
 std::set<int> v3;
 std::multiset<int> v4;
 std::list<Point2D> v5;
 std::valarray<double> v6;
 std::array<Point2D,4> v7;
 
 std::cout << "Input values in the same format as the default dsl output: \n";
 std::cout << "Insert pair values: ";
 std::cin >> my_pair;
 std::cout << "Insert vector of ints: ";
 std::cin >> v;
 std::cout << "Insert set of doubles: ";
 std::cin >> v2;
 std::cout << "Insert set of ints: ";
 std::cin >> v3;
 std::cout << "Insert multiset of ints: ";
 std::cin >> v4;
 std::cout << "Insert list of Point2Ds: ";
 std::cin >> v5;
 std::cout << "Insert valarray of doubles: ";
 std::cin >> v6;
 std::cout << "Insert array of 4 Point2Ds: ";
 std::cin >> v7;
 
 std::cout << "pair stored as: " << my_pair << std::endl;
 std::cout << "vector of ints: " << v << std::endl;
 std::cout << "set of doubles: " << v2 << std::endl;
 std::cout << "set of ints: " << v3 << std::endl;
 std::cout << "multiset of ints: " << v4 << std::endl;
 std::cout << "list of Point2Ds: " << v5 << std::endl;
 std::cout << "valarray of doubles: " << v6 << std::endl;
 std::cout << "array of Point2D: " << v7 << std::endl;
 
 
 return 0;
 }
 @endcode
 Should produce output like
 @code
 Input values in the same format as the default dsl output:
 Insert pair values: {1,2}
 Insert vector of ints: {5,4,2,1}
 Insert set of doubles: {1.1,1.1,2.2,2.4,3.7}
 Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 Insert list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 Insert valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 Insert array of 4 Point2Ds: {(0,0),(-1,1),(-1,-1),(1,-1)}
 pair stored as: {1,2}
 vector of ints: {5,4,2,1}
 set of doubles: {1.1,2.2,2.4,3.7}
 set of ints: {1,2,3,4,5}
 multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
 list of Point2Ds: {(1,2),(3.4,5.4),(0,0)}
 valarray of doubles: {1.1,2.2,3.3,4.4,5.5,6.6}
 array of Point2D: {(0,0),(-1,1),(-1,-1),(1,-1)}
 @endcode
 */
template<typename T >
std::istream& operator>>(std::istream& in, std::set<T>& my_list) {
    
    T local_value;
    
    in>>unused;
    while(unused != '}' && in >> local_value) {
        my_list.insert(local_value);
        in>>unused;
    }
        
    return in;
};

/**
 Templated function for output of std::multiset< std::pair<size_t, size_t> >::iterator format.  Not sure why I made this function ...

 @param out is the output stream
 @param my_list is the list<T> to output
 @return the output stream.
 */
//std::istream& operator>>(std::istream& in, const typename std::multiset< std::pair<size_t,size_t> >::iterator& my_it) {
    
  //  return in >> *my_it;
    /*
    auto start = std::begin(my_list);
    auto stop = --std::end(my_list);
    
    in>>cout_open_bracket;
    while(start != stop) {
        in>>*start++<<cout_separation;
    }
    
    in>>*(start)<<cout_close_bracket;
    
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
    void read(T& container, std::istream& in = std::cin) {
        
        in >> container;
        
    }
    
}


#endif