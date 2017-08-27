/** @file file.h
  @author Stephen DeSalvo
  @date July, 2015
  @brief Operations on Reading/Writing to files
 
  Access to file operations that follows the RAII paradigm.  Designed for simple access to file read/write functionality.  Uses PTS so easily extendible to other cases like append, append_sequence, etc.
 
  file < input/output/console > f;
 */

#ifndef Combinatorics_FileSystem_h
#define Combinatorics_FileSystem_h

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>


namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    // These typedefs and strategy for forwarding all input to the underlying stream come from the first answer in:
    // http://stackoverflow.com/questions/1133739/how-does-ofstream-or-ostream-type-cast-all-types-to-string/1134501#1134501

    /** @brief abbrev. for type 1 manipulators */
    typedef std::ostream& (*manip1)( std::ostream& );
    
    /** @brief abbrev. for use with typedef for type 2 manipulators */
    typedef std::basic_ios< std::ostream::char_type, std::ostream::traits_type > ios_type;

    /** @brief abbrev. for type 2 manipulators */
    typedef ios_type& (*manip2)( ios_type& );
    
    /** @brief abbrev. for type 3 manipulators */
    typedef std::ios_base& (*manip3)( std::ios_base& );
    
    
    // Undocumented, for personal use
    namespace path {
        std::string Teaching = "/Users/stephendesalvo/Documents/Teaching/";
        std::string Research = "/Users/stephendesalvo/Documents/Research/";
        std::string Permutahedron = "/Users/stephendesalvo/Documents/Research/PermutahedronVisualization/";
    };
    
    /** @enum file_type controls the type of file */
    enum class file_type { input, output, console };
    
    
    /** @class file
      @brief Partially specialized for input and output
     
      This class is designed to be extended in various directions.\n
      1.  input -- handle for input from a file\n
      2.  output -- handle for outputting to a file\n
      3.  console -- handle for std::cin and std::cout
     
      The plan is to make the following extensions
      3.  SmartOutput -- auto detects if file already exists when writing and changes name
      4.  AppendOutput -- Appends to already existing file
      5.  IncrementalOutput -- Used when needing to write many files with same name but different indices
      e.g., Data_n_is_2, Data_n_is_4, Data_n_is_8, etc.
     
     
    Example 1:
     @code
     std::string filename = "/Users/stephendesalvo/Documents/";
     
     dsl::file< output > f(filename + "file.txt");
     dsl::file< input  > f_in(filename + "file_to_read_from.txt");
     
     dsl::file< console > screen;
     
     //f.write("Hi there!", "\n");
     f << "Hi there!";
     
     std::string text;
     
     f_in.getline(text);
     
     std::cout << text << std::endl;
     
     screen << text << std::endl;
     
     screen.write(text);
     
     screen.read(text);
     screen.write(text, "\n");
     screen.write(text, std::endl);
     
     screen.getline(text);
     screen.write(text);
     
     screen << "Same as before \n";
     screen << "Can do endl as well" << std::endl;
     @endcode
     
     Example 2: Produces a file which contains random numbers between 1 and 6.
     @code
     // Program to generate random dice rolls
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     #include <random>
     #include "desalvo/statistics.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "dice_data.txt");
     
     std::uniform_int_distribution<int> unif(1,6);
     
     file << "{";
     for(auto i=0;i<9999;++i)
     file << unif(dsl::generator_64) << ",";
     
     file << unif(dsl::generator_64) << "}";
     
     return 0;
     }
     @endcode
     
     Example 3: Generate a matrix and store it in a file
     @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     dsl::file< dsl::file_type::output > make_matlab_matrix(prefix + "matlab_matrix.txt");
     
     size_t m = 10;
     size_t n = 5;
     
     make_matlab_matrix << "[";
     for(size_t i=0;i<m;++i) {
     for(size_t j=0;j<n-1;++j) {
     make_matlab_matrix << 1./(1.+i+j) << ",";
     }
     make_matlab_matrix << 1./(1.+i+(n-1.)) << ";";
     }
     make_matlab_matrix << "]";
     
     return 0;
     }
     @endcode
     */
    template<file_type type>
    class file;
    
    template<> class file <file_type::input> {
        
        friend bool getline(file& fin, std::string& s);
        
    public:
        
        // Reminder that we do not let a file be created by default.
        file() = delete;
        file(const std::string& initial_filename);
        file(file&& other);
        file& operator=(file&& other);
        ~file();

        template<typename Streamsize = size_t>
        file& ignore(Streamsize n = 1, int delim = EOF);
        
        template<typename T> file& operator>>(T& p);
        template<typename T> file& read(T& t);
        
        template<typename String = std::string> bool getline(String& line);
        operator bool();
        
        void reset();
        
    private:
        /** @var file is the file stream object for writing */
        std::fstream file_stream;
        std::string filename;
    };
    
    
    template<>
    class file < file_type::output > {
    private:
        /** @var file is the file stream for outputting data */
        std::fstream file_stream;
        
        /** @var p is the set of flags for outputting */
        std::fstream::openmode p;
    public:
        
        file(std::string filename, std::fstream::openmode additional_modes = std::fstream::out, size_t output_precision = 10);
        file(file&& other);
        file& operator=(file&& other);
        ~file();
        
        template<typename T> file& operator<<(T&& t);
        template<typename T> file& write(T&& t);
        

        
        file& operator<<( manip1 fp );
        file& operator<<( manip2 fp );
        file& operator<<( manip3 fp );
        
        template<typename T> file& operator<<(const std::vector<T>& v);
    };
    

    
    template<> class file <file_type::console> {
        /** @var p is the set of flags for outputting */
        std::fstream::openmode p;

        template<typename String>
        friend bool getline(file& fin, String& s);
        
    public:

        file(std::fstream::openmode additional_modes=std::fstream::out, size_t output_precision = 10);
        
        template<typename T> file& read(T&& p);
        
        //template<typename T> file& operator>>(std::vector<T>& v);
        
        operator bool();
        
        template<typename T> file& operator>>(T&& p);
        template<typename T> file& operator<<(T&& t);
        
        template<typename T> file& write(T&& t);
        
        template<typename Streamsize=size_t>
        file& ignore(Streamsize n = 1, int delim = EOF);
        
        template<typename T, typename String = std::string> file& write(T&& t, String&& ending=std::string(""));
        
        template<typename T> file& write(T&& t, manip1 ending);
        template<typename T> file& read(T& t);

        file& operator<<( manip1 fp );
        file& operator<<( manip2 fp );
        file& operator<<( manip3 fp );
        
        file& operator<<(const std::vector<int>& v);
    };
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    // *********** Begin implementation for file_type::input ***********
    
    /**  Gets a line from the file
     @param fin is the File object
     @param s stores the line from the file
     @returns true/false according to getline acting on the stream
     @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file<dsl::file_type::input> text(prefix + "data_richard_iii_opening_monologue.txt");
     
     std::vector<std::string> v(5);
     
     std::cout << "Let's get the first few lines of Richard III's opening monologue:" << std::endl;
     dsl::getline(text, v[0]);
     dsl::getline(text, v[1]);
     dsl::getline(text, v[2]);
     dsl::getline(text, v[3]);
     dsl::getline(text, v[4]);
     
     std::cout << v << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     Let's get the first few lines of Richard III's opening monologue:
     {Now is the winter of our discontent,Made glorious summer by this sun of York;,And all the clouds that lour'd upon our house,In the deep bosom of the ocean buried.,Now are our brows bound with victorious wreaths;}
     @endcode
     */
    bool getline(file<file_type::input>& fin, std::string& s) {
        std::getline( fin.file_stream, s);
        return fin;
    }
    
    
    /** Initialize using filename
     @param filename contains the name of the file
     @code
     // Program to load in random dice rolls
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     // Prepare file for loading in values.
     dsl::file< dsl::file_type::input > file(prefix + "data_dice.txt");
     
     // We shall store them here
     std::vector<int> rolls;
     
     // Grab values.  We don't know how many in advance so by default it calls push_back
     file >> rolls;
     
     std::cout << rolls << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output to the console depending on contents of file, e.g.,
     @code
     {1,6,4,3,2,2,3,1,5,6}
     @endcode
     */
    file<file_type::input>::file(const std::string& input_filename) : filename(input_filename) {
        file_stream.open(filename, std::fstream::in);
        
        // Cannot continue with code if the file doesn't exist.
        if(file_stream.fail()) std::runtime_error{"Error: opening file "+filename};
    }
    
    /** Move constructor
     @param other is the expiring file stream
     @code
     // Program to load in random dice rolls
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     // Prepare file for loading in values.
     dsl::file< dsl::file_type::input > file(prefix + "data_dice.txt");
     
     // Changed my mind, would prefer to use file_pair handle
     dsl::file< dsl::file_type::input > file_pair(std::move(file));
     
     // We shall store them here
     std::vector<int> rolls;
     
     // Grab values.  We don't know how many in advance so by default it calls push_back
     file_pair >> rolls;
     
     std::cout << rolls << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output to the console depending on contents of file, e.g.,
     @code
     {1,6,4,3,2,2,3,1,5,6}
     @endcode
     */
    file<file_type::input>::file(file&& other) : file_stream(std::move(other.file_stream)), filename(other.filename) {
        if(file_stream.fail()) std::runtime_error{"Error: moving file"};
    }
    
    /** move assignment operator
     @param other is the expiring file stream
     @returns reference to file for chaining
     @code
     // Program to load in random dice rolls
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     // Prepare file for loading in values.
     dsl::file< dsl::file_type::input > file(prefix + "data_dice.txt");
     dsl::file< dsl::file_type::input > file_pair(prefix + "data_pair.txt");
     
     // Changed my mind, would prefer to use file_pair, so safely close data_pair.txt and take over data_dice.txt
     file_pair = std::move(file);
     
     // We shall store them here
     std::vector<int> rolls;
     
     // Grab values.  We don't know how many in advance so by default it calls push_back
     file_pair >> rolls;
     
     std::cout << rolls << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output to the console depending on contents of file, e.g.,
     @code
     {1,6,4,3,2,2,3,1,5,6}
     @endcode
     */
    file<file_type::input>& file<file_type::input>::operator=(file<file_type::input>&& other) {
        file_stream = std::move(other.file_stream);
        filename = other.filename;
        return *this;
    }
    
    
    /** Passes along the input to the stream, object must have operator>>(istream&,T&) -> istream& defined
     @tparam T is the type of object
     @param p is the stream input
     @returns reference to the File object for chaining
     @code
     // Program to load in random dice rolls
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     // Prepare file for loading in values.
     dsl::file< dsl::file_type::input > file(prefix + "data_dice.txt");
     
     // We shall store them here
     std::vector<int> rolls;
     
     // Grab values.  We don't know how many in advance so by default it calls push_back
     file >> rolls;
     
     std::cout << rolls << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output to the console depending on contents of file, e.g.,
     @code
     {1,6,4,3,2,2,3,1,5,6}
     @endcode
     */
    template<typename T>
    file<file_type::input>& file<file_type::input>::operator>>(T& p) {
        file_stream >> std::forward<T&>(p);
        return *this;
    }
    
    /** Loads in next value of type T from stream
     @tparam T must have operator>>(istream&, T&) defined
     @param t is an object reference
     @returns false if file is no longer in valid state
     @code
     
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
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
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // For outputting to console
     dsl::file<dsl::file_type::console> console;
     
     // For reading in data from file
     dsl::file<dsl::file_type::input> file(prefix + "data_collections.txt");
     
     std::pair<int, int> my_pair;
     std::vector<int> v;
     std::set<double> v2;
     std::set<int> v3;
     std::multiset<int> v4;
     std::list<Point2D> v5;
     std::valarray<double> v6;
     std::array<Point2D,4> v7;
     
     console << "Get values in the same format as the default dsl output from file: data_collections.txt \n";
     console << "Loading pair values: \n";         file.read(my_pair);
     console << "Loading vector of ints: \n";       file.read(v);
     console << "Loading set of doubles: \n";       file.read(v2);
     console << "Loading set of ints: \n";          file.read(v3);
     console << "Loading multiset of ints: \n";     file.read(v4);
     console << "Loading list of Point2Ds: \n";     file.read(v5);
     console << "Loading valarray of doubles: \n";  file.read(v6);
     console << "Loading array of 4 Point2Ds: \n";  file.read(v7);
     
     console << "pair stored as: ";      console.write(my_pair,std::endl);
     console << "vector of ints: ";      console.write(v, std::endl);
     console << "set of doubles: ";      console.write(v2,std::endl);
     console << "set of ints: ";         console.write(v3,std::endl);
     console << "multiset of ints: ";    console.write(v4,std::endl);
     console << "list of Point2Ds: ";    console.write(v5,std::endl);
     console << "valarray of doubles: "; console.write(v6,std::endl);
     console << "array of Point2D: ";    console.write(v7,std::endl);
     
     return 0;
     }
     @endcode
     The file data_collections.txt is
     @code
     {1,2}
     {5,4,2,1}
     {1.1,1.1,2.2,5.4,6.5,3.14159}
     {1,1,1,1,2,2,2,3,3,4,5}
     {1,1,1,1,2,2,2,3,3,4,5}
     {(0,0),(-1.1,1),(-2,2)}
     {1.1,1.2,1.3,1.4,1.5}
     {(0,0),(1,1),(2,2),(3,3)}
     @endcode
     Should produce output
     @code
     Get values in the same format as the default dsl output from file: data_collections.txt
     Loading pair values:
     Loading vector of ints:
     Loading set of doubles:
     Loading set of ints:
     Loading multiset of ints:
     Loading list of Point2Ds:
     Loading valarray of doubles:
     Loading array of 4 Point2Ds:
     pair stored as: {1,2}
     vector of ints: {5,4,2,1}
     set of doubles: {1.1,2.2,3.14159,5.4,6.5}
     set of ints: {1,2,3,4,5}
     multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
     list of Point2Ds: {(0,0),(-1.1,1),(-2,2)}
     valarray of doubles: {1.1,1.2,1.3,1.4,1.5}
     array of Point2D: {(0,0),(1,1),(2,2),(3,3)}
     @endcode
     */
    template<typename T>
    file<file_type::input>& file<file_type::input>::read(T& t) {
        file_stream >> t;
        return *this;
    };
    
    
    
    void file<file_type::input>::reset() {
        file_stream.close();
        file_stream.open(filename);
    }

    
    /** Wrapper for the std::getline function
     @tparam String is the type of object to load into
     @param line contains the next line in the file.
     @returns whether the file is in a valid state after the get
     @code
     // Program to load in random dice rolls
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     // Prepare file for loading in values.
     dsl::file< dsl::file_type::input > file(prefix + "data_richard_iii_opening_monologue_short.txt");
     
     std::string line;
     
     while( dsl::getline(file, line) )
     std::cout << line << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     Now is the winter of our discontent
     Made glorious summer by this sun of York;
     And all the clouds that lour'd upon our house
     In the deep bosom of the ocean buried.
     Now are our brows bound with victorious wreaths;
     Our bruised arms hung up for monuments;
     Our stern alarums changed to merry meetings,
     Our dreadful marches to delightful measures.
     Grim-visaged war hath smooth'd his wrinkled front;
     And now, instead of mounting barded steeds
     To fright the souls of fearful adversaries,
     He capers nimbly in a lady's chamber
     To the lascivious pleasing of a lute.
     @endcode
     */
    template<typename String>
    bool file<file_type::input>::getline(String& line) {
        std::getline( file_stream, line);
        return *this;
    }
    
    
    /** Makes the file convertable to bool for use in conditional expressions
     @returns false when the stream fails
     @code
     // Program to load in random dice rolls
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     // Earlier, we made a file with numbers between 1 and 6 contained in dice_data.txt
     
     // Prepare file for loading in values.
     dsl::file< dsl::file_type::input > file(prefix + "data_richard_iii_opening_monologue_short.txt");
     
     std::string line;
     
     while( dsl::getline(file, line) )
     std::cout << line << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     Now is the winter of our discontent
     Made glorious summer by this sun of York;
     And all the clouds that lour'd upon our house
     In the deep bosom of the ocean buried.
     Now are our brows bound with victorious wreaths;
     Our bruised arms hung up for monuments;
     Our stern alarums changed to merry meetings,
     Our dreadful marches to delightful measures.
     Grim-visaged war hath smooth'd his wrinkled front;
     And now, instead of mounting barded steeds
     To fright the souls of fearful adversaries,
     He capers nimbly in a lady's chamber
     To the lascivious pleasing of a lute.
     @endcode
     */
    file<file_type::input>::operator bool() {
        return !file_stream.fail();
    }
    
    /** Closes out the stream.  +1 for RAII! */
    file<file_type::input>::~file() { file_stream.close(); };
    
    
    
    
    
    
    
    
    
    // *********** Begin implementation for file_type::output ***********
    
    
    
    
    /** Constructor via filename and various modes and options
     @param filename is the full filepath of the file
     @param additional_modes is a collection of options for formatting output
     @param output_precision is for outputting numerical floating point values
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     #include <random>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "data.txt");
     
     file << 3;
     file << '.';
     file << 1<<4<<1<<5<<9<<2<<6<<5;
     
     // too slow?
     std::string more_digits = "35897932384626433832795028841971693993751058209";
     
     file << more_digits;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see
     @code
     3.1415926535897932384626433832795028841971693993751058209
     @endcode
     */
    file<file_type::output>::file(std::string filename, std::fstream::openmode additional_modes, size_t output_precision) {
        
        p = std::fstream::out;
        p |= additional_modes;
        //        if(append) p |= std::fstream::app;
        
        file_stream.open(filename, p);
        
        if(file_stream.fail()) throw std::runtime_error{"Error: opening file"};
        
        file_stream.precision(output_precision);
    };
    
    /** Move constructor
     @param other is the expiring file stream
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     #include <random>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "data.txt");
     
     file << 3;
     file << '.';
     file << 1<<4<<1<<5<<9<<2<<6<<5;
     
     // too slow?
     std::string more_digits = "35897932384626433832795028841971693993751058209";
     
     file << more_digits;
     
     // Grab ownership of file, but ONLY via move constructor
     auto file2( std::move(file) );
     
     // Copy constructor is not defined, so line below will not compile
     //auto file3(file2);
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see
     @code
     3.141592653589793238462643383279502884197169399375105820974944
     @endcode
     */
    file<file_type::output>::file(file&& other) : file_stream(std::move(other.file_stream)) {
        if(file_stream.fail()) std::runtime_error{"Error: moving file"};
    }
    
    /** move assignment operator
     @param other is the expiring file stream
     @returns reference to file for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     #include <random>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "data.txt");
     
     file << 3;
     file << '.';
     file << 1<<4<<1<<5<<9<<2<<6<<5;
     
     // too slow?
     std::string more_digits = "35897932384626433832795028841971693993751058209";
     
     file << more_digits;
     
     dsl::file< dsl::file_type::output > file2(prefix + "data2.txt");
     
     // Throw in some digits of e in another file
     file2 << "2.718281828";
     
     // close out digits of e file, take ownership of digits of pi file
     file2 = std::move(file);
     
     // Copy constructor is not defined, so line below will not compile
     //file2 = file;
     
     file2 << "7494459230";
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see
     @code
     3.14159265358979323846264338327950288419716939937510582097494459230
     @endcode
     and in the file data2.txt we should see
     @code
     2.718281828
     @endcode
     */
    file<file_type::output>& file<file_type::output>::operator=(file<file_type::output>&& other) {
        file_stream = std::move(other.file_stream);
        return *this;
    }
    
    /** Outputs argument to underlying file stream
     @tparam t is the input for the file stream
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     #include <random>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "data.txt");
     
     file << 3;
     file << '.';
     file << 1<<4<<1<<5<<9<<2<<6<<5;
     
     // too slow?
     std::string more_digits = "35897932384626433832795028841971693993751058209";
     
     file << more_digits;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see
     @code
     3.1415926535897932384626433832795028841971693993751058209
     @endcode
     
     Example 2:
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     #include <iomanip>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "vector.txt");
     
     std::vector<int> v {3,1,4,1,5,9,2,6,5};
     
     file << v << std::endl;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file vector.txt we should see
     @code
     {3,1,4,1,5,9,2,6,5}
     @endcode
     */
    template<typename T>
    file<file_type::output>& file<file_type::output>::operator<<(T&& t) {
        file_stream << std::forward<T>(t);
        return *this;
    };
    
    /** Outputs argument to underlying file stream
     @tparam t is the input for the file stream
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     namespace dsl = desalvo_standard_library;
     
     class Point2D {
     friend std::ostream& operator<<(std::ostream& out, const Point2D& pt) {
     return out << "(" << pt.x << "," << pt.y << ")";
     }
     public:
     Point2D(double input_x, double input_y) : x(input_x), y(input_y) { };
     private:
     double x, y;
     };
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "data.txt");
     
     file << 3;
     file << '.';
     file << 1<<4<<1<<5<<9<<2<<6<<5;
     
     // too slow?
     std::string more_digits = "35897932384626433832795028841971693993751058209";
     
     file << more_digits << std::endl;
     
     double math_e = 2.718281828;
     
     file.write(math_e);
     file.write("\n");
     file.write(Point2D(3,4));
     file << std::endl;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see
     @code
     3.1415926535897932384626433832795028841971693993751058209
     2.718281828
     (3,4)
     @endcode
     */
    template<typename T>
    file<file_type::output>& file<file_type::output>::write(T&& t) {
        file_stream << std::forward<T>(t);
        return *this;
    };
    
    
    /** Output for manipulators, allows one to use std::endl as input
     @param fp is an argument like std::endl
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     #include <iomanip>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "values.txt");
     
     file << std::setprecision(20);
     
     file << 3.1415926535897932384626433832 << std::endl;
     
     file << std::setw(20) << std::left;
     
     file << 3 << std::setw(20) << 12 << std::setw(10) << 4 << std::setw(5) << 9987;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see (something like)
     @code
     3.141592653589793116
     3                   12                  4         9987
     @endcode
     */
    file<file_type::output>& file<file_type::output>::operator<<( manip1 fp ) {
        file_stream << fp;
        return *this;
    }
    /** Output for manipulators
     @param fp is an argument which manipulates the underlying file stream
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     #include <iomanip>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "values.txt");
     
     file << std::setprecision(20);
     
     file << 3.1415926535897932384626433832 << std::endl;
     
     file << std::setw(20) << std::left;
     
     file << 3 << std::setw(20) << 12 << std::setw(10) << 4 << std::setw(5) << 9987;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see (something like)
     @code
     3.141592653589793116
     3                   12                  4         9987
     @endcode
     */
    file<file_type::output>& file<file_type::output>::operator<<( manip2 fp ) {
        file_stream << fp;
        return *this;
    }
    /** Output for manipulators
     @param fp is an argument which manipulates the underlying file stream
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     #include <iomanip>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "values.txt");
     
     file << std::setprecision(20);
     
     file << 3.1415926535897932384626433832 << std::endl;
     
     file << std::setw(20) << std::left;
     
     file << 3 << std::setw(20) << 12 << std::setw(10) << 4 << std::setw(5) << 9987;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see (something like)
     @code
     3.141592653589793116
     3                   12                  4         9987
     @endcode
     */
    file<file_type::output>& file<file_type::output>::operator<<( manip3 fp ) {
        file_stream << fp;
        return *this;
    }

    /** Mimics the ignore function in the istream library; that is, it ignores the next n characters or until the delim character is reached, discarding the delim character.
       @tparam Streamsize is any unsigned integer type
       @param n is the number of characters to ignore
       @param delim is a char with which to stop reading characters
     @code
     
     @endcode
    */
    template<typename Streamsize>
    file<file_type::input>& file<file_type::input>::ignore(Streamsize n, int delim) {
        file_stream.ignore(n,delim);
        return *this;
    }
    
    
    /** Closes out the stream.  +1 for RAII! */
    file<file_type::output>::~file() { file_stream.close(); };
    




