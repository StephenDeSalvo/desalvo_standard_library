//
//  dsl_time.h
//  DeSalvo Standard Library
//
//  Named with the dsl_ prefix, like dsl_algorithm.h: a header called time.h
//  in the include path would shadow the system <time.h>.
//
//  Created by Stephen DeSalvo on 9/20/15.
//  Copyright (c) 2015 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_time_h
#define DeSalvo_Standard_Library_time_h

#include <ctime>

namespace desalvo_standard_library {


/** @class time
 @brief A class for keeping track of timings easily
 
 The idea is to do something like
 dsl::time tic;
 function();
 cout << "function() took "<< tic.toc() << " seconds "<<endl;
 
 The style is admittedly similar to the Matlab style, but this is because they worked out a decent system and I see no reason not to use the similar style, since many people would already be familiar with it.
 
 
 */
class time {
public:
    time();
    
    void reset();
    double toc();
    
private:
    clock_t t;
};

/** Default constructor, initializes clock cycles to current number */
inline time::time() : t(clock()) { };

/** Resets the clock cycles to current number in program */
inline void time::reset() { t = clock(); };

/** Calculates total time in seconds that has elapsed
 @returns number of seconds transpired since construction or last call to reset
 */
inline double time::toc() { return (double)(clock()-t)/CLOCKS_PER_SEC; };

}


#endif
