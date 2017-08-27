//
//  combinatorial_structure.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 12/11/16.
//  Copyright © 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef combinatorial_structure_h
#define combinatorial_structure_h
#include <iostream>
#include <vector>    // only used for default type for statistics


template<typename Derived, typename ObjectType, typename ComponentType, typename Collection=std::vector<ComponentType> >
class combinatorial_structure {
public:
    
    
    class object : public ObjectType {
        
        
    private:
        
    };
    
    class generator;
    
    class iterator_random_access;
    
    class iterator_bidirectional;
    
    class iterator_forward;
    
private:
    
    
};









#endif /* combinatorial_structure_h */
