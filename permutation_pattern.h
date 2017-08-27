//
//  permutation_pattern.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 4/12/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_permutation_pattern_h
#define DeSalvo_Standard_Library_permutation_pattern_h

#include <algorithm>

namespace desalvo_standard_library {
    
    
    // function to find consecutive patterns in a permutation
    template<typename V = std::vector<int>, typename InputIterator = std::vector<int>::iterator >
    V reduction(InputIterator start, InputIterator stop) {
        
        V v(stop - start);
        
        std::copy( start, stop, std::begin(v) );
        std::sort( std::begin(v), std::end(v) );
        
        
    }
    
    
    
    
}



#endif
