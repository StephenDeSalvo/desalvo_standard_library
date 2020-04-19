//
//  combinatorics.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 9/24/15.
//  Copyright (c) 2015 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_combinatorics_h
#define DeSalvo_Standard_Library_combinatorics_h

#include "../Toolbox/pybind11-stable/include/pybind11/pybind11.h"

namespace py = pybind11;

#include "sequence.h"
#include "numerical.h"
#include <algorithm>

//namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    int add(int a, int b){
        return a+b;
    }
    
    PYBIND11_MODULE(example, m) {
        m.doc() = "pybind11 example plugin"; // optional module docstring

        m.def("add", &add, "A function which adds two numbers");
    }
    
//}




#endif
