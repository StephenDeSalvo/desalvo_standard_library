//
//  integer_sequences.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 12/8/15.
//  Copyright (c) 2015 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_integer_sequences_h
#define DeSalvo_Standard_Library_integer_sequences_h




namespace desalvo_standard_library {

    
    template<typename Integer>
    Integer Euler(const Integer& n, const Integer& m) {
        
        // negative inputs return 0, n=0 always returns 0
        if(n <= 0 || m < 0) return 0;
        
        // n >= 1 and m == 0 returns 1
        if(m == 0) return 1;

        // A(n,n-1) = 1
        if(m == n-1) return 1;
        
        return (n-m)*Euler(n-1,m-1)+(m+1)*Euler(n-1,m);
        
    }
    
    
    
    
    
}



#endif