// *********** Begin implementation for file_type::console ***********


    /**  Gets a line from the file
     @param fin is the File object
     @param s stores the line from the file
     @returns true/false according to getline acting on the stream
     @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     dsl::file<dsl::file_type::console> console;
     
     std::vector<std::string> v(5);
     
     std::cout << "Let's try to recall the first few lines of Hamlet's to be or not to be speach" << std::endl;
     dsl::getline(console, v[0]);
     dsl::getline(console, v[1]);
     dsl::getline(console, v[2]);
     dsl::getline(console, v[3]);
     dsl::getline(console, v[4]);
     
     std::cout << "Let's see that again.\n";
     std::cout << v << std::endl;
     
     return 0;
     }
     @endcode
     Should produce output like the following:
     @code
     Let's try to recall the first few lines of Hamlet's to be or not to be speach
     To be, or not to be, that is the question
     whether tis nobler in the mind to suffer
     the slings and arrows of outrageous fortune
     or to take arms against a sea of troubles
     and by opposing, end them.  To die, to sleep
     Let's see that again.
     {To be, or not to be, that is the question,whether tis nobler in the mind to suffer,the slings and arrows of outrageous fortune,or to take arms against a sea of troubles,and by opposing, end them.  To die, to sleep}
     @endcode
     */
    template<typename String>
    bool getline(file<file_type::console>& fin, String& s) {
        std::getline( std::cin, s);
        return fin;
    }

    
    /** Constructor via filename and various modes and options
     @param filename is the full filepath of the file
     @param additional_modes is a collection of options for formatting output
     @param output_precision is for outputting numerical floating point values
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     
     #include <random>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     // Local file path as std::string for easy concatenation
     std::string prefix = "/Users/stephendesalvo/Documents/";
     
     dsl::file< dsl::file_type::output > file(prefix + "data.txt");
     
     file << 3;
     file << '.';
     file << 1<<4<<1<<5<<9<<2<<6<<5;
     
     // too slow?
     std::string more_digits = "35897932384626433832795028841971693993751058209";
     
     file << more_digits;
     
     return 0;
     }
     @endcode
     Doesn't produce any output to the console, but in the file data.txt we should see
     @code
     3.1415926535897932384626433832795028841971693993751058209
     @endcode
     */
    file<file_type::console>::file(std::fstream::openmode additional_modes, size_t output_precision) {
        
        p = std::fstream::out;
        p |= additional_modes;
        //        if(append) p |= std::fstream::app;
        
        std::cout.precision(output_precision);
    };
    
    /**
    Mimics the ignore function in the istream library; that is, it ignores the next n characters or until the delim character is reached, discarding the delim character.
    @tparam Streamsize is any unsigned integer type
    @param n is the number of characters to ignore
    @param delim is a char with which to stop reading characters
    @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     dsl::file<dsl::file_type::console> console;
     
     console << "Hello World!\n";
     
     std::vector<std::string> v(5);
     
     console << "Go ahead, write 5 words of text:" << std::endl;
     console >> v[0];
     console >> v[1];
     console >> v[2];
     console >> v[3];
     console >> v[4];
     
     // There is an extra return carriage we would very must like to ignore
     console.ignore();
     
     console << "Let me make sure I got that: \n";
     console << v << std::endl;
     
     console << "Now please write 5 lines of text:" << std::endl;
     dsl::getline(console, v[0]);
     dsl::getline(console, v[1]);
     dsl::getline(console, v[2]);
     dsl::getline(console, v[3]);
     dsl::getline(console, v[4]);
     
     console << "Let me make sure I got that: \n";
     console << v << std::endl;
     
     
     return 0;
     }
    @endcode
     Sample input/output
     @code
     Hello World!
     Go ahead, write 5 words of text:
     Now is the winter of
     Let me make sure I got that:
     {Now,is,the,winter,of}
     Now please write 5 lines of text:
     Now is the winter of our discontent
     Made glorious summer by this sun of York;
     And all the clouds that lour'd upon our house
     In the deep bosom of the ocean buried.
     Now are our brows bound with victorious wreaths;
     Let me make sure I got that:
     {Now is the winter of our discontent,Made glorious summer by this sun of York;,And all the clouds that lour'd upon our house,In the deep bosom of the ocean buried.,Now are our brows bound with victorious wreaths;}
     @endcode
    */
    template<typename Streamsize>
    file<file_type::console>& file<file_type::console>::ignore(Streamsize n, int delim) {
        std::cin.ignore(n,delim);
        return *this;
    }
    
    
    /** Passes along the input to the stream
     @tparam T is the type of object
     @param p is the stream input
     @returns reference to the File object for chaining
     @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     
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
     
     dsl::file<dsl::file_type::console> console;
     
     std::pair<int, int> my_pair;
     std::vector<int> v;
     std::set<double> v2;
     std::set<int> v3;
     std::multiset<int> v4;
     std::list<Point2D> v5;
     std::valarray<double> v6;
     std::array<Point2D,4> v7;
     
     console << "Input values in the same format as the default dsl output: \n";
     console << "Insert pair values: ";
     console >> my_pair;
     console << "Insert vector of ints: ";
     console >> v;
     console << "Insert set of doubles: ";
     console >> v2;
     console << "Insert set of ints: ";
     console >> v3;
     console << "Insert multiset of ints: ";
     console >> v4;
     console << "Insert list of Point2Ds: ";
     console >> v5;
     console << "Insert valarray of doubles: ";
     console >> v6;
     console << "Insert array of 4 Point2Ds: ";
     console >> v7;
     
     console << "pair stored as: " << my_pair << std::endl;
     console << "vector of ints: " << v << std::endl;
     console << "set of doubles: " << v2 << std::endl;
     console << "set of ints: " << v3 << std::endl;
     console << "multiset of ints: " << v4 << std::endl;
     console << "list of Point2Ds: " << v5 << std::endl;
     console << "valarray of doubles: " << v6 << std::endl;
     console << "array of Point2D: " << v7 << std::endl;
     
     
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
    file<file_type::console>& file<file_type::console>::operator>>(T&& p) {
        std::cin>>std::forward<T>(p);
        return *this;
    }

    

    
    /** Makes the file convertable to bool for use in conditional expressions */
    file<file_type::console>::operator bool() {
        return !std::cin.fail();
    }
    
    /** Outputs argument to underlying file stream
     @tparam t is the input for the file stream
     @returns a reference to the file object for chaining
     @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     
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
     
     dsl::file<dsl::file_type::console> console;
     
     std::pair<int, int> my_pair;
     std::vector<int> v;
     std::set<double> v2;
     std::set<int> v3;
     std::multiset<int> v4;
     std::list<Point2D> v5;
     std::valarray<double> v6;
     std::array<Point2D,4> v7;
     
     console << "Input values in the same format as the default dsl output: \n";
     console << "Insert pair values: ";
     console >> my_pair;
     console << "Insert vector of ints: ";
     console >> v;
     console << "Insert set of doubles: ";
     console >> v2;
     console << "Insert set of ints: ";
     console >> v3;
     console << "Insert multiset of ints: ";
     console >> v4;
     console << "Insert list of Point2Ds: ";
     console >> v5;
     console << "Insert valarray of doubles: ";
     console >> v6;
     console << "Insert array of 4 Point2Ds: ";
     console >> v7;
     
     console << "pair stored as: " << my_pair << std::endl;
     console << "vector of ints: " << v << std::endl;
     console << "set of doubles: " << v2 << std::endl;
     console << "set of ints: " << v3 << std::endl;
     console << "multiset of ints: " << v4 << std::endl;
     console << "list of Point2Ds: " << v5 << std::endl;
     console << "valarray of doubles: " << v6 << std::endl;
     console << "array of Point2D: " << v7 << std::endl;
     
     
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
    file<file_type::console>& file<file_type::console>::operator<<(T&& t) {
        std::cout << std::forward<T>(t);
        return *this;
    };
    
    /** Outputs argument to underlying file stream
     @tparam t is the input for the file stream
     @returns a reference to the file object for chaining
     @code
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
     
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
     
     dsl::file<dsl::file_type::console> console;
     
     std::pair<int, int> my_pair;
     std::vector<int> v;
     std::set<double> v2;
     std::set<int> v3;
     std::multiset<int> v4;
     std::list<Point2D> v5;
     std::valarray<double> v6;
     std::array<Point2D,4> v7;
     
     console << "Input values in the same format as the default dsl output: \n";
     console << "Insert pair values: ";
     console >> my_pair;
     console << "Insert vector of ints: ";
     console >> v;
     console << "Insert set of doubles: ";
     console >> v2;
     console << "Insert set of ints: ";
     console >> v3;
     console << "Insert multiset of ints: ";
     console >> v4;
     console << "Insert list of Point2Ds: ";
     console >> v5;
     console << "Insert valarray of doubles: ";
     console >> v6;
     console << "Insert array of 4 Point2Ds: ";
     console >> v7;
     
     console << "pair stored as: ";      console.write(my_pair,std::endl);
     console << "vector of ints: ";      console.write(v, std::endl);
     console << "set of doubles: ";      console.write(v2,std::endl);
     console << "set of ints: ";         console.write(v3,std::endl);
     console << "multiset of ints: ";    console.write(v4,std::endl);
     console << "list of Point2Ds: ";    console.write(v5,std::endl);
     console << "valarray of doubles: "; console.write(v6,std::endl);
     console << "array of Point2D: ";    console.write(v7,std::endl);
     
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
    file<file_type::console>& file<file_type::console>::write(T&& t, manip1 fp) {
        std::cout << std::forward<T>(t);
        std::cout << fp;
        return *this;
    };
    
    /** Loads in next value of type T from stream
     @tparam T must have operator>>(istream&, T&) defined
     @param t is an object reference
     @returns false if file is no longer in valid state
     @code
     
     // Program to generate rectangular version of Hilbert matrix
     #include "std_cin.h"
     #include "std_cout.h"
     #include "file.h"
     
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
     
     dsl::file<dsl::file_type::console> console;
     
     std::pair<int, int> my_pair;
     std::vector<int> v;
     std::set<double> v2;
     std::set<int> v3;
     std::multiset<int> v4;
     std::list<Point2D> v5;
     std::valarray<double> v6;
     std::array<Point2D,4> v7;
     
     console << "Input values in the same format as the default dsl output: \n";
     console << "Insert pair values: ";          console.read(my_pair);
     console << "Insert vector of ints: ";       console.read(v);
     console << "Insert set of doubles: ";       console.read(v2);
     console << "Insert set of ints: ";          console.read(v3);
     console << "Insert multiset of ints: ";     console.read(v4);
     console << "Insert list of Point2Ds: ";     console.read(v5);
     console << "Insert valarray of doubles: ";  console.read(v6);
     console << "Insert array of 4 Point2Ds: ";  console.read(v7);
     
     console << "pair stored as: ";      console.write(my_pair,std::endl);
     console << "vector of ints: ";      console.write(v, std::endl);
     console << "set of doubles: ";      console.write(v2,std::endl);
     console << "set of ints: ";         console.write(v3,std::endl);
     console << "multiset of ints: ";    console.write(v4,std::endl);
     console << "list of Point2Ds: ";    console.write(v5,std::endl);
     console << "valarray of doubles: "; console.write(v6,std::endl);
     console << "array of Point2D: ";    console.write(v7,std::endl);
     
     return 0;
     }
     @endcode
     Should produce output like
     @code
     Input values in the same format as the default dsl output:
     Insert pair values: {1,2}
     Insert vector of ints: {5,4,2,1}
     Insert set of doubles: {1.1,1.1,2.2,5.4,6.5,3.14159}
     Insert set of ints: {1,1,1,1,2,2,2,3,3,4,5}
     Insert multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
     Insert list of Point2Ds: {(0,0),(-1.1,1),(-2,2)}
     Insert valarray of doubles: {1.1,1.2,1.3,1.4,1.5}
     Insert array of 4 Point2Ds: {(0,0),(1,1),(2,2),(3,3)}
     pair stored as: {1,2}
     vector of ints: {5,4,2,1}
     set of doubles: {1.1,2.2,3.14159,5.4,6.5}
     set of ints: {1,2,3,4,5}
     multiset of ints: {1,1,1,1,2,2,2,3,3,4,5}
     list of Point2Ds: {(0,0),(-1.1,1),(-2,2)}
     valarray of doubles: {1.1,1.2,1.3,1.4,1.5}
     array of Point2D: {(0,0),(1,1),(2,2),(3,3)}
     @endcode
     */
    template<typename T>
    file<file_type::console>& file<file_type::console>::read(T& t) {
        std::cin >> t;
        return *this;
    };
    
    /** Output for manipulators
     @param fp is an argument like std::endl
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     #include <iomanip>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     dsl::file< dsl::file_type::console > console;
     
     console << std::setprecision(20);
     
     console << 3.1415926535897932384626433832 << std::endl;
     
     console << std::setw(20) << std::left;
     
     console << 3 << std::setw(20) << 12 << std::setw(10) << 4 << std::setw(5) << 9987;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     3.141592653589793116
     3                   12                  4         9987
     @endcode
     */
    file<file_type::console>& file<file_type::console>::operator<<( manip1 fp ) {
        std::cout << fp;
        return *this;
    }
    
    /** Output for manipulators
     @param fp is an argument which manipulates the underlying file stream
     @returns a reference to the file object for chaining
     */
    file<file_type::console>& file<file_type::console>::operator<<( manip2 fp ) {
        std::cout << fp;
        return *this;
    }
    
    /** Output for manipulators
     @param fp is an argument which manipulates the underlying file stream
     @returns a reference to the file object for chaining
     @code
     #include "desalvo/std_cout.h"
     #include "desalvo/file.h"
     #include <iomanip>
     
     namespace dsl = desalvo_standard_library;
     
     int main(int argc, const char * argv[]) {
     
     dsl::file< dsl::file_type::console > console;
     
     console << std::setprecision(20);
     
     console << 3.1415926535897932384626433832 << std::endl;
     
     console << std::setw(20) << std::left;
     
     console << 3 << std::setw(20) << 12 << std::setw(10) << 4 << std::setw(5) << 9987;
     
     return 0;
     }
     @endcode
     Should produce output
     @code
     3.141592653589793116
     3                   12                  4         9987
     @endcode
     */
    file<file_type::console>& file<file_type::console>::operator<<( manip3 fp ) {
        std::cout << fp;
        return *this;
    }
    
    
    
}



#endif
