//
//  sampling.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 3/27/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_sampling_h
#define DeSalvo_Standard_Library_sampling_h

#include <tuple>

namespace desalvo_standard_library {
    
    template<typename T1, typename...Args>
    class sequence_parameters {
    public:
        sequence_parameters(T1 t, Args... args) {
            parameters = std::make_tuple<T1,Args...>(t,args...);
        }
        
        sequence_parameters() { }
        
        
        void replace_with(std::tuple<T1,Args...>&& params) {
            parameters = params;
        }

        void replace_with(const std::tuple<T1,Args...>& params) {
            parameters = params;
        }
        
        template<typename T>
        T get(const size_t index) { return std::get<index>(parameters); };

    private:
        std::tuple<T1,Args...> parameters;
    };
    
    
    
}




#endif
