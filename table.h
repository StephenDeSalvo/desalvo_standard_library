/** @file table.h
    @brief classes pertaining to 2-dimensional tables of values
 
 Provides a base class, dsl::table<ValueType,WorkingPrecision>, which can be inherited from for use with matrices or contingency tables.  Provides basic access, and supports C++11-style generic algorithms like range-based for loops.

The dsl::table stores its elements contiguously, so it can be used with many libraries requiring a matrix or table of values stored in a contiguous array.
 
 */
#ifndef desalvo_table_h
#define desalvo_table_h


#include <iostream>
#include <memory>
#include <numeric>
#include <initializer_list>

#include <vector>
#include <array>

#include "numerical.h"


namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    

    /** @class table
        @brief stores a 2-dimensional table of values
     
     Stores a contiguous collection of values, organized in a table.
     
    */
    template<typename ValueType=double, typename WorkingPrecision=long double>
    class table {
        
        /** Output operator, in the form {{#,...,#},{#,...,#},...,{#,..,#}}
         @param out is the stream object
         @param t is the table object to output
         @returns the stream object for overloading
         @code
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
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
         
         // Create a 0 x 0 matrix, i.e., empty matrix
         dsl::table<int> m;
         std::cout << m << std::endl;
         
         // Create a 3 x 4 matrix, with entries default initialized
         dsl::table<double> m2(3,4);
         
         // Create a 2 x 3 matrix of points.
         dsl::table<Point2D> m3(2,3);
         
         
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{}}
         {{0,0,0,0},
         {0,0,0,0},
         {0,0,0,0}}
         {{(0,0),(0,0),(0,0)},
         {(0,0),(0,0),(0,0)}}
         @endcode
         */
        friend std::ostream& operator<<(std::ostream& out, const table& t) {
            
            if(t.m == 0 || t.n == 0) return out << "{{}}";
            
            ValueType* v = t.entry;
            out<<"{";
            
            // i=0 first row
            out<<"{"<<*v++;
            for(size_t j=0;j<t.n-1;++j)
                out<<","<<*v++;
            out<<"},"<<std::endl;
            
            for(size_t i=1;i<t.m-1;++i) {
                out<<" {"<<*v++;
                for(size_t j=0;j<t.n-1;++j)
                    out<<","<<*v++;
                out<<"},"<<std::endl;
            }
            
            out<<" {"<<*v++;
            for(size_t j=0;j<t.n-1;++j)
                out<<","<<*v++;
            out<<"}}";
            
            return out;
        };
        
        friend bool operator==(const table& lhs, const table& rhs) {
            
            // check dimensions first
            if(lhs.m != rhs.m || lhs.n != rhs.n) return false;
            
            ValueType* lhsptr = lhs.entry;
            ValueType* rhsptr = rhs.entry;
            
            // check entry by entry
            for(size_t i=0;i<lhs.m*lhs.n;++i)
                if(*lhsptr++ != *rhsptr++) return false;
            
            return true;
        }
        
        
    private:
        ValueType* entry;
        size_t m, n;     // Dimension must be nonnegative
        
    public:
        /** Initializes the "empty" table.
         @code
         
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
         }
         
         public:
         // Initialize value of point
         Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
         
         // Default constructor, calls more general constructor, new C++11.
         //Point2D() : Point2D(0,0) { };
         private:
         double x, y;
         };
         
         int main(int argc, const char * argv[]) {
         
         dsl::table<int> m;
         std::cout << m << std::endl;
         
         // Create a 0 x 0 matrix, i.e., empty matrix.  Note that it does not default initialize anything, so the underlying type does n't have to have a default constructor defined in this case.  This is similar to making an std::set with no elements, it is only until you insert elements that you must have some kind of less than function defined.
         dsl::table<Point2D> m2;
         std::cout << m2 << std::endl;
         
         // Note: Point2D only needs a default constructor when it is default initialized.
         // The line below will not compile since the default constructor of Point2D has been commented out.
         //dsl::table<Point2D> m3(3,4);
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{}}
         {{}}
         @endcode
         */
        table() : m(0), n(0), entry(nullptr) { };
        
        /** Initialize entries to value
         @param number_of_rows is the initial number of rows
         @param number_of_columns is the initial number of columns
         @param val is the initial value for all entries.
         @code
         
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
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
         
         // Default initializes a 3x1 column of ints
         dsl::table<int> m(3);
         
         // Create a 2x2 table of Point2Ds, each default initialized to (0,0)
         dsl::table<Point2D> m2(2,2);
         
         // Create a 3x4 table of Point2Ds, each default initialized to (2.718,3.14)
         dsl::table<Point2D> m3(3,4, Point2D(2.718,3.14));
         
         
         std::cout << m << std::endl;
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0},
         {0},
         {0}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         @endcode
         */
        table(size_t number_of_rows, size_t number_of_columns=1,
               const ValueType& val = ValueType()) : m(number_of_rows), n(number_of_columns), entry(nullptr) {
            //std::cout << "New table created" << std::endl;
            try {
                entry = new ValueType[n*m];
            }
            catch(const std::exception& e) {
                delete [] entry;
            }
            
            std::for_each(entry, entry+m*n, [&](ValueType& a){ a = val; });
        };
        
        /** copy constructor
         @param initial_table is an existing table
         @code
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
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
         
         // Default initializes a 3x1 column of ints
         dsl::table<int> m(3);
         
         // Create a 2x2 table of Point2Ds, each default initialized to (0,0)
         dsl::table<Point2D> m2(2,2);
         
         // Create a 3x4 table of Point2Ds, each default initialized to (2.718,3.14)
         dsl::table<Point2D> m3(3,4, Point2D(2.718,3.14));
         
         // copy construct new tables
         auto m4(m);
         auto m5(m2);
         auto m6(m3);
         
         // Print out values
         std::cout << m << std::endl;
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         std::cout << "Again again!" << std::endl;
         std::cout << m4 << std::endl;
         std::cout << m5 << std::endl;
         std::cout << m6 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0},
         {0},
         {0}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         Again again!
         {{0},
         {0},
         {0}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         @endcode
         */
        table(const table& initial_table) : m(initial_table.m), n(initial_table.n), entry(nullptr) {
            //std::cout << "Copy made " << std::endl;
            try {
                entry = new ValueType[m*n];
            }
            catch(const std::exception& e) {
                delete [] entry;
            }
            std::copy_n(initial_table.entry, m*n, entry );
        }
        
        /** move constructor
         @param initial_table is an expiring table
         @code
         
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
         }
         
         public:
         // Initialize value of point
         Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
         
         // Default constructor, calls more general constructor, new C++11.
         Point2D() : Point2D(0,0) { };
         private:
         double x, y;
         };
         
         // Make a table whose (i,j)-th entry is the value 1./(1.+i+j).  This is called the Hilbert matrix
         dsl::table<double> hilbert_table(size_t n) {
         
         // default initialize entries to 1
         dsl::table<double> hilbert(n,n,1.);
         
         for(size_t i=0;i<hilbert.size_row();++i)
         for(size_t j=0;j<hilbert.size_column();++j)
         hilbert(i,j) = 1./(1.+i+j);
         
         return hilbert;
         }
         
         int main(int argc, const char * argv[]) {
         
         // Default initializes a 3x1 column of ints
         dsl::table<int> m(3);
         
         // Create a 2x2 table of Point2Ds, each default initialized to (0,0)
         dsl::table<Point2D> m2(2,2);
         
         // Create a 3x4 table of Point2Ds, each default initialized to (2.718,3.14)
         dsl::table<Point2D> m3(3,4, Point2D(2.718,3.14));
         
         // Presumably RVO will be applied here
         auto m4(hilbert_table(5));
         
         // move construct new tables
         auto m5(std::move(m2));
         auto m6(std::move(m3));
         
         // Print out values
         std::cout << m << std::endl;
         
         // Since we called move, m2 and m3 are in an undetermined state
         //std::cout << m2 << std::endl;
         //std::cout << m3 << std::endl;
         
         std::cout << "Hilbert table: " << m4 << std::endl;
         std::cout << m5 << std::endl;
         std::cout << m6 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0},
         {0},
         {0}}
         Hilbert table: {{1,0.5,0.333333,0.25,0.2},
         {0.5,0.333333,0.25,0.2,0.166667},
         {0.333333,0.25,0.2,0.166667,0.142857},
         {0.25,0.2,0.166667,0.142857,0.125},
         {0.2,0.166667,0.142857,0.125,0.111111}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         @endcode
        */
        table(table&& initial_table) : m(initial_table.m), n(initial_table.n), entry(initial_table.entry) {
            initial_table.entry = nullptr;
            //std::cout << "Move performed" << std::endl;
        }
        
        /** Swap two table handles
         @param other table to swap. 
         @code
         
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
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
         
         // Default initializes a 3x1 column of ints
         dsl::table<int> m(3);
         
         // Create a 2x2 table of Point2Ds, each default initialized to (0,0)
         dsl::table<Point2D> m2(2,2);
         
         // Create a 3x4 table of Point2Ds, each default initialized to (2.718,3.14)
         dsl::table<Point2D> m3(3,4, Point2D(2.718,3.14));
         
         // Create a 1x3 table with values 3
         dsl::table<int> m4(1,3,3);
         
         // Print out values
         std::cout << m << std::endl;
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         std::cout << m4 << std::endl;
         
         m.swap(m4);
         m2.swap(m3);
         
         std::cout << "After swaps: \n";
         std::cout << m << std::endl;
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         std::cout << m4 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0},
         {0},
         {0}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         {{3,3,3}}
         After swaps:
         {{3,3,3}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{0},
         {0},
         {0}}
         @endcode
         */
        void swap(table& other) {
            std::swap(m, other.m);
            std::swap(n, other.n);
            std::swap(this->entry, other.entry);
        }
        
        /** Assigns values via copy & swap idiom
         @param val is the initial value for all entries. 
         @code
         
         #include <iostream>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // custom class to handle 2D data point, in order to demonstrate how to integrate custom code with existing library.
         class Point2D {
         // output operator: std::cout << pt << std::endl;
         friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
         return out << "(" << pt.x << "," << pt.y << ")";
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
         
         // Default initializes a 3x1 column of ints
         dsl::table<int> m(3);
         
         // Create a 2x2 table of Point2Ds, each default initialized to (0,0)
         dsl::table<Point2D> m2(2,2);
         
         // Create a 3x4 table of Point2Ds, each default initialized to (2.718,3.14)
         dsl::table<Point2D> m3(3,4, Point2D(2.718,3.14));
         
         // Create a 1x3 table with values 3
         dsl::table<int> m4(1,3,3);
         
         // Print out values
         std::cout << m << std::endl;
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         std::cout << m4 << std::endl;
         
         // tables are assignable as long as they have same underlying template type, regardless of current size
         m = m4;
         m2 = m3;
         
         std::cout << "After assignment: \n";
         std::cout << m << std::endl;
         std::cout << m2 << std::endl;
         std::cout << m3 << std::endl;
         std::cout << m4 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0},
         {0},
         {0}}
         {{(0,0),(0,0)},
         {(0,0),(0,0)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         {{3,3,3}}
         After assignment:
         {{3,3,3}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         {{(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)},
         {(2.718,3.14),(2.718,3.14),(2.718,3.14),(2.718,3.14)}}
         {{3,3,3}}
         @endcode
         */
        table& operator=(table other) {
            
            // NOT std::swap, see above
            swap(other);
            return *this;
        }
        
        /* Initialize table using notation {{1,2,3},{4,5,6},{7,8,9}}
         @param init is the initializer list. */
        /*table(std::initializer_list<ValueType> init) : entry(nullptr)  {
            
            // Iterators through each row
            auto start = std::begin(init);
            auto stop = std::end(init);
            
            m = stop - start;
            n = (*start).size();
            entry = new ValueType[m*n];
            
            // Initialize t to first element in contiguous array
            auto t = std::begin(entry);
            
            // For each row ...
            while(start != stop) {
                auto st = begin(*start);
                auto sp = end(*start++);
                
                // Copy over values into t
                while(st != sp)
                    *t++ = *st++;
            }
        };
         */
        
        /** Initialize using a row-major std::array of m*n values
         @tparam VType is any type which can be cast to ValueType
         @param init is an array of values to initilize. */
        /*template<typename VType, size_t m, size_t n>
        table(std::array<VType,n*m> init) : entry(new ValueType[n*m])  {
            
            //auto start = std::begin(init);
            //auto stop = std::end(init);
            
            //auto t = std::begin(entry);
            
            //while(start != stop) {
            //    *t++ = static_cast<ValueType>(*start++);
            //}
            
            std::copy_n(std::begin(init), m*n, std::begin(entry));
            
        }*/
        
        /** Initialize using a vector of vectors
         @tparam VType is any type which can be cast to ValueType
         @param v is an std::vector of std::vector of values to initilize, the first entry of which must be an std::vector with the smallest number of entries as in the other rows.

         @code
         #include <iostream>
         #include <valarray>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         std::vector<std::vector<int>> sv {{1,2,3},{4,5,6},{7,8,9}};
         
         std::vector<std::string> v1 {"Now", "is", "the"};
         std::vector<std::string> v2 {"winter", "of", "our"};
         std::vector<std::string> v3 {"discontent", "made", "glorious"};
         std::vector<std::string> v4 {"summer", "by", "this"};
         std::vector<std::string> v5 {"sonne", "of", "york"};
         
         // Construct a table from an std::vector of std::vectors
         dsl::table<int> m(sv);
         
         // Construct a table from a collection of std::vector<std::string>
         dsl::table<std::string> richard3( std::vector<std::vector<std::string>>({v1,v2,v3,v4,v5}));
         
         // Print out values
         std::cout << m << std::endl;
         
         // Print out some text
         std::cout << richard3 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,2,3},
         {4,5,6},
         {7,8,9}}
         {{Now,is,the},
         {winter,of,our},
         {discontent,made,glorious},
         {summer,by,this},
         {sonne,of,york}}
         @endcode
         */
        template<typename VType>
        table(const std::vector<std::vector<VType>>& v) {
            //            tableIterator tstart = begin();
            m = v.size();
            
            // If empty, set entry to nullptr and return
            if(m==0) {
                n=0;
                entry = nullptr;
                return;
            }
            
            n = v[0].size();
            entry = new ValueType[n*m];
            
            ValueType* initializer = entry;
            for(size_t i=0;i<m;++i)
                for(size_t j=0;j<n;++j)
                    *initializer++ = static_cast<ValueType>(v[i][j]);
            //std::copy(std::cbegin(v), std::cend(v), entry);
        };
        
        /** Initialize using a vector of vectors
         @tparam VType is any type which can be cast to ValueType
         @param v is an array of values to initilize.
         @code
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         dsl::table<int> m ((std::vector<std::vector<int>>()));
         dsl::table<int> m2 ((std::vector<std::vector<int>>(3,{1,2,3})));
         
         // print out empty table
         std::cout << m << std::endl;
         
         // print out 3x3 table with each row {1,2,3}
         std::cout << m2 << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{}}
         {{1,2,3},
         {1,2,3},
         {1,2,3}}
         @endcode
         */
        template<typename VType>
        table(std::vector<std::vector<VType>>&& v) {
            //            tableIterator tstart = begin();
            m = v.size();

            // If empty, set entry to nullptr and return
            if(m==0) {
                entry = nullptr;
                return;
            }

            n = v[0].size();
            entry = new ValueType[n*m];
            
            ValueType* initializer = entry;
            for(size_t i=0;i<m;++i)
                for(size_t j=0;j<n;++j)
                    *initializer++ = static_cast<ValueType>(v[i][j]);
            //std::copy(std::cbegin(v), std::cend(v), entry);
        };
        
        /** Initializes a table using dimensions and an iterator at some location with at least as many values as the number of entries in the table.
         @tparam InputIterator is any input iterator
         @param number_of_rows is the number of rows
         @param number_of_columns is the number of columns
         @param start is an input iterator to a starting set of values
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v {1,2,3,4,5,6,7,8,9,10,11,12};
         
         dsl::table<int> t1(1,12,std::begin(v));
         dsl::table<int> t2(2,6,std::begin(v));
         dsl::table<int> t3(3,4,std::begin(v));
         dsl::table<int> t4(4,3,std::begin(v));
         dsl::table<int> t5(6,2,std::begin(v));
         dsl::table<int> t6(12,1,std::begin(v));
         
         std::cout << "1x12: \n" << t1 << std::endl << std::endl;
         std::cout << "2x6: \n" <<t2 << std::endl << std::endl;
         std::cout << "3x4: \n" <<t3 << std::endl << std::endl;
         std::cout << "4x3: \n" <<t4 << std::endl << std::endl;
         std::cout << "6x2: \n" <<t5 << std::endl << std::endl;
         std::cout << "12x1: \n" <<t6 << std::endl << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         1x12:
         {{1,2,3,4,5,6,7,8,9,10,11,12}}
         
         2x6:
         {{1,2,3,4,5,6},
         {7,8,9,10,11,12}}
         
         3x4:
         {{1,2,3,4},
         {5,6,7,8},
         {9,10,11,12}}
         
         4x3:
         {{1,2,3},
         {4,5,6},
         {7,8,9},
         {10,11,12}}
         
         6x2:
         {{1,2},
         {3,4},
         {5,6},
         {7,8},
         {9,10},
         {11,12}}
         
         12x1:
         {{1},
         {2},
         {3},
         {4},
         {5},
         {6},
         {7},
         {8},
         {9},
         {10},
         {11},
         {12}}
         @endcode
         */
       /* template<typename InputIterator>
        table(size_t number_of_rows, size_t number_of_columns, InputIterator start) : m(number_of_rows), n(number_of_columns), entry(nullptr) {
            
            try {
                entry = new ValueType[m*n];
            }
            catch(const std::exception& e) {
                delete [] entry;
            }
            
            // copy over values to table
            std::copy_n(start, m*n, entry);
            
        }
        */
        
        /** Initializes a 1 x n table using a pair of random access iterators.
         @tparam RandomAccessIterator is any iterator which can take the difference between two of its objects
         @param start is a random access iterator to a starting set of values
         @param stop is a random access iterator to one after the last value
         
         @code
         
         #include "std_cout.h"
         #include "table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         std::vector<size_t> v {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
         
         dsl::table<size_t> t(std::begin(v), std::end(v));
         
         std::cout << t << std::endl;
         
         return 0;
         }
         @endcode
         */
        /*
        template<typename RandomAccessIterator>
        table(RandomAccessIterator start, RandomAccessIterator stop) : m(1), n(stop-start), entry(nullptr) {
            
            try {
                entry = new ValueType[m*n];
            }
            catch(const std::exception& e) {
                delete [] entry;
            }
            
            // copy over values to table
            std::copy_n(start, m*n, entry);
            
        }
        */
        
        
        /** Initializes a 1 x n table using a pair of random access iterators.
         @tparam RandomAccessIterator is any iterator which can take the difference between two of its objects
         @param start is a random access iterator to a starting set of values
         @param stop is a random access iterator to one after the last value
         
         @code
         
         #include "std_cout.h"
         #include "table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         std::vector<size_t> v {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
         
         dsl::table<size_t> t(std::begin(v), std::end(v));
         
         std::cout << t << std::endl;
         
         return 0;
         }
         @endcode
         */
        template<typename RandomAccessIterator>
        table(RandomAccessIterator start, RandomAccessIterator stop, size_t number_of_columns) : n(number_of_columns), entry(nullptr) {
            
            size_t total_number_of_elements = stop-start;
            
            m = static_cast<size_t>(ceil(static_cast<long double>(total_number_of_elements) / static_cast<long double>(n)));
            
            try {
                entry = new ValueType[m*n]();
            }
            catch(const std::exception& e) {
                delete [] entry;
            }
            
            // copy over values to table
            std::copy_n(start, m*n, entry);
            
        }
        
        /** virtual destructors deletes entry memory. */
        virtual ~table() { delete [] entry; };
        
        /** Returns the position of the entry.
         @returns the pointer to the initial value in the entry.
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // make a 3x3 table with rows {1,2,3}
         dsl::table<int> m ((std::vector<std::vector<int>>(3,{1,2,3})));
         
         // get a pointer to first element
         // VERY DANGEROUS!  This allows you to discretely change the values in m without accessing m directly.
         auto p = m.get();
         
         // print out 3x3 table with each row {1,2,3}
         std::cout << m << std::endl;
         
         // square each of the values in m
         std::for_each(p,p+9,[](int& a) { a*=a;});
         
         // notice there was no mention of m in between these two outputs, and yet ...
         std::cout << m << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,2,3},
         {1,2,3},
         {1,2,3}}
         {{1,4,9},
         {1,4,9},
         {1,4,9}}
         @endcode
         */
        ValueType* get() const { return entry; };
        
        /** returns number of rows
            @returns the number of rows
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // Make a table whose (i,j)-th entry is the value 1./(1.+i+j).  This is called the Hilbert matrix
         dsl::table<double> hilbert_table(size_t n) {
         
         // default initialize entries to 1
         dsl::table<double> hilbert(n,n,1.);
         
         for(size_t i=0;i<hilbert.size_row();++i)
         for(size_t j=0;j<hilbert.size_column();++j)
         hilbert(i,j) = 1./(1.+i+j);
         
         return hilbert;
         }
         
         int main(int argc, const char * argv[]) {
         
         // make a Hilbert matrix
         auto hilbert = hilbert_table(7);
         
         // print out the Hilbert matrix as a table of values.
         std::cout << hilbert << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,0.5,0.333333,0.25,0.2,0.166667,0.142857},
         {0.5,0.333333,0.25,0.2,0.166667,0.142857,0.125},
         {0.333333,0.25,0.2,0.166667,0.142857,0.125,0.111111},
         {0.25,0.2,0.166667,0.142857,0.125,0.111111,0.1},
         {0.2,0.166667,0.142857,0.125,0.111111,0.1,0.0909091},
         {0.166667,0.142857,0.125,0.111111,0.1,0.0909091,0.0833333},
         {0.142857,0.125,0.111111,0.1,0.0909091,0.0833333,0.0769231}}
         @endcode
        */
        size_t size_row() const { return m; }
        
        
        /** returns number of columns
         @returns the number of columns
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // Make a table whose (i,j)-th entry is the value 1./(1.+i+j).  This is called the Hilbert matrix
         dsl::table<double> hilbert_table(size_t n) {
         
         // default initialize entries to 1
         dsl::table<double> hilbert(n,n,1.);
         
         for(size_t i=0;i<hilbert.size_row();++i)
         for(size_t j=0;j<hilbert.size_column();++j)
         hilbert(i,j) = 1./(1.+i+j);
         
         return hilbert;
         }
         
         int main(int argc, const char * argv[]) {
         
         // make a Hilbert matrix
         auto hilbert = hilbert_table(7);
         
         // print out the Hilbert matrix as a table of values.
         std::cout << hilbert << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,0.5,0.333333,0.25,0.2,0.166667,0.142857},
         {0.5,0.333333,0.25,0.2,0.166667,0.142857,0.125},
         {0.333333,0.25,0.2,0.166667,0.142857,0.125,0.111111},
         {0.25,0.2,0.166667,0.142857,0.125,0.111111,0.1},
         {0.2,0.166667,0.142857,0.125,0.111111,0.1,0.0909091},
         {0.166667,0.142857,0.125,0.111111,0.1,0.0909091,0.0833333},
         {0.142857,0.125,0.111111,0.1,0.0909091,0.0833333,0.0769231}}
         @endcode
         */
        size_t size_column() const { return n; }
        
        /** Returns a pair of values corresponding to dimension, i.e., {# rows, #columns}
         @returns a pair {#rows,#columns}
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // Make a table whose (i,j)-th entry is the value 1./(1.+i+j).  This is called the Hilbert matrix
         dsl::table<double> hilbert_table(size_t n) {
         
         // default initialize entries to 1
         dsl::table<double> hilbert(n,n,1.);
         
         for(size_t i=0;i<hilbert.size_row();++i)
         for(size_t j=0;j<hilbert.size_column();++j)
         hilbert(i,j) = 1./(1.+i+j);
         
         return hilbert;
         }
         
         int main(int argc, const char * argv[]) {
         
         // make a Hilbert matrix
         auto hilbert = hilbert_table(7);
         
         auto dim = hilbert.size();
         
         std::cout << "We made a " << dim.first << " x " << dim.second << " Hilbert matrix." << std::endl;
         
         std::cout << hilbert << std::endl;
         
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         We made a 7 x 7 Hilbert matrix.
         {{1,0.5,0.333333,0.25,0.2,0.166667,0.142857},
         {0.5,0.333333,0.25,0.2,0.166667,0.142857,0.125},
         {0.333333,0.25,0.2,0.166667,0.142857,0.125,0.111111},
         {0.25,0.2,0.166667,0.142857,0.125,0.111111,0.1},
         {0.2,0.166667,0.142857,0.125,0.111111,0.1,0.0909091},
         {0.166667,0.142857,0.125,0.111111,0.1,0.0909091,0.0833333},
         {0.142857,0.125,0.111111,0.1,0.0909091,0.0833333,0.0769231}}
         @endcode
        */
        std::pair<size_t,size_t> size() const { return std::make_pair(m,n); };
        
        /** random access of element (i,j) in the table, with indices starting at 0
         @param i is the row number
         @param j is the column number
         @returns reference to the value in the matrix
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // Make a table whose (i,j)-th entry is the value 1./(1.+i+j).  This is called the Hilbert matrix
         dsl::table<double> hilbert_table(size_t n) {
         
         // default initialize entries to 1
         dsl::table<double> hilbert(n,n,1.);
         
         for(size_t i=0;i<hilbert.size_row();++i)
         for(size_t j=0;j<hilbert.size_column();++j)
         hilbert(i,j) = 1./(1.+i+j);
         
         return hilbert;
         }
         
         int main(int argc, const char * argv[]) {
         
         // make a Hilbert matrix
         auto hilbert = hilbert_table(7);
         
         dsl::table<int> m(3,4);
         m(0,0) = 4;
         m(1,2) = 3;
         m(2,2) = 7;
         
         std::cout << hilbert << std::endl;
         std::cout << m << std::endl;
         
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,0.5,0.333333,0.25,0.2,0.166667,0.142857},
         {0.5,0.333333,0.25,0.2,0.166667,0.142857,0.125},
         {0.333333,0.25,0.2,0.166667,0.142857,0.125,0.111111},
         {0.25,0.2,0.166667,0.142857,0.125,0.111111,0.1},
         {0.2,0.166667,0.142857,0.125,0.111111,0.1,0.0909091},
         {0.166667,0.142857,0.125,0.111111,0.1,0.0909091,0.0833333},
         {0.142857,0.125,0.111111,0.1,0.0909091,0.0833333,0.0769231}}
         {{4,0,0,0},
         {0,0,3,0},
         {0,0,7,0}}
         @endcode
        */
        ValueType& operator()(long int i, long int j) const {
            
            // Check for out of bounds
            if(i < 0 || j < 0 || i >= m || j >= n)
                throw std::runtime_error{"Out of bounds, attempted to access index ("+std::to_string(i)+","+std::to_string(j)+")\n"};
            
            return entry[i*n+j];
            
        }
        
        
        /** Check if the matrix is all 0s
         @returns true if each entry in the matrix is equivalent to ValueType(0)
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         // Make a table whose (i,j)-th entry is the value 1./(1.+i+j).  This is called the Hilbert matrix
         dsl::table<double> hilbert_table(size_t n) {
         
         // default initialize entries to 1
         dsl::table<double> hilbert(n,n,1.);
         
         for(size_t i=0;i<hilbert.size_row();++i)
         for(size_t j=0;j<hilbert.size_column();++j)
         hilbert(i,j) = 1./(1.+i+j);
         
         return hilbert;
         }
         
         int main(int argc, const char * argv[]) {
         
         // make a Hilbert matrix
         auto hilbert = hilbert_table(7);
         
         // makes matrix with custom values
         dsl::table<int> m(3,4);
         m(0,0) = 4;
         m(1,2) = 3;
         m(2,2) = 7;
         
         // default initialized to all 0s
         dsl::table<int> m2(5,5);
         
         std::cout << hilbert.is_zero() << std::endl;
         std::cout << m.is_zero() << std::endl;
         std::cout << m2.is_zero() << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         0
         0
         1
         @endcode
        */
        bool is_zero() const {
            return std::all_of(entry, entry+m*n, [](const ValueType& i){ return (i==static_cast<ValueType>(0)); });
        }
        
        
        // TODO: Rethink the purpose of this, maybe replace or add a function called reset, which will reset values to the default initialized value.
        // SHOULD clear also set the dimensions to 0?  Or does it just clear out the values?
        // Should I even have this function?
        void set_all_values_to(const ValueType& new_value) {
            
            // Set each value to the new value
            std::for_each(entry, entry+m*n, [&](ValueType& a) { a=new_value; });
        };
        
        /** returns a pointer to the first element
         @returns a pointer to the first element
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v {1,2,3,4,5,6,7,8,9,10,11,12};
         
         // You don't have to use all of the values in v
         dsl::table<int> t(3,3,std::begin(v));
         
         std::cout << t << std::endl;
         
         // We now have complete, unfettered access to the elements in t.
         auto p = t.begin_raw();
         auto p2 = t.end_raw();
         
         // This is useful if we wish to use a routine from another library, e.g., a matrix library, that requires two pointers referring to a contiguous array of values.
         
         // This routine rearranges values into the transpose, the last parameter is the size of each row
         dsl::transpose(p,p2,3);
         
         // print out the transposed table
         std::cout << t << std::endl;
         
         // const iterators, so can access values but cannot alter them.
         auto p3 = t.cbegin_raw();
         auto p4 = t.cend_raw();
         
         std::for_each(p3,p4,[](int a) { std::cout << a << ","; }); std::cout<<std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,2,3},
         {4,5,6},
         {7,8,9}}
         {{1,4,7},
         {2,5,8},
         {3,6,9}}
         1,4,7,2,5,8,3,6,9,
         @endcode
         */
        ValueType* begin_raw() { return entry; };
        
        /**returns a pointer to one after the last element
         @returns a pointer to one after the last element
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v {1,2,3,4,5,6,7,8,9,10,11,12};
         
         // You don't have to use all of the values in v
         dsl::table<int> t(3,3,std::begin(v));
         
         std::cout << t << std::endl;
         
         // We now have complete, unfettered access to the elements in t.
         auto p = t.begin_raw();
         auto p2 = t.end_raw();
         
         // This is useful if we wish to use a routine from another library, e.g., a matrix library, that requires two pointers referring to a contiguous array of values.
         
         // This routine rearranges values into the transpose, the last parameter is the size of each row
         dsl::transpose(p,p2,3);
         
         // print out the transposed table
         std::cout << t << std::endl;
         
         // const iterators, so can access values but cannot alter them.
         auto p3 = t.cbegin_raw();
         auto p4 = t.cend_raw();
         
         std::for_each(p3,p4,[](int a) { std::cout << a << ","; }); std::cout<<std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,2,3},
         {4,5,6},
         {7,8,9}}
         {{1,4,7},
         {2,5,8},
         {3,6,9}}
         1,4,7,2,5,8,3,6,9,
         @endcode
         */
        ValueType* end_raw() { return entry+m*n; };
        
        /** Returns a raw pointer to a given row of a table
         @param i is the row number, indexed starting at 0
         @returns a raw pointer to the first element of a given row
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         std::iota(std::begin(v), std::end(v), 0);
         
         // You don't have to use all of the values in v
         dsl::table<int> t(10,10,std::begin(v));
         
         std::cout << t << std::endl;
         
         // Iterate through every other row
         for(auto i = 0; i<10; i += 2) {
         // Square each value
         std::for_each(t.begin_row_raw(i), t.end_row_raw(i), [](int& a) { a *= a; });
         }
         
         // Iterate through every third row
         for(auto i = 0; i<10; i += 3) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.begin_row_raw(i), t.end_row_raw(i), [](int& a) { std::cout << a<<","; });
         std::cout << "}\n";
         }
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         {0,1,4,9,16,25,36,49,64,81,}
         {30,31,32,33,34,35,36,37,38,39,}
         {3600,3721,3844,3969,4096,4225,4356,4489,4624,4761,}
         {90,91,92,93,94,95,96,97,98,99,}
         @endcode
         */
        ValueType* begin_row_raw(size_t i) { return entry+i*n; };
        
        /** Returns a raw pointer to one after the last entry of a given row of a table
         @param i is the row number, indexed starting at 0
         @returns a raw pointer to one after the last element of a given row
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         std::iota(std::begin(v), std::end(v), 0);
         
         // You don't have to use all of the values in v
         dsl::table<int> t(10,10,std::begin(v));
         
         std::cout << t << std::endl;
         
         // Iterate through every other row
         for(auto i = 0; i<10; i += 2) {
         // Square each value
         std::for_each(t.begin_row_raw(i), t.end_row_raw(i), [](int& a) { a *= a; });
         }
         
         // Iterate through every third row
         for(auto i = 0; i<10; i += 3) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_row_raw(i), t.cend_row_raw(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n";
         }
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         {0,1,4,9,16,25,36,49,64,81,}
         {30,31,32,33,34,35,36,37,38,39,}
         {3600,3721,3844,3969,4096,4225,4356,4489,4624,4761,}
         {90,91,92,93,94,95,96,97,98,99,}
         @endcode
         */
        ValueType* end_row_raw(size_t i) { return entry+(i+1)*n; };
        
        
        /** returns a const pointer to the first element
         @returns a const pointer to the first element
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v {1,2,3,4,5,6,7,8,9,10,11,12};
         
         // You don't have to use all of the values in v
         dsl::table<int> t(3,3,std::begin(v));
         
         std::cout << t << std::endl;
         
         // We now have complete, unfettered access to the elements in t.
         auto p = t.begin_raw();
         auto p2 = t.end_raw();
         
         // This is useful if we wish to use a routine from another library, e.g., a matrix library, that requires two pointers referring to a contiguous array of values.
         
         // This routine rearranges values into the transpose, the last parameter is the size of each row
         dsl::transpose(p,p2,3);
         
         // print out the transposed table
         std::cout << t << std::endl;
         
         // const iterators, so can access values but cannot alter them.
         auto p3 = t.cbegin_raw();
         auto p4 = t.cend_raw();
         
         std::for_each(p3,p4,[](int a) { std::cout << a << ","; }); std::cout<<std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{1,2,3},
         {4,5,6},
         {7,8,9}}
         {{1,4,7},
         {2,5,8},
         {3,6,9}}
         1,4,7,2,5,8,3,6,9,
         @endcode
        */
        const ValueType* cbegin_raw() { return entry; };
        
        /**
         returns a const pointer to one after the last element
         @returns a const pointer to one after the last element
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v {1,2,3,4,5,6,7,8,9,10,11,12};
         
         // You don't have to use all of the values in v
         dsl::table<int> t(3,3,std::begin(v));
         
         std::cout << t << std::endl;
         
         // We now have complete, unfettered access to the elements in t.
         auto p = t.begin_raw();
         auto p2 = t.end_raw();
         
         // This is useful if we wish to use a routine from another library, e.g., a matrix library, that requires two pointers referring to a contiguous array of values.
         
         // This routine rearranges values into the transpose, the last parameter is the size of each row
         dsl::transpose(p,p2,3);
         
         // print out the transposed table
         std::cout << t << std::endl;
         
         // const iterators, so can access values but cannot alter them.
         auto p3 = t.cbegin_raw();
         auto p4 = t.cend_raw();
         
         std::for_each(p3,p4,[](int a) { std::cout << a << ","; }); std::cout<<std::endl;
         
         return 0;
         }
        @endcode
         Should produce output
         @code
         {{1,2,3},
         {4,5,6},
         {7,8,9}}
         {{1,4,7},
         {2,5,8},
         {3,6,9}}
         1,4,7,2,5,8,3,6,9,
         @endcode
        */
        const ValueType* cend_raw() { return entry+m*n; };
        
        
        /**
         Returns a const raw pointer to a given row of a table
         @param i is the row number, indexed starting at 0
         @returns a const raw pointer to the first element of a given row
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         std::iota(std::begin(v), std::end(v), 0);
         
         // You don't have to use all of the values in v
         dsl::table<int> t(10,10,std::begin(v));
         
         std::cout << t << std::endl;
         
         // Iterate through every other row
         for(auto i = 0; i<10; i += 2) {
         // Square each value
         std::for_each(t.begin_row_raw(i), t.end_row_raw(i), [](int& a) { a *= a; });
         }
         
         // Iterate through every third row
         for(auto i = 0; i<10; i += 3) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_row_raw(i), t.cend_row_raw(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n";
         }
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         {0,1,4,9,16,25,36,49,64,81,}
         {30,31,32,33,34,35,36,37,38,39,}
         {3600,3721,3844,3969,4096,4225,4356,4489,4624,4761,}
         {90,91,92,93,94,95,96,97,98,99,}
         @endcode
        */
        const ValueType* cbegin_row_raw(size_t i) { return entry+i*n; };
        
        /**
         Returns a const raw pointer to one after the last element of a given row of a table
         @param i is the row number, indexed starting at 0
         @returns a const raw pointer to one after the last element of a given row
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         std::iota(std::begin(v), std::end(v), 0);
         
         // You don't have to use all of the values in v
         dsl::table<int> t(10,10,std::begin(v));
         
         std::cout << t << std::endl;
         
         // Iterate through every other row
         for(auto i = 0; i<10; i += 2) {
         // Square each value
         std::for_each(t.begin_row_raw(i), t.end_row_raw(i), [](int& a) { a *= a; });
         }
         
         // Iterate through every third row
         for(auto i = 0; i<10; i += 3) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_row_raw(i), t.cend_row_raw(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n";
         }
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         {0,1,4,9,16,25,36,49,64,81,}
         {30,31,32,33,34,35,36,37,38,39,}
         {3600,3721,3844,3969,4096,4225,4356,4489,4624,4761,}
         {90,91,92,93,94,95,96,97,98,99,}
         @endcode
        */
        const ValueType* cend_row_raw(size_t i) { return entry+(i+1)*n; };
        
        
        /** @class const_iterator
            @brief random access const iterator for all entries in table
         
         This iterator treats the entries in the table as a 1D array of values.  It is useful if the same operation or transformation needs to be applied to all entries in the table.
         
         In principle it is faster to use the raw pointers to iterate through all entries, but this provides a unified object interface, especially when working with column iterators, which must be objects since the operations do not translate over literally.
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create 10x10 table, default initialized values (i.e., 0 for int)
         dsl::table<int> t(10,10);
         
         // Very simple linear congruential engine
         int A = 16807;
         int C = 127;
         int value = 1;
         
         // initialize values using custom linear congruential engine
         for(auto& i : t)
         i = (value = ( (A*value)%C));
         
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // Sort the entire set of values
         std::sort(t.begin(), t.end());
         
         std::cout << "Sort all elements, and print t again:\n";
         std::cout << t << std::endl << std::endl;
         
         // Check for duplicates
         auto start = t.cbegin();
         auto stop = t.cend();
         
         // check is initialized one before start, which now points to second element
         auto checker = start++;
         
         unsigned int number_of_duplicates = 0;
         
         // loop through all elements, essentially treating table as 1D array
         while(start != stop) {
         
         // check for equailty, increment
         if(*checker++ == *start++)
         ++number_of_duplicates;
         }
         
         std::cout << "The number of duplicate elements is: " << number_of_duplicates << std::endl << std::endl;
         
         // I get 0 duplicates, which is not very random!
         // This is an example of the birthday problem.  At each new element created, there is a chance it will match an existing element.  Assuming perfect randomness, the probability that there are no duplicates in 100 values of 127 possible values is given by
         // (1)(1-1/127)(1-2/127)...(1-99/127)
         // Let's do that calculation quickly.
         double x = 1.;
         
         for(size_t i=1;i<100;++i)
         x *= 1.-i/127.;
         
         std::cout << "The probability of having 0 elements if they were in fact generated perfectly randomly is: " << x << ".\n\n";
         
         std::cout << "Do you need further anecdotal evidence to suggest that linear congruential generators are not great randomizers?" << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{43,71,5,88,101,25,59,124,125,41},
         {112,117,78,52,77,9,6,4,45,30},
         {20,98,23,100,109,115,119,37,67,87},
         {58,81,54,36,24,16,53,120,80,11},
         {92,19,55,79,95,21,14,94,105,70},
         {89,17,96,64,85,99,66,44,114,76},
         {93,62,126,84,56,122,39,26,102,68},
         {3,2,86,15,10,49,75,50,118,121},
         {123,82,97,107,29,104,27,18,12,8},
         {90,60,40,69,46,73,91,103,111,74}}
         
         Sort all elements, and print t again:
         {{2,3,4,5,6,8,9,10,11,12},
         {14,15,16,17,18,19,20,21,23,24},
         {25,26,27,29,30,36,37,39,40,41},
         {43,44,45,46,49,50,52,53,54,55},
         {56,58,59,60,62,64,66,67,68,69},
         {70,71,73,74,75,76,77,78,79,80},
         {81,82,84,85,86,87,88,89,90,91},
         {92,93,94,95,96,97,98,99,100,101},
         {102,103,104,105,107,109,111,112,114,115},
         {117,118,119,120,121,122,123,124,125,126}}
         
         The number of duplicate elements is: 0
         
         The probability of having 0 elements if they were in fact generated perfectly randomly is: 1.15238e-25.
         
         Do you need further anecdotal evidence to suggest that linear congruential generators are not great randomizers?
         @endcode
         */
        class const_iterator : public std::iterator<std::random_access_iterator_tag, ValueType>{
        public:
            
            /** Constructor with parameters with default-initialized parameters so also includes the default constructor.  Initializes iterator with a table and an initial location on the table
             @param initial_mat is the table to iterate through
             @param initial_row is the row number.
             @param initial_col is the column number.
            */
            const_iterator(const table* initial_mat=nullptr, int initial_row=0, int initial_col=0) : mat(initial_mat), row(initial_row), column(initial_col) { };
            
            /** copy constructor
                @param other is the existing interator to copy from
             */
            const_iterator(const const_iterator& other) : mat(other.mat), row(other.row), column(other.column) { }
            
            /** swaps two iterators
             @param other is the object to swap with
            */
            void swap(const_iterator& other) {
                std::swap(this->mat, other.mat);
                std::swap(this->row, other.row);
                std::swap(this->column, other.column);
            }
            
            /** Assignment operator, grabs values and copies over
             @param to_copy is the object from which to copy over the values.
            */
            const_iterator& operator=(const_iterator to_copy) {
                swap(to_copy);
                return *this;
            }
            
            /** Standard prefix ++ operator
             @returns reference to self after the increment. */
            const_iterator& operator++() {
                ++column;                   // Increment column
                row    += column/mat->n;   // Increment row if column goes over
                column %= mat->n;          // Reset column if goes over.
                return *this;
            }
            
            /** Postfix ++ operator
             @param unused is an unused parameter to distinguish from the prefix operator++.
            */
            const_iterator operator++(int unused) {
                const_iterator clone(*this);
                ++(*this);
                return clone;
            }
            
            /** increment operator by integer value
             @param col is the number of entries to increment
             @returns a reference to the iterator for chaining.
            */
            const_iterator& operator+=(int col) {
                column += col;
                row    += column/mat->n;
                column %= mat->n;
                return *this;
            };
            
            /** increment operator by integer value
                @param col is the number of entries to increment
                @returns a copy of the iterator pointing to the newly incremented value
            */
            const_iterator operator+(int col) const {
                const_iterator clone(*this);
                clone += col;
                return clone;
            };
            
            /** Standard prefix -- operator
             @returns reference to self after the decrement. */
            const_iterator& operator--() {
                --column;                   // Increment column
                if(column < 0) {
                    column += mat->n;
                    --row;
                }
                return *this;
            }
            
            /** Postfix -- operator
                @param unused is an unused parameter to distinguish from the prefix operator--
                @returns a copy of the iterator to the original position before the decrement
             */
            const_iterator operator--(int unused) {
                const_iterator clone(*this);
                --(*this);
                return clone;
            }
            
            /** decrement operator by integer value
             @param col is the number of entries to decrement
             @returns a reference to the iterator for chaining.
             */
            const_iterator& operator-=(int col) {
                column -= col;
                while(column < 0) {
                    column += mat->n;
                    --row;
                }
                return *this;
            };
            
            /** decrement operator by integer value
             @param col is the number of entries to decrement
             @returns a copy of the iterator pointing to the newly decremented value
             */
            const_iterator operator-(int col) const {
                const_iterator clone(*this);
                clone -= col;
                return clone;
            };

            /** Take the difference between two iterators of the same type, *this-p2
             @param p2 is the rhs of *this-p2
             @returns the number of elements that must be transversed in order to get from *this to p2; can be negative.
            */
            // double check the return type when you get a chance.
            typename const_iterator::iterator::difference_type operator-(const const_iterator& p2) const {
                
                // Convert all values to int since value can be negative
                auto r1 = static_cast<int>(row);
                auto r2 = static_cast<int>(p2.row);
                auto c1 = static_cast<int>(column);
                auto c2 = static_cast<int>(p2.column);
                auto nn = static_cast<int>(mat->n);

                // calculate number of entries in between the contiguous array.
                return (r1-r2)*nn+(c1-c2);
            };
            
            /** Accesses the entry directly relative to mat, so even if mat assumes new memory or location it still points to relatively the same location
             @returns reference to (row,column)-th entry in mat
             */
            ValueType& operator*() const { return mat->entry[row*mat->n+column]; }
            
            /** Accesses the entry directly relative to mat, so even if mat assumes new memory or location it still points to relatively the same location
             @returns reference to (row,column)-th entry in mat
             */
            ValueType& operator->() const { return *(*this); }
            
            /** Accesses the entry n past the current point
             @returns reference to (row,column+n)-th entry in mat
             */
            ValueType& operator[](int n) const { return *(*this+n); }
            
            /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator==(const table::const_iterator& lhs, const table::const_iterator& rhs) {
                return (lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column == rhs.column);
            };
            
            /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator<(const table::const_iterator& lhs, const table::const_iterator& rhs) {
                return (lhs.mat == rhs.mat && lhs.row*lhs.mat->n+lhs.column < rhs.row*lhs.mat->n+rhs.column);
            };
            
            
        private:
            const table* const mat;
            int row;            // int instead of size_t since we are ok with negative values
            int column;         // ditto, negative values useful for indexing reverse iterator locations.
        };
        
        
        table::const_iterator cbegin() const { return table::const_iterator(this); };
        table::const_iterator cend()   const { return table::const_iterator(this, (int)m, 0); };
        
        /** Tests for iterators in two non-equivalent positions @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the iterators are not pointing to the exact same coordinates in the exact same table. */
        friend bool operator!=(const table::const_iterator& lhs, const table::const_iterator& rhs) { return !(lhs == rhs); };
        
        /** Tests for iterators one weakly less than the other @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the iterators are pointing to the same table AND the lhs has a smaller row or if equal row then smaller or equal column.  */
        friend bool operator<=(const table::const_iterator& lhs, const table::const_iterator& rhs) { return (lhs < rhs || lhs == rhs); };
        
        /** Tests for iterators one strictly greater than the other @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the const_iterators are pointing to the same table AND the lhs has a smaller row or if equal row then smaller or equal column.  */
        friend bool operator>(const table::const_iterator& lhs, const table::const_iterator& rhs) { return !(lhs <= rhs); };
        
        /** Tests for const_iterators one weakly greater than the other @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the const_iterators are pointing to the same table AND the lhs has a smaller row or if equal row then smaller or equal column.  */
        friend bool operator>=(const table::const_iterator& lhs, const table::const_iterator& rhs) { return !(lhs < rhs); };
        
        
        
        /** @class row_const_iterator
         @brief Random Access const_iterator for Rows, muentry
         
         This class is designed to be a RANDOM ACCESS const_iterator for a given row of the entry.
         
         The row is modifiable so that it can change which row it is pointing to.
         The column is modified along with the pointer so that it is easier to keep track of bounds.
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         // initialize values to 0,1,2,...,99
         std::iota(std::begin(v), std::end(v), 0);
         
         // Initialize 10x10 table using 10 numbers for each row from v
         dsl::table<int> t(10,10,std::begin(v));
         
         // print out the table of values first
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // Iterate through every fourth column, squaring each element
         for(auto j = 0; j<10; j += 4) {
         // Square each value
         std::for_each(t.begin_column(j), t.end_column(j), [](int& a) { a *= a; });
         }
         
         std::cout << "Printing every other column ... \n";
         // Iterate through every other row, print it out
         for(auto i = 0; i<10; i += 2) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_row(i), t.cend_row(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n\n";
         }
         
         // Sort each column...no good reason, just want to demonstrate that the iterators work even for algorithms which require random access iterators.  Note that begin_column and end_column return objects, not raw pointers, since raw pointers will not observe the desired behavior for the ROW-MAJOR table format.
         for(auto i = 0;i<10;++i)
         std::sort(t.begin_column(i), t.end_column(i));
         
         std::cout << "After all that, sort elements in each column, and print t again:\n";
         std::cout << t << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         
         Printing every other column ...
         {0,1,2,3,16,5,6,7,64,9,}
         
         {400,21,22,23,576,25,26,27,784,29,}
         
         {1600,41,42,43,1936,45,46,47,2304,49,}
         
         {3600,61,62,63,4096,65,66,67,4624,69,}
         
         {6400,81,82,83,7056,85,86,87,7744,89,}
         
         After all that, sort elements in each column, and print t again:
         {{0,1,2,3,16,5,6,7,64,9},
         {100,11,12,13,196,15,16,17,324,19},
         {400,21,22,23,576,25,26,27,784,29},
         {900,31,32,33,1156,35,36,37,1444,39},
         {1600,41,42,43,1936,45,46,47,2304,49},
         {2500,51,52,53,2916,55,56,57,3364,59},
         {3600,61,62,63,4096,65,66,67,4624,69},
         {4900,71,72,73,5476,75,76,77,6084,79},
         {6400,81,82,83,7056,85,86,87,7744,89},
         {8100,91,92,93,8836,95,96,97,9604,99}}
         @endcode
         */
        class row_const_iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
        public:
            /** Construct by table object @param T is the table object with data @param r is the row number. @param col is the column */
            row_const_iterator(const table* initial_mat=nullptr, int initial_row=0, int initial_col=0) : mat(initial_mat), row(initial_row), column(initial_col) { };
            
            row_const_iterator(const row_const_iterator& other) : mat(other.mat), row(other.row), column(other.column) {
            }
            
            void swap(row_const_iterator& other) {
                std::swap(this->mat, other.mat);
                std::swap(this->row, other.row);
                std::swap(this->column, other.column);
            }
            
            row_const_iterator& operator=(row_const_iterator to_copy) {
                swap(to_copy);
                return *this;
            }
            
            /** Standard prefix ++ operator
             @returns reference to self after the increment. */
            row_const_iterator& operator++() {
                ++column;                   // Increment column, do not change row
                return *this;
            }
            
            /** Postfix ++ operator, increments iterator to the next element
             @param unused is an unused parameter
             @returns an iterator referring to the element before the increment
             */
            row_const_iterator operator++(int) {
                row_const_iterator clone(*this);
                ++(*this);
                return clone;
            }
            
            /** Increment equals operator
                @param col is the number of elements to increment over, can be negative
                @returns reference to the iterator for chaining
            */
            row_const_iterator& operator+=(int col) {
                column += col;                  // Again, do NOT change row value
                return *this;
            };
            
            /** Increment operator
                @param col is the number of elements to increment over, can be negative
                @returns a new iterator referring to the incremented value
            */
            row_const_iterator operator+(int col) const {
                row_const_iterator clone(*this);
                clone += col;
                return clone;
            };
            
            /** Standard prefix -- operator
             @returns reference to self after the increment. */
            row_const_iterator& operator--() {
                --column;                   // Decrement column, do not adjust row value.
                return *this;
            }
            
            /** Postfix -- operator */
            row_const_iterator operator--(int) {
                row_const_iterator clone(*this);
                --(*this);
                return clone;
            }
            
            /** Decrement equals operator
             @param col is the number of elements to increment over, can be negative
             @returns reference to the iterator for chaining
             */
            row_const_iterator& operator-=(int col) {
                column -= col;                  // As before, only change column value, do NOT adjust row value.
                return *this;
            };
            
            /** Decrement operator
             @param col is the number of elements to decrement over, can be negative
             @returns a new iterator referring to the decremented value
             */
            row_const_iterator operator-(int col) const {
                row_const_iterator clone(*this);
                clone -= col;
                return clone;
            };
            
            /** Take the difference between two iterators of the same type, *this-p2
             @param p2 is the rhs of *this-p2
             @returns the number of elements that must be transversed in order to get from *this to p2; can be negative.
             */
            int operator-(const row_const_iterator& p2) const {

                // convert values to int, since can be negative
                auto c1 = static_cast<int>(column);
                auto c2 = static_cast<int>(p2.column);
                
                // calculate the difference
                return c1-c2;
            };
            
            /** Dereference operator @returns the value the const_iterator points to. */
            ValueType& operator*() const { return mat->entry[row*mat->n+column]; };
            
            /** Dereference operator @returns the value the const_iterator points to.  Equivalent to operator* by dereferencing twice. */
            ValueType& operator->() const { return **this; };
            
            // Comparison Operators
            
            
            /** Tests for const_iterators in two equivalent positions @param t is the other const_iterator @returns true if const_iterators are equivalent */
            friend bool operator==(const table::row_const_iterator& lhs, const table::row_const_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column == rhs.column;
            };
            
            /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
            friend bool operator<(const table::row_const_iterator& lhs, const table::row_const_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column < rhs.column;
            };
            
            /** Random access operator, makes iterator feel more like a pointer
                @param n is the offset, can be negative
                @returns a reference to the element referred to by the iterator and offset
            */
            ValueType& operator[](int n) const { return *(*this+n); };
            
            
        private:
            // Keeps track of which column current in, useful for bounds checking.
            int column;
            int row;
            
            // pointer to the memory in entry.
            const table* mat;
            
        };
        
        /** Tests for const_iterators in two equivalent positions @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator!=(const table::row_const_iterator& lhs, const table::row_const_iterator& rhs) {
            return !(lhs == rhs);
        };
        
        /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator<=(const table::row_const_iterator& lhs, const table::row_const_iterator& rhs) {
            return lhs==rhs || lhs < rhs;
        };
        
        /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator>=(const table::row_const_iterator& lhs, const table::row_const_iterator& rhs) {
            return !(lhs < rhs);
        };
        
        /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator>(const table::row_const_iterator& lhs, const table::row_const_iterator& rhs) {
            return !(lhs <= rhs);
        };
        
        /** const row iterator so can be used in const member functions
            @param row is the row number
            @returns an iterator referring to the first element in the indicated row of the table.
        */
        table::row_const_iterator cbegin_row(int row) const {
            return table::row_const_iterator(this, row, 0);
        };
        
        /** const row iterator so can be used in const member functions
         @param row is the row number
         @returns an iterator referring to one after the last element in the indicated row of the table.
         */
        table::row_const_iterator cend_row(int row) const {
            return table::row_const_iterator(this, row, (int)this->n);
        };
        
        
        
        /** @class column_const_iterator
         @brief Random Access const_iterator for columns
         
         This class is designed to be a RANDOM ACCESS const_iterator for a given column of the entry.
         
         The row is declared const so that it cannot be modified once set.
         The col is modified along with the pointer so that it is easier to keep track of bounds.
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         // initialize values to 0,1,2,...,99
         std::iota(std::begin(v), std::end(v), 0);
         
         // Initialize 10x10 table using 10 numbers for each row from v
         dsl::table<int> t(10,10,std::begin(v));
         
         // print out the table of values first
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // Iterate through every other row, squaring each element in each row
         for(auto i = 0; i<10; i += 2) {
         // Square each value
         std::for_each(t.begin_row(i), t.end_row(i), [](int& a) { a *= a; });
         }
         
         std::cout << "Printing every third column ... \n";
         // Iterate through every third column, print it out
         for(auto i = 0; i<10; i += 3) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_column(i), t.cend_column(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n\n";
         }
         
         // Sort each row...no good reason, just want to demonstrate that the iterators work even for algorithms which require random access iterators.  Note that begin_row and end_row return objects, not raw pointers.  Use begin_row_raw and end_row_raw to obtain the raw pointer types.
         for(auto i = 0;i<10;++i)
         std::sort(t.begin_row(i), t.end_row(i));
         
         std::cout << "After all that, sort elements in each row, and print t again:\n";
         std::cout << t << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         
         Printing every third column ...
         {0,10,400,30,1600,50,3600,70,6400,90,}
         
         {9,13,529,33,1849,53,3969,73,6889,93,}
         
         {36,16,676,36,2116,56,4356,76,7396,96,}
         
         {81,19,841,39,2401,59,4761,79,7921,99,}
         
         After all that, sort elements in each row, and print t again:
         {{0,1,4,9,16,25,36,49,64,81},
         {10,11,12,13,14,15,16,17,18,19},
         {400,441,484,529,576,625,676,729,784,841},
         {30,31,32,33,34,35,36,37,38,39},
         {1600,1681,1764,1849,1936,2025,2116,2209,2304,2401},
         {50,51,52,53,54,55,56,57,58,59},
         {3600,3721,3844,3969,4096,4225,4356,4489,4624,4761},
         {70,71,72,73,74,75,76,77,78,79},
         {6400,6561,6724,6889,7056,7225,7396,7569,7744,7921},
         {90,91,92,93,94,95,96,97,98,99}}
         @endcode
         */
        class column_const_iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
        public:
            /** Construct by table object @param T is the table object with data @param r is the row number. @param col is the column */
            column_const_iterator(const table* initial_mat=nullptr, int initial_row=0, int initial_col=0) : mat(initial_mat), row(initial_row), column(initial_col) { };
            
            /** copy constructor, no heap memory managed so simple copy of all members, not really necessary to write explicitly since compiler would generate this for us.
                @param other
            */
            column_const_iterator(const column_const_iterator& other) : mat(other.mat), row(other.row), column(other.column) { };
            
            /** swaps two iterators
                @param other is the other iterator
            */
            void swap(column_const_iterator& other) {
                std::swap(this->mat, other.mat);
                std::swap(this->row, other.row);
                std::swap(this->column, other.column);
            }
            
            /** Assignment operator, copies over using Copy & Swap idiom
             @param to_copy is the existing object to copy from
             @returns a reference to the newly updated object, for chaining.
             */
            column_const_iterator& operator=(column_const_iterator to_copy) {
                swap(to_copy);
                return *this;
            }
            
            
            /** Standard prefix ++ operator
             @returns reference to self after the increment. */
            column_const_iterator& operator++() {
                ++row;                   // Increment column, do not change row
                return *this;
            }
            
            /** Postfix ++ operator */
            column_const_iterator operator++(int) {
                column_const_iterator clone(*this);
                ++(*this);
                return clone;
            }
            
            /** Increment equals operator, updated value of iterator
                @param r is the increment, can be negative
                @returns a reference to the newly increment iterator for chaining.
            */
            column_const_iterator& operator+=(int r) {
                row += r;                  // Again, do NOT change row value
                return *this;
            };
            
            /** Increment operator.
                @param r is the increment, can be negative
                @returns a new iterator referring to the original element plus the input offset.
            */
            column_const_iterator operator+(int r) const {
                column_const_iterator clone(*this);
                clone += r;
                return clone;
            };
            
            /** Standard prefix -- operator
             @returns reference to self after the increment. */
            column_const_iterator& operator--() {
                --row;                   // Decrement column, do not adjust row value.
                return *this;
            }
            
            /** Postfix -- operator */
            column_const_iterator operator--(int) {
                column_const_iterator clone(*this);
                --(*this);
                return clone;
            }
            
            /** Decrement equals operator, updated value of iterator
             @param r is the decrement, can be negative
             @returns a reference to the newly decremented iterator for chaining.
             */
            column_const_iterator& operator-=(int r) {
                row -= r;                  // As before, only change column value, do NOT adjust row value.
                return *this;
            };
            
            /** Decrement operator.
             @param r is the decrement, can be negative
             @returns a new iterator referring to the original element minus the input offset.
             */
            column_const_iterator operator-(int r) const {
                column_const_iterator clone(*this);
                clone -= r;
                return clone;
            };
            
            /** Take the difference between two iterators of the same type, *this-p2
             @param p2 is the rhs of *this-p2
             @returns the number of elements that must be transversed in order to get from *this to p2; can be negative.
             */
            int operator-(const column_const_iterator& p2) const {
                
                auto r1 = static_cast<int>(row);
                auto r2 = static_cast<int>(p2.row);
                
                return (r1-r2);
            };
            
            /** Dereference operator @returns the value the const_iterator points to. */
            ValueType& operator*() const { return mat->entry[row*mat->n+column]; };
            
            /** Dereference operator @returns the value the const_iterator points to.  Equivalent to operator* by dereferencing twice. */
            ValueType& operator->() const { return **this; };
            
            // Comparison Operators
            
            
            /** Tests for const_iterators in two equivalent positions @param t is the other const_iterator @returns true if const_iterators are equivalent */
            friend bool operator==(const table::column_const_iterator& lhs, const table::column_const_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column == rhs.column;
            };
            
            /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
            friend bool operator<(const table::column_const_iterator& lhs, const table::column_const_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row < rhs.row && lhs.column == rhs.column;
            };
            
            /** Random access operator, in order to mimic pointer.
                @param n is the offset, can be negative
                @returns a reference to the element referred to by the iterator and offset.
            */
            ValueType& operator[](int n) const { return *(*this+n); };
            
            
        private:
            // Keeps track of which column current in, useful for bounds checking.
            int column;
            int row;
            
            // pointer to the memory in entry.
            const table* mat;
            
        };
        
        /** Tests for const_iterators in two equivalent positions @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator!=(const table::column_const_iterator& lhs, const table::column_const_iterator& rhs) {
            return !(lhs == rhs);
        };
        
        /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator<=(const table::column_const_iterator& lhs, const table::column_const_iterator& rhs) {
            return lhs==rhs || lhs < rhs;
        };
        
        /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator>=(const table::column_const_iterator& lhs, const table::column_const_iterator& rhs) {
            return !(lhs < rhs);
        };
        
        /** Tests for const_iterators in same row but strictly smaller column @param t is the other const_iterator @returns true if const_iterators are equivalent */
        friend bool operator>(const table::column_const_iterator& lhs, const table::column_const_iterator& rhs) {
            return !(lhs <= rhs);
        };
        
        /** Returns an iterator referring to the first element of a given column
            @param col is the column number to refer to, can be negative ... why?  Any good reason?
            @returns an iterator referring to the first element of a given column
        */
        table::column_const_iterator cbegin_column(int col) const {
            return table::column_const_iterator(this, 0, col);
        };
        
        /** Returns an iterator referring to one after the last element of a given column
         @param col is the column number to refer to, can be negative ... why?  Any good reason?
         @returns an iterator referring to one after the last element of a given column
         */
        table::column_const_iterator cend_column(int col) const {
            return table::column_const_iterator(this, (int)this->m, col);
        };
        
        
        /** @class iterator
            @brief random access iterator for a table, treating it like a 1D array
         
         Use this if the tabular structure of the table is immaterial, and you would like to operate on entries of the table as if they were one contiguous array.
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create 10x10 table, default initialized values (i.e., 0 for int)
         dsl::table<int> t(10,10);
         
         // Very simple linear congruential engine
         int A = 16807;
         int C = 127;
         int value = 1;
         
         // initialize values using custom linear congruential engine
         for(auto& i : t)
         i = (value = ( (A*value)%C));
         
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // Sort the entire set of values
         std::sort(t.begin(), t.end());
         
         std::cout << "Sort all elements, and print t again:\n";
         std::cout << t << std::endl << std::endl;
         
         // Check for duplicates
         auto start = t.cbegin();
         auto stop = t.cend();
         
         // check is initialized one before start, which now points to second element
         auto checker = start++;
         
         unsigned int number_of_duplicates = 0;
         
         // loop through all elements, essentially treating table as 1D array
         while(start != stop) {
         
         // check for equailty, increment
         if(*checker++ == *start++)
         ++number_of_duplicates;
         }
         
         std::cout << "The number of duplicate elements is: " << number_of_duplicates << std::endl << std::endl;
         
         // I get 0 duplicates, which is not very random!
         // This is an example of the birthday problem.  At each new element created, there is a chance it will match an existing element.  Assuming perfect randomness, the probability that there are no duplicates in 100 values of 127 possible values is given by
         // (1)(1-1/127)(1-2/127)...(1-99/127)
         // Let's do that calculation quickly.
         double x = 1.;
         
         for(size_t i=1;i<100;++i)
         x *= 1.-i/127.;
         
         std::cout << "The probability of having 0 elements if they were in fact generated perfectly randomly is: " << x << ".\n\n";
         
         std::cout << "Do you need further anecdotal evidence to suggest that linear congruential generators are not great randomizers?" << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{43,71,5,88,101,25,59,124,125,41},
         {112,117,78,52,77,9,6,4,45,30},
         {20,98,23,100,109,115,119,37,67,87},
         {58,81,54,36,24,16,53,120,80,11},
         {92,19,55,79,95,21,14,94,105,70},
         {89,17,96,64,85,99,66,44,114,76},
         {93,62,126,84,56,122,39,26,102,68},
         {3,2,86,15,10,49,75,50,118,121},
         {123,82,97,107,29,104,27,18,12,8},
         {90,60,40,69,46,73,91,103,111,74}}
         
         Sort all elements, and print t again:
         {{2,3,4,5,6,8,9,10,11,12},
         {14,15,16,17,18,19,20,21,23,24},
         {25,26,27,29,30,36,37,39,40,41},
         {43,44,45,46,49,50,52,53,54,55},
         {56,58,59,60,62,64,66,67,68,69},
         {70,71,73,74,75,76,77,78,79,80},
         {81,82,84,85,86,87,88,89,90,91},
         {92,93,94,95,96,97,98,99,100,101},
         {102,103,104,105,107,109,111,112,114,115},
         {117,118,119,120,121,122,123,124,125,126}}
         
         The number of duplicate elements is: 0
         
         The probability of having 0 elements if they were in fact generated perfectly randomly is: 1.15238e-25.
         
         Do you need further anecdotal evidence to suggest that linear congruential generators are not great randomizers?
         @endcode
        */
        class iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
        public:
            
            /** Constructors with various default parameters.  \n
                If 2 parameters are provided, then the default column is 0. \n
                If 1 prameter is provided, then the default row and column are 0. \n
                If no parameters are provided, then the default row and column are 0 and the default table pointer is set to nullptr.
                @param initial_mat is a table for which to refer to
                @param initial_row is the initial row entry to refer to
                @param initial_col is the initial column entry to refer to
            */
            iterator(table* initial_mat=nullptr, int initial_row=0, int initial_col=0) : mat(initial_mat), row(initial_row), column(initial_col) { };
            
            /** Default Copy constructor, no memory is being managed so this one can also be generated by the compiler.
                @param other is the other iterator from which to copy values.
            */
            iterator(const iterator& other) : mat(other.mat), row(other.row), column(other.column) { };
            
            /** Swaps out the values of two iterators, even iterators referring to different tables.
                @param other is the table from which to swap.
            */
            void swap(iterator& other) {
                std::swap(this->mat, other.mat);
                std::swap(this->row, other.row);
                std::swap(this->column, other.column);
            }
            
            /** Assignment operator, follows the usual copy and swap idiom
                @param to_copy is an iterator from whcih to copy from.
                @returns a reference to the iterator for chaining.
            */
            iterator& operator=(iterator to_copy) {
                swap(to_copy);
                return *this;
            }
            
            /** Standard prefix ++ operator
             @returns reference to self after the increment. */
            iterator& operator++() {
                ++column;                   // Increment column
                row    += column/mat->n;   // Increment row if column goes over
                column %= mat->n;          // Reset column if goes over.
                return *this;
            }
            
            /** Postfix ++ operator */
            iterator operator++(int) {
                iterator clone(*this);
                ++(*this);
                return clone;
            }
            
            /** Increment equals operator for iterator, so that it works like a pointer.
                @param col is an increment through columns, can be negative
                @returns reference to the iterator for chaining.
            */
            iterator& operator+=(int col) {
                column += col;
                row    += column/mat->n;
                column %= mat->n;
                return *this;
            };
            
            /** Increment operator for iterator, so that it works like a pointer.
             @param col is an increment through columns, can be negative
             @returns a new iterator referring to the element indicated by the increment.
             */
            iterator operator+(int col) {
                iterator clone(*this);
                clone += col;
                return clone;
            };
            
            /** Standard prefix -- operator
             @returns reference to self after the increment. */
            iterator& operator--() {
                --column;                   // Increment column
                if(column < 0) {
                    column += mat->n;
                    --row;
                }
                return *this;
            }
            
            /** Postfix -- operator */
            iterator operator--(int) {
                iterator clone(*this);
                --(*this);
                return clone;
            }
            
            /** Decrement equals operator for iterator, so that it works like a pointer.
             @param col is a decrement through columns, can be negative
             @returns reference to the iterator for chaining.
             */
            iterator& operator-=(int col) {
                column -= col;
                while(column < 0) {
                    column += mat->n;
                    --row;
                }
                return *this;
            };
            
            /** Decrement operator for iterator, so that it works like a pointer.
             @param col is a decrement through columns, can be negative
             @returns a new iterator referring to the element indicated by the decrement.
             */
            iterator operator-(int col) {
                iterator clone(*this);
                clone -= col;
                return clone;
            };
            
            /** Take the difference between two iterators of the same type, *this-p2
             @param p2 is the rhs of *this-p2
             @returns the number of elements that must be transversed in order to get from *this to p2; can be negative.
             */
            int operator-(const iterator& p2) const {
                
                // Convert all values to int
                auto r1 = static_cast<int>(row);
                auto r2 = static_cast<int>(p2.row);
                auto c1 = static_cast<int>(column);
                auto c2 = static_cast<int>(p2.column);
                auto nn = static_cast<int>(mat->n);
                // compute total number of entries in contiguous array separating the two iterators
                return (r1-r2)*nn+(c1-c2);
            };
            
            /** Accesses the entry directly relative to mat, so even if mat assumes new memory or location it still points to relatively the same location
             @returns reference to (row,column)-th entry in mat
             */
            ValueType& operator*() { return mat->entry[row*mat->n+column]; }
            
            /** Accesses the entry directly relative to mat, so even if mat assumes new memory or location it still points to relatively the same location
             @returns reference to (row,column)-th entry in mat
             */
            ValueType& operator->() { return *(*this); }
            
            /** Accesses the entry n past the current point
             @returns reference to (row,column+n)-th entry in mat
             */
            ValueType& operator[](int n) { return *(*this+n); }
            
            /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator==(const table::iterator& lhs, const table::iterator& rhs) {
                return (lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column == rhs.column);
            };
            
            /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator<(const table::iterator& lhs, const table::iterator& rhs) {
                return (lhs.mat == rhs.mat && lhs.row*lhs.mat->n+lhs.column < rhs.row*lhs.mat->n+rhs.column);
            };
            
            
        private:
            const table* mat;
            int row;            // int instead of size_t since we are ok with negative values
            int column;         // ditto, negative values useful for indexing reverse iterator locations.
        };
        
        /** member begin function so can be used with range-based for loops and other generic algorithms, applied to each element in the table as if it was a 1D array.
            @returns an iterator to the first element in the table.
        */
        table::iterator begin() { return table::iterator(this); };

        /** member end function so can be used with range-based for loops and other generic algorithms, applied to each element in the table as if it was a 1D array.
         @returns an iterator to one after the last element in the table.
         */
        table::iterator end()   { return table::iterator(this, (int)m, 0); };
        
        /** Tests for iterators in two non-equivalent positions @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the iterators are not pointing to the exact same coordinates in the exact same table. */
        friend bool operator!=(const table::iterator& lhs, const table::iterator& rhs) { return !(lhs == rhs); };
        
        /** Tests for iterators one weakly less than the other @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the iterators are pointing to the same table AND the lhs has a smaller row or if equal row then smaller or equal column.  */
        friend bool operator<=(const table::iterator& lhs, const table::iterator& rhs) { return (lhs < rhs || lhs == rhs); };
        
        /** Tests for iterators one strictly greater than the other @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the iterators are pointing to the same table AND the lhs has a smaller row or if equal row then smaller or equal column.  */
        friend bool operator>(const table::iterator& lhs, const table::iterator& rhs) { return !(lhs <= rhs); };
        
        /** Tests for iterators one weakly greater than the other @param lhs is the left hand side @param rhs is the right hand side
         @returns true as long as the iterators are pointing to the same table AND the lhs has a smaller row or if equal row then smaller or equal column.  */
        friend bool operator>=(const table::iterator& lhs, const table::iterator& rhs) { return !(lhs < rhs); };
        
        
        
        /** @class row_iterator
         @brief Random Access Iterator for Rows, muentry
         
         This class is designed to be a RANDOM ACCESS ITERATOR for a given row of the entry.
         
         The row is modifiable so that it can change which row it is pointing to.
         The column is modified along with the pointer so that it is easier to keep track of bounds.
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         // initialize values to 0,1,2,...,99
         std::iota(std::begin(v), std::end(v), 0);
         
         // Initialize 10x10 table using 10 numbers for each row from v
         dsl::table<int> t(10,10,std::begin(v));
         
         // print out the table of values first
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // Iterate through every other row, squaring each element in each row
         for(auto i = 0; i<10; i += 2) {
         // Square each value
         std::for_each(t.begin_row(i), t.end_row(i), [](int& a) { a *= a; });
         }
         
         std::cout << "Printing every third column ... \n";
         // Iterate through every third column, print it out
         for(auto i = 0; i<10; i += 3) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_column(i), t.cend_column(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n\n";
         }
         
         // Sort each row...no good reason, just want to demonstrate that the iterators work even for algorithms which require random access iterators.  Note that begin_row and end_row return objects, not raw pointers.  Use begin_row_raw and end_row_raw to obtain the raw pointer types.
         for(auto i = 0;i<10;++i)
         std::sort(t.begin_row(i), t.end_row(i));
         
         std::cout << "After all that, sort elements in each row, and print t again:\n";
         std::cout << t << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         
         Printing every third column ...
         {0,10,400,30,1600,50,3600,70,6400,90,}
         
         {9,13,529,33,1849,53,3969,73,6889,93,}
         
         {36,16,676,36,2116,56,4356,76,7396,96,}
         
         {81,19,841,39,2401,59,4761,79,7921,99,}
         
         After all that, sort elements in each row, and print t again:
         {{0,1,4,9,16,25,36,49,64,81},
         {10,11,12,13,14,15,16,17,18,19},
         {400,441,484,529,576,625,676,729,784,841},
         {30,31,32,33,34,35,36,37,38,39},
         {1600,1681,1764,1849,1936,2025,2116,2209,2304,2401},
         {50,51,52,53,54,55,56,57,58,59},
         {3600,3721,3844,3969,4096,4225,4356,4489,4624,4761},
         {70,71,72,73,74,75,76,77,78,79},
         {6400,6561,6724,6889,7056,7225,7396,7569,7744,7921},
         {90,91,92,93,94,95,96,97,98,99}}
         @endcode
         */
        class row_iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
        public:
            /** Construct by table object @param T is the table object with data @param r is the row number. @param col is the column */
            row_iterator(table* initial_mat, int initial_row=0, int initial_col=0) : mat(initial_mat), row(initial_row), column(initial_col) { };
            
            /** Copy constructor, same as default
                @param other is the iterator from which to copy from
             */
            row_iterator(const row_iterator& other) : mat(other.mat), row(other.row), column(other.column) {
                //std::cout << "Making a copy" << std::endl;
            };
            
            /** Standard swap between two row_iterator s, even those referring to different tables
                @param other is another row_iterator
             */
            void swap(row_iterator& other) {
                std::swap(this->mat, other.mat);
                std::swap(this->row, other.row);
                std::swap(this->column, other.column);
            }
            
            /** Assignment operator, follows the Copy & Swap idiom
                @param to_copy is the iterator from which to copy from
                @returns a reference to the iterator, for chaining.
            */
            row_iterator& operator=(row_iterator to_copy) {
                swap(to_copy);
                return *this;
            }
            
            /** Standard prefix ++ operator
             @returns reference to self after the increment. */
            row_iterator& operator++() {
                ++column;                   // Increment column, do not change row
                return *this;
            }
            
            /** Postfix ++ operator */
            row_iterator operator++(int) {
                row_iterator clone(*this);
                ++(*this);
                return clone;
            }
            
            
            /** Increment equals operator
             @param col is the number of elements to increment over, can be negative
             @returns reference to the iterator for chaining
             */
            row_iterator& operator+=(int col) {
                column += col;                  // Again, do NOT change row value
                return *this;
            };
            
            /** Increment operator
             @param col is the number of elements to increment over, can be negative
             @returns a new iterator referring to the incremented value
             */
            row_iterator operator+(int col) {
                row_iterator clone(*this);
                clone += col;
                return clone;
            };
            
            /** Standard prefix -- operator
             @returns reference to self after the increment. */
            row_iterator& operator--() {
                --column;                   // Decrement column, do not adjust row value.
                return *this;
            }
            
            /** Postfix -- operator */
            row_iterator operator--(int) {
                row_iterator clone(*this);
                --(*this);
                return clone;
            }
            
            
            /** Decrement equals operator
             @param col is the number of elements to increment over, can be negative
             @returns reference to the iterator for chaining
             */
            row_iterator& operator-=(int col) {
                column -= col;                  // As before, only change column value, do NOT adjust row value.
                return *this;
            };
            
            /** Decrement operator
             @param col is the number of elements to decrement over, can be negative
             @returns a new iterator referring to the decremented value
             */
            row_iterator operator-(int col) {
                row_iterator clone(*this);
                clone -= col;
                return clone;
            };
            
            /** Take the difference between two iterators of the same type, *this-p2
             @param p2 is the rhs of *this-p2
             @returns the number of elements that must be transversed in order to get from *this to p2; can be negative.
             */
            int operator-(const row_iterator& p2) const {
                
                // convert column numbers to int
                auto c1 = static_cast<int>(column);
                auto c2 = static_cast<int>(p2.column);
                
                // take the difference
                return c1-c2;
            };
            
            /** Dereference operator @returns the value the iterator points to. */
            ValueType& operator*() const { return mat->entry[row*mat->n+column]; };
            
            /** Dereference operator @returns the value the iterator points to.  Equivalent to operator* by dereferencing twice. */
            ValueType& operator->() const { return **this; };
            
            // Comparison Operators
            
            
            /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator==(const table::row_iterator& lhs, const table::row_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column == rhs.column;
            };
            
            /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator<(const table::row_iterator& lhs, const table::row_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column < rhs.column;
            };
            
            /** Random access operator, makes the iterator act like a pointer.
                @param n is the offset, can be negative.
                @returns reference to the element referred to by the iterator and offset.
            */
            ValueType& operator[](int n) { return *(*this+n); };
            
            
        private:
            // Keeps track of which column current in, useful for bounds checking.
            int column;
            int row;
            
            // pointer to the memory in entry.
            table* mat;
            
        };
        
        /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator!=(const table::row_iterator& lhs, const table::row_iterator& rhs) {
            return !(lhs == rhs);
        };
        
        /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator<=(const table::row_iterator& lhs, const table::row_iterator& rhs) {
            return lhs==rhs || lhs < rhs;
        };
        
        /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator>=(const table::row_iterator& lhs, const table::row_iterator& rhs) {
            return !(lhs < rhs);
        };
        
        /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator>(const table::row_iterator& lhs, const table::row_iterator& rhs) {
            return !(lhs <= rhs);
        };
        
        /** Returns an iterator to the first element of a given row
         @param row is the given row
         @returns an iterator to the first element of a given row
        */
        table::row_iterator begin_row(int row) {
            return table::row_iterator(this, row, 0);
        };
        
        /** Returns an iterator to one after the last element of a given row
         @param row is the given row
         @returns an iterator to one after the last element of a given row
         */
        table::row_iterator end_row(int row) {
            return table::row_iterator(this, row, (int)this->n);
        };
        
        
        
        /** @class column_iterator
         @brief Random Access Iterator for columns
         
         This class is designed to be a RANDOM ACCESS ITERATOR for a given column of the entry.
         
         The row is declared const so that it cannot be modified once set.
         The col is modified along with the pointer so that it is easier to keep track of bounds.
         @code
         
         #include <iostream>
         #include <vector>
         #include "std_cout.h"
         #include "desalvo/table.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Make a vector of values
         std::vector<int> v(100);
         
         // initialize values to 0,1,2,...,99
         std::iota(std::begin(v), std::end(v), 0);
         
         // Initialize 10x10 table using 10 numbers for each row from v
         dsl::table<int> t(10,10,std::begin(v));
         
         // print out the table of values first
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // Iterate through every fourth column, squaring each element
         for(auto j = 0; j<10; j += 4) {
         // Square each value
         std::for_each(t.begin_column(j), t.end_column(j), [](int& a) { a *= a; });
         }
         
         std::cout << "Printing every other column ... \n";
         // Iterate through every other row, print it out
         for(auto i = 0; i<10; i += 2) {
         // Print every other row
         std::cout << "{";
         std::for_each(t.cbegin_row(i), t.cend_row(i), [](int a) { std::cout << a<<","; });
         std::cout << "}\n\n";
         }
         
         // Sort each column...no good reason, just want to demonstrate that the iterators work even for algorithms which require random access iterators.  Note that begin_column and end_column return objects, not raw pointers, since raw pointers will not observe the desired behavior for the ROW-MAJOR table format.
         for(auto i = 0;i<10;++i)
         std::sort(t.begin_column(i), t.end_column(i));
         
         std::cout << "After all that, sort elements in each column, and print t again:\n";
         std::cout << t << std::endl;
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{0,1,2,3,4,5,6,7,8,9},
         {10,11,12,13,14,15,16,17,18,19},
         {20,21,22,23,24,25,26,27,28,29},
         {30,31,32,33,34,35,36,37,38,39},
         {40,41,42,43,44,45,46,47,48,49},
         {50,51,52,53,54,55,56,57,58,59},
         {60,61,62,63,64,65,66,67,68,69},
         {70,71,72,73,74,75,76,77,78,79},
         {80,81,82,83,84,85,86,87,88,89},
         {90,91,92,93,94,95,96,97,98,99}}
         
         Printing every other column ...
         {0,1,2,3,16,5,6,7,64,9,}
         
         {400,21,22,23,576,25,26,27,784,29,}
         
         {1600,41,42,43,1936,45,46,47,2304,49,}
         
         {3600,61,62,63,4096,65,66,67,4624,69,}
         
         {6400,81,82,83,7056,85,86,87,7744,89,}
         
         After all that, sort elements in each column, and print t again:
         {{0,1,2,3,16,5,6,7,64,9},
         {100,11,12,13,196,15,16,17,324,19},
         {400,21,22,23,576,25,26,27,784,29},
         {900,31,32,33,1156,35,36,37,1444,39},
         {1600,41,42,43,1936,45,46,47,2304,49},
         {2500,51,52,53,2916,55,56,57,3364,59},
         {3600,61,62,63,4096,65,66,67,4624,69},
         {4900,71,72,73,5476,75,76,77,6084,79},
         {6400,81,82,83,7056,85,86,87,7744,89},
         {8100,91,92,93,8836,95,96,97,9604,99}}
         @endcode
         */
        class column_iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {
        public:
            /** Construct by table object @param T is the table object with data @param r is the row number. @param col is the column */
            column_iterator(table* initial_mat, int initial_row=0, int initial_col=0) : mat(initial_mat), row(initial_row), column(initial_col) { };
            
            /** Copy constructor, follows default
             @param other is the existing object from which to copy from
             */
            column_iterator(const column_iterator& other) : mat(other.mat), row(other.row), column(other.column) { };
            
            /** Copy constructor, follows default
             @param other is the existing object from which to copy from
             */
            column_iterator(column_iterator&& other) : mat(std::move(other.mat)), row(std::move(other.row)), column(std::move(other.column)) { };
            
            /** Swaps two iterators, even those referring to different tables
                @param other is the other iterator.
            */
            void swap(column_iterator& other) {
                std::swap(this->mat, other.mat);
                std::swap(this->row, other.row);
                std::swap(this->column, other.column);
            }
            
            /** Assignment operator, copies over using Copy & Swap idiom
                @param to_copy is the existing object to copy from
                @returns a reference to the newly updated object, for chaining.
            */
            column_iterator& operator=(column_iterator to_copy) {
                swap(to_copy);
                return *this;
            }
            
            /** Standard prefix ++ operator
             @returns reference to self after the increment. */
            column_iterator& operator++() {
                ++row;                   // Increment column, do not change row
                return *this;
            }
            
            /** Postfix ++ operator */
            column_iterator operator++(int) {
                column_iterator clone(*this);
                ++(*this);
                return clone;
            }
            
            column_iterator& operator+=(int r) {
                row += r;                  // Again, do NOT change row value
                return *this;
            };
            
            column_iterator operator+(int r) {
                column_iterator clone(*this);
                clone += r;
                return clone;
            };
            
            /** Standard prefix -- operator
             @returns reference to self after the increment. */
            column_iterator& operator--() {
                --row;                   // Decrement column, do not adjust row value.
                return *this;
            }
            
            /** Postfix -- operator */
            column_iterator operator--(int) {
                column_iterator clone(*this);
                --(*this);
                return clone;
            }
            
            column_iterator& operator-=(int r) {
                row -= r;                  // As before, only change column value, do NOT adjust row value.
                return *this;
            };
            
            column_iterator operator-(int r) {
                column_iterator clone(*this);
                clone -= r;
                return clone;
            };
            
            /** Take the difference between two iterators of the same type, *this-p2
             @param p2 is the rhs of *this-p2
             @returns the number of elements that must be transversed in order to get from *this to p2; can be negative.
             */
            int operator-(const column_iterator& p2) const {
                
                // convert row numbers to int
                auto r1 = static_cast<int>(row);
                auto r2 = static_cast<int>(p2.row);
                
                // return the difference
                return (r1-r2);
            };
            
            /** Dereference operator @returns the value the iterator points to. */
            const ValueType& operator*() const { return mat->entry[row*mat->n+column]; };
            
            /** Dereference operator @returns the value the iterator points to. */
            ValueType& operator*() { return mat->entry[row*mat->n+column]; };
            
            /** Dereference operator @returns the value the iterator points to.  Equivalent to operator* by dereferencing twice. */
            const ValueType& operator->() const { return **this; };
            
            /** Dereference operator @returns the value the iterator points to.  Equivalent to operator* by dereferencing twice. */
            ValueType& operator->() { return **this; };
            
            // Comparison Operators
            
            
            /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator==(const table::column_iterator& lhs, const table::column_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row == rhs.row && lhs.column == rhs.column;
            };
            
            /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
            friend bool operator<(const table::column_iterator& lhs, const table::column_iterator& rhs) {
                return lhs.mat == rhs.mat && lhs.row < rhs.row && lhs.column == rhs.column;
            };
            
            
            ValueType& operator[](int n) { return *(*this+n); };
            const ValueType& operator[](int n) const { return *(*this+n); };
            
            
        private:
            // Keeps track of which column current in, useful for bounds checking.
            int column;
            int row;
            
            // pointer to the memory in entry.
            table* mat;
            
        };
        
        /** Tests for iterators in two equivalent positions @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator!=(const table::column_iterator& lhs, const table::column_iterator& rhs) {
            return !(lhs == rhs);
        };
        
        /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator<=(const table::column_iterator& lhs, const table::column_iterator& rhs) {
            return lhs==rhs || lhs < rhs;
        };
        
        /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator>=(const table::column_iterator& lhs, const table::column_iterator& rhs) {
            return !(lhs < rhs);
        };
        
        /** Tests for iterators in same row but strictly smaller column @param t is the other iterator @returns true if iterators are equivalent */
        friend bool operator>(const table::column_iterator& lhs, const table::column_iterator& rhs) {
            return !(lhs <= rhs);
        };
        
        table::column_iterator begin_column(int col) {
            return table::column_iterator(this, 0, col);
        };
        
        table::column_iterator end_column(int col) {
            return table::column_iterator(this, (int)this->m, col);
        };
        
        
        
        
        
        /** @class table_row_reference
         @brief reference to a row of a table, useful for range-based for loops
         
         This is to be used to access rows of a table.
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         #include "std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create 10x10 table, default initialized values (i.e., 0 for int)
         dsl::table<int> t(10,10);
         
         // Very simple linear congruential engine
         int A = 16807;
         int C = 127;
         int value = 1;
         
         // initialize values using custom linear congruential engine
         for(auto& i : t)
         i = (value = ( (A*value)%C));
         
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // table_row_reference objects are pretty neat with range-based for loops, C++
         
         // Regenerate entries in the first row.
         for(auto& x : t.row(0))
         x = (value = ( (A*value)%C));
         
         std::cout << "t (with first row regenerated): \n" << t << std::endl << std::endl;
         
         // Replace 5-th (index 4) column with new values from the linear congruential engine
         for(auto& y : t.column(4))
         y = (value = ( (A*value)%C));
         
         std::cout << "t (with fifth column regenerated): \n" << t << std::endl << std::endl;
         
         std::cout << "The first two columns are: \n";
         // print first two columns side by side
         dsl::print_side_by_side(t.column(0), t.column(1));
         
         std::cout << "\nThe last two rows (transposed) are: \n";
         // print last two rows side by side in column format
         dsl::print_side_by_side(t.row(8), t.row(9));
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{43,71,5,88,101,25,59,124,125,41},
         {112,117,78,52,77,9,6,4,45,30},
         {20,98,23,100,109,115,119,37,67,87},
         {58,81,54,36,24,16,53,120,80,11},
         {92,19,55,79,95,21,14,94,105,70},
         {89,17,96,64,85,99,66,44,114,76},
         {93,62,126,84,56,122,39,26,102,68},
         {3,2,86,15,10,49,75,50,118,121},
         {123,82,97,107,29,104,27,18,12,8},
         {90,60,40,69,46,73,91,103,111,74}}
         
         t (with first row regenerated):
         {{7,47,116,35,108,72,48,32,106,113},
         {112,117,78,52,77,9,6,4,45,30},
         {20,98,23,100,109,115,119,37,67,87},
         {58,81,54,36,24,16,53,120,80,11},
         {92,19,55,79,95,21,14,94,105,70},
         {89,17,96,64,85,99,66,44,114,76},
         {93,62,126,84,56,122,39,26,102,68},
         {3,2,86,15,10,49,75,50,118,121},
         {123,82,97,107,29,104,27,18,12,8},
         {90,60,40,69,46,73,91,103,111,74}}
         
         t (with fifth column regenerated):
         {{7,47,116,35,33,72,48,32,106,113},
         {112,117,78,52,22,9,6,4,45,30},
         {20,98,23,100,57,115,119,37,67,87},
         {58,81,54,36,38,16,53,120,80,11},
         {92,19,55,79,110,21,14,94,105,70},
         {89,17,96,64,31,99,66,44,114,76},
         {93,62,126,84,63,122,39,26,102,68},
         {3,2,86,15,42,49,75,50,118,121},
         {123,82,97,107,28,104,27,18,12,8},
         {90,60,40,69,61,73,91,103,111,74}}
         
         The first two columns are: 
         7  47
         112  117
         20  98
         58  81
         92  19
         89  17
         93  62
         3  2
         123  82
         90  60
         
         The last two rows (transposed) are: 
         123  90
         82  60
         97  40
         107  69
         28  61
         104  73
         27  91
         18  103
         12  111
         8  74
         @endcode
         */
        class table_row_reference {
            
        public:
            /** Constructors with default parameters.\n
                With 2 parameters, specifies the table and the row number.\n
                With 1 parameter, specifies the table, row is 0 by default.\n
                With 0 parameters, table is nullptr and row is 0 by default.
                @param m is a table pointer.
                @param r is the row number for which to reference.
            */
            table_row_reference(table* m=nullptr, int r=0) : mat(m), row(r) { };
            
            /** Member begin function so can be used in generic algorithms and range-based for loops.
                @returns iterator referring to the first element of the indicated row of the table.
            */
            row_iterator begin() const { return mat->begin_row(row); };

            /** Member end function so can be used in generic algorithms and range-based for loops.
             @returns iterator referring to one after the last element of the indicated row of the table.
             */
            row_iterator end()   const { return mat->end_row  (row); };
            
            template<typename V>
            table_row_reference& operator=(const V& v) {
                
                // start with first element of v
                auto start = std::begin(v);
                
                // copy over elements in v one by one.
                for(size_t j=0;j<mat->n;++j)
                    mat->entry[row*mat->n+j] = *start++;
                
                return *this;
            }
            
            template<typename InputIterator>
            table_row_reference& operator=(std::pair<InputIterator,InputIterator> iterators) {
                
                auto start = iterators.first;
                auto ending = iterators.second;
                
                // column index
                auto j = 0;

                // loop until either the table ends or the iterator container ends
                while(j < mat->n && start != ending)
                    mat->entry[row*mat->n+j] = *start++;
                
                return *this;
            }
            
        private:
            int row;
            table* const mat;
        };
        
        /** Used in range-based for loops.
            @code
         #include <iostream>
         #include "desalvo/table.h"
         #include "std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create 10x10 table, default initialized values (i.e., 0 for int)
         dsl::table<int> t(10,10);
         
         for(auto& x : t.row(3))
         x = 5;
         
         std::cout << t << std::endl;
         
         return 0;
         }

            @endcode
         Should produce output
            @code
         {{0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0},
         {5,5,5,5,5,5,5,5,5,5},
         {0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0}}
         @endcode
            @param i is a row number.
            @returns a reference to a given row of the table.
        */
        table_row_reference row(int i) {
            return table_row_reference(this, i);
        }
        
        
        
        /** @class table_column_reference
         @brief reference to a column of a table, useful for range-based for loops, C++11
         
         This is to be used to access columns of a table.  The object can be used in a range-based for loop.
         
         @code
         
         #include <iostream>
         #include <vector>
         #include "desalvo/table.h"
         #include "std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create 10x10 table, default initialized values (i.e., 0 for int)
         dsl::table<int> t(10,10);
         
         // Very simple linear congruential engine
         int A = 16807;
         int C = 127;
         int value = 1;
         
         // initialize values using custom linear congruential engine
         for(auto& i : t)
         i = (value = ( (A*value)%C));
         
         std::cout << "t: \n" << t << std::endl << std::endl;
         
         // table_row_reference objects are pretty neat with range-based for loops, C++
         
         // Regenerate entries in the first row.
         for(auto& x : t.row(0))
         x = (value = ( (A*value)%C));
         
         std::cout << "t (with first row regenerated): \n" << t << std::endl << std::endl;
         
         // Replace 5-th (index 4) column with new values from the linear congruential engine
         for(auto& y : t.column(4))
         y = (value = ( (A*value)%C));
         
         std::cout << "t (with fifth column regenerated): \n" << t << std::endl << std::endl;
         
         std::cout << "The first two columns are: \n";
         // print first two columns side by side
         dsl::print_side_by_side(t.column(0), t.column(1));
         
         std::cout << "\nThe last two rows (transposed) are: \n";
         // print last two rows side by side in column format
         dsl::print_side_by_side(t.row(8), t.row(9));
         
         return 0;
         }
         @endcode
         Should produce output
         @code
         t:
         {{43,71,5,88,101,25,59,124,125,41},
         {112,117,78,52,77,9,6,4,45,30},
         {20,98,23,100,109,115,119,37,67,87},
         {58,81,54,36,24,16,53,120,80,11},
         {92,19,55,79,95,21,14,94,105,70},
         {89,17,96,64,85,99,66,44,114,76},
         {93,62,126,84,56,122,39,26,102,68},
         {3,2,86,15,10,49,75,50,118,121},
         {123,82,97,107,29,104,27,18,12,8},
         {90,60,40,69,46,73,91,103,111,74}}
         
         t (with first row regenerated):
         {{7,47,116,35,108,72,48,32,106,113},
         {112,117,78,52,77,9,6,4,45,30},
         {20,98,23,100,109,115,119,37,67,87},
         {58,81,54,36,24,16,53,120,80,11},
         {92,19,55,79,95,21,14,94,105,70},
         {89,17,96,64,85,99,66,44,114,76},
         {93,62,126,84,56,122,39,26,102,68},
         {3,2,86,15,10,49,75,50,118,121},
         {123,82,97,107,29,104,27,18,12,8},
         {90,60,40,69,46,73,91,103,111,74}}
         
         t (with fifth column regenerated):
         {{7,47,116,35,33,72,48,32,106,113},
         {112,117,78,52,22,9,6,4,45,30},
         {20,98,23,100,57,115,119,37,67,87},
         {58,81,54,36,38,16,53,120,80,11},
         {92,19,55,79,110,21,14,94,105,70},
         {89,17,96,64,31,99,66,44,114,76},
         {93,62,126,84,63,122,39,26,102,68},
         {3,2,86,15,42,49,75,50,118,121},
         {123,82,97,107,28,104,27,18,12,8},
         {90,60,40,69,61,73,91,103,111,74}}
         
         The first two columns are:
         7  47
         112  117
         20  98
         58  81
         92  19
         89  17
         93  62
         3  2
         123  82
         90  60
         
         The last two rows (transposed) are:
         123  90
         82  60
         97  40
         107  69
         28  61
         104  73
         27  91
         18  103
         12  111
         8  74
         @endcode
         */
        class table_column_reference {
            
        public:
            /** Constructors with default parameters.\n
             With 2 parameters, specifies the table and the column number.\n
             With 1 parameter, specifies the table, column is 0 by default.\n
             With 0 parameters, table is nullptr and column is 0 by default.
             @param m is a table pointer.
             @param r is the column number for which to reference.
             */
            table_column_reference(table* m=nullptr, int col=0) : mat(m), column(col) { };
            
            /** Member begin function so can be used in generic algorithms and range-based for loops.
             @returns iterator referring to the first element of the indicated column of the table.
             */
            column_iterator begin() const { return mat->begin_column(column); };
            
            /** Member end function so can be used in generic algorithms and range-based for loops.
             @returns iterator referring to one after the last element of the indicated column of the table.
             */
            column_iterator end()   const { return mat->end_column  (column); };
            
        private:
            int column;
            table* const mat;
        };
        
        
        /** Used in range-based for loops.
         @code
         #include <iostream>
         #include "desalvo/table.h"
         #include "std_cout.h"
         
         namespace dsl = desalvo_standard_library;
         
         int main(int argc, const char * argv[]) {
         
         // Create 10x10 table, default initialized values (i.e., 0 for int)
         dsl::table<int> t(10,10);
         
         for(auto& x : t.column(3))
             x = 5;
         
         std::cout << t << std::endl;
         
         return 0;
         }
         
         @endcode
         Should produce output
         @code
         {{0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0},
         {0,0,0,5,0,0,0,0,0,0}}
         @endcode
         @param i is a column number.
         @returns a reference to a given column of the table.
         */
        table_column_reference column(int i) {
            return table_column_reference(this, i);
        }
        
        
        
        /** Computes the row sums, @returns row sums. */
        //        template<typename Container = table<m,1,ValueType> >
        template<typename Container = std::vector<ValueType> >
        Container row_sums()  {
            Container t(m);
            auto v = std::begin(t);
            for(size_t i=0;i<m;++i)
                *v++ = std::accumulate(begin_row(i), end_row(i), static_cast<WorkingPrecision>(0));
            
            return t;
        }
        
        
        /** Computes the row sums, @returns row sums. */
        //        table<1,n,ValueType> ColumnSums() {
        template<typename Container = std::vector<ValueType> >
        Container column_sums()  {
            Container t(n);
            auto v = std::begin(t);
            for(size_t i=0;i<n;++i) {
                *v++ = std::accumulate(begin_column(i), end_column(i), static_cast<WorkingPrecision>(0));
            }
            
            return t;
        }
        
        /** Computes the row sums, @returns row sums. */
        //        table<1,n,ValueType> ColumnSums() {
        WorkingPrecision column_sum(size_t column_index)  {
            
            WorkingPrecision t = std::accumulate(begin_column(column_index), end_column(column_index), static_cast<WorkingPrecision>(0));
            
            return t;
        }
        
        /** For each row, calculates the row sums, divides each element by it, so that each row sums to 1.*/
        void normalize_by_row_sums() {
            
            // Calculate row sums, store in an std::vector by default
            auto r_sums = row_sums();
            
            // Iterate through each row, dividing each element by its row sum.
            for(size_t i=0;i<m;++i)
                for(auto& x : row(i))
                    x /= r_sums[i];
        }
        
        
        /** For each column, calculates the column sums, divides each element by it, so that each column sums to 1.*/
        void normalize_by_column_sums() {
            
            // Calculate column sums, store in an std::vector by default.
            auto c_sums = column_sums();
            
            // Iterate through each column, dividing each element by its column sum.
            for(size_t j=0;j<n;++j)
                for(auto& x : column(j))
                    x /= c_sums[j];
        }
        
        /** Normalize each row by the l_p norm.
            @param p is the norm parameter.
        */
        void normalize_rows_by_lp(int p=2) {
            
            WorkingPrecision r_sums = 0.;
            
            // For each row ...
            for(size_t i=0;i<m;++i) {
                r_sums = 0.;
                
                // Calculate the l_p norm by iterating through all elements in the row ...
                for(auto& x : row(i))
                    r_sums += static_cast<WorkingPrecision>(pow(static_cast<WorkingPrecision>(x),p));
                
                r_sums = pow(r_sums,static_cast<WorkingPrecision>(1./p));
                
                // then iterator through all elements in the row again, this time dividing by the calculated l_p norm.
                for(auto& x : row(i))
                    x /= r_sums;
            }
            
        }
        
        /** Normalize each column by the l_p norm.
         @param p is the norm parameter.
         */
        void normalize_columns_by_lp(int p=2) {
            
            WorkingPrecision c_sums = 0.;
            
            // For each column ...
            for(size_t j=0;j<n;++j) {

                c_sums = 0.;
                
                // Calculate the l_p norm by iterating through all elements in the row ...
                for(auto& x : column(j))
                    c_sums += pow(static_cast<WorkingPrecision>(x),p);
                
                c_sums = pow(c_sums,static_cast<WorkingPrecision>(1./p));
                
                // then iterator through all elements in the row again, this time dividing by the calculated l_p norm.
                for(auto& x : column(j))
                    x /= c_sums;
            }
            
        }
        
        /** Calculate the l_p norms of each row
            @param p is the norm parameter
            @returns a container with the l_p norms of each row, by default an std::vector
        */
        template<typename Container = std::vector<WorkingPrecision> >
        Container row_lp_norms(int p=2) {
            
            Container v(m);
            auto start = std::begin(v);
            
            WorkingPrecision r_sums = 0.;
            
            // For each row ...
            for(size_t i=0;i<m;++i) {
                r_sums = 0.;
                
                // Iterator through each element to calculate the l_p norm
                for(auto& x : row(i))
                    r_sums += pow(static_cast<WorkingPrecision>(x),p);
                
                // Final normalization by taking the (1/p)-th power.
                *start++ = pow(r_sums,static_cast<WorkingPrecision>(1./p));
            }
            return v;
        }
        
        /** Calculate the l_p norms of each column
         @param p is the norm parameter
         @returns a container with the l_p norms of each column, by default an std::vector
         */
        template<typename Container = std::vector<WorkingPrecision> >
        Container column_lp_norms(int p=2) {

            Container v(n);
            auto start = std::begin(v);
            WorkingPrecision c_sums = 0.;
            
            // For each column ...
            for(size_t j=0;j<n;++j) {
                c_sums = 0.;
                
                // iterate through each element to calculate the l_p norm
                for(auto& x : column(j))
                    c_sums += pow(static_cast<WorkingPrecision>(x),p);
                
                // Final step is to take the (1/p)-th power.
                *start++ = pow(c_sums,static_cast<WorkingPrecision>(1./p));
            }
            return v;
        }
        
        /** Returns the sum of all entries.
            @returns the sum of all entries.
        */
        WorkingPrecision sum() { return std::accumulate(entry, entry+m*n, 0); }
        
        /** Returns the average (i.e., mean) of all entries.
         @returns the average (i.e., mean) of all entries.
         */
        WorkingPrecision mean() { return sum() / (double)(m*n); }
        
        /** Returns the average of all entries.  Exists to prevent idiots from not liking the library.
         @returns the average of all entries.
         */
        WorkingPrecision average() { return mean(); }
        
        /*
         table<ValueType>& operator-=(table<ValueType> t) {
         transform(entry, entry+m*n, t.entry, entry, std::minus<ValueType>());
         return *this;
         }
         
         table<ValueType> operator-(table<ValueType> t) {
         table<ValueType> s;
         transform(entry, entry+m*n, t.entry, s.entry, std::minus<ValueType>());
         return s;
         }*/
        
        /** Copies ALL elements of a container starting at a position given by an iterator.
            @param v is the container of elements
            @param it is an iterator.
        */
        template<typename V, typename Iterator>
        void insert(V&& v, Iterator&& it) {
            //std::copy( std::cbegin(v), std::cend(v), it);
            std::copy( std::begin(v), std::end(v), it);
            
        }
        
        /** Applies a permutation to the entries of a table, where the table is treated like a 1D array.
            @param permutation_map is a permutation of entries.
        */
        template<typename V>
        void apply_permutation_map(V&& permutation_map) {
            
            auto it = this->begin();
            auto stop = this->end();
            
            while(it != stop) {
                *it = permutation_map[*it-1];
                ++it;
            }
            
        }
        
        /** Permutes rows by a given permutation, assuming the first row is labelled 0.
            @param permutation_indices is a rearrangement of the numbers 0,1,...,k for which to reorder the rows.
        */
        template<typename V>
        void permute_rows(V&& permutation_indices) {
            
            // inefficient copy, but makes life easier
            auto t = *this;
            
            for(size_t i=0;i<m;++i) {
                //std::cout << t << std::endl;
                std::copy_n(begin_row(i), n, t.begin_row(permutation_indices[i]));
            }
            //std::cout << t << std::endl;
            
            *this = t;
            
            /* // old, buggy algorithm
            std::vector<size_t> indices( permutation_indices.size(), -1 );
            
            auto it = std::begin(permutation_indices);
            auto first = std::begin(permutation_indices);
            auto stop = std::end(permutation_indices);
            auto it_runner = first;
            auto it_indices = std::begin(indices);
            
            
            while(it != stop) {
                it_runner = first;
                it_indices = std::begin(indices);
                while(it_runner != stop) {
                    if(*it_runner >= *it)
                        (*it_indices)++;
                    ++it_runner;
                    ++it_indices;
                }
                ++it;
            }
            
            // Convert the permutation to a product of transpositions
            auto trans = dsl::permutation_as_product_of_transpositions(indices);
            
            // Simply swap rows according to the permutations interpretation as a collection of row swaps.
            for(auto& x : trans)
                swap_rows( permutation_indices[x[0]], permutation_indices[x[1]] );
            */
        }
        
        /** Permutes columns by a given permutation, assuming the first column is labelled 0.
         @param permutation_indices is a rearrangement of the numbers 0,1,...,k for which to reorder the columns.
         */
        template<typename V>
        void permute_columns(V&& permutation_indices) {
            
            // inefficient copy, but makes life easier
            auto t = *this;
            
            for(size_t j=0;j<n;++j) {
                //std::cout << t << std::endl;
                std::copy_n(begin_column(j), m, t.begin_column(permutation_indices[j]));
            }
            //std::cout << t << std::endl;
            
            *this = t;
            

            /* // Old, buggy way
            std::vector<size_t> indices( permutation_indices.size(), -1 );
            
            auto it = std::begin(permutation_indices);
            auto first = std::begin(permutation_indices);
            auto stop = std::end(permutation_indices);
            auto it_runner = first;
            auto it_indices = std::begin(indices);
            
            
            while(it != stop) {
                it_runner = first;
                it_indices = std::begin(indices);
                while(it_runner != stop) {
                    if(*it_runner >= *it)
                        (*it_indices)++;
                    ++it_runner;
                    ++it_indices;
                }
                ++it;
            }
            
            // Convert the permutation to a product of transpositions
            auto trans = dsl::permutation_as_product_of_transpositions(indices);
            
            // Simply swap columns according to the permutations interpretation as a collection of column swaps.
            
            for(auto& x : trans)
                swap_columns( permutation_indices[x[0]], permutation_indices[x[1]] );
            */
            
        }
        
        /*        template<typename V=std::vector<ValueType>>
         void insert(const V& v, const table<m,n,ValueType>::tableRowIterator& it) {
         //std::copy( std::cbegin(v), std::cend(v), it);
         std::copy( std::begin(v), std::end(v), it);
         
         }
         
         template<typename V>
         void insert(const V& v, const table<m,n,ValueType>::tableColumnIterator& it) {
         //std::copy( std::cbegin(v), std::cend(v), it);
         std::copy( std::begin(v), std::end(v), it);
         }
         
         template<typename V>
         void insert(V&& v, const table<m,n,ValueType>::tableColumnIterator& it) {
         //std::copy( std::cbegin(v), std::cend(v), it);
         std::copy( std::begin(v), std::end(v), it);
         }
         */
        
        /** Swaps the entries of the two rows indicated by the input parameters.
            @param i is the first row
            @param j is the second row.
        */
        void swap_rows(size_t i, size_t j) {
            
            // If i == j, do nothing.  Otherwise,
            if(i != j) {
                
                // Start your engines ... i.e., get iterators to starting locations of entries.
                auto row_i = begin_row(i);
                auto row_j = begin_row(j);
                auto stop = end_row(i);
                
                // Swap elements one at a time.
                while(row_i != stop) {
                    std::swap( *row_i++, *row_j++);
                }
            }
        }
        
        /** Swaps the entries of the two rows indicated by the input parameters.
         @param i is the first row
         @param j is the second row.
         */
        void swap_columns(size_t i, size_t j) {

            // If i == j, do nothing.  Otherwise,
            if(i != j) {
                
                // Start your engines ... i.e., get iterators to starting locations of entries.
                auto col_i = begin_column(i);
                auto col_j = begin_column(j);
                auto stop = end_column(i);
                
                // Swap elements one at a time.
                while(col_i != stop) {
                    std::swap( *col_i++, *col_j++);
                }
            }
        }
        
        /** Create a new container with elements from the entries of a given row, the template parameter specifies the container type, which must supply an input iterator.
            @tparam Container is any class which supplies a .begin() member function and an input iterator.
            @param i is the row
            @returns a container with the given row values stored in the container.
        */
        template<typename Container = std::vector<ValueType>>
        Container row_as(size_t i) {
            Container c(n);
            auto it = std::begin(c);
            auto start = begin_row(i);
            auto stop = end_row(i);
            
            std::copy(start, stop, it);
            return c;
        }
        
        /** Create a new container with elements from the entries of a given column, the template parameter specifies the container type, which must supply an input iterator.
         @tparam Container is any class which supplies a .begin() member function and an input iterator.
         @param i is the column
         @returns a container with the given row values stored in the container.
         */
        template<typename Container = std::vector<ValueType>>
        Container column_as(size_t i) {
            Container c(n);
            auto it = std::begin(c);
            auto start = begin_column(i);
            auto stop = end_column(i);
            
            std::copy(start, stop, it);
            return it;
        }
        
        /** Creates a string so that the table looks like the input for a Matlab array.
            @returns a string of values which can be copied into a Matlab terminal.
        */
        std::string as_one_line_matlab_table() {
            
            std::string line = "[";
            
            // Iterator through each row
            for(size_t i=0;i<n-1;++i) {
                auto it = this->begin_row(i);
                auto stop = this->end_row(i)-1;
                
                while(it != stop) {
                    line += std::to_string( *it++ );
                    line += ",";
                }
                line += std::to_string(*it);
                line += ";";
            }
            auto it = this->begin_row(n-1);
            auto stop = this->end_row(n-1)-1;
            
            while(it != stop) {
                line += std::to_string( *it++ );
                line += ",";
            }
            line += std::to_string(*it);
            line += "];";
            return line;
            
        }

    };
    
    template<typename ValueType, typename WorkingPrecision>
    bool operator!=(const table<ValueType,WorkingPrecision>& lhs, const table<ValueType,WorkingPrecision>& rhs) { return !(lhs == rhs); }
    
    
}




#endif
