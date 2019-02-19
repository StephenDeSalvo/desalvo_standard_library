//
//  integer_partition.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 3/18/16.
//  Copyright (c) 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_integer_partition_h
#define DeSalvo_Standard_Library_integer_partition_h

#include <iostream>
#include <map>
#include <cmath>
#include <random>

#include "table.h"
#include "combinatorial_structure.h"

namespace desalvo_standard_library {

    
    template<typename UnsignedInteger, typename Floating=long double>
    class integer_partition_generator;
    
    
    template<typename UnsignedInteger>
    class integer_partition {
        friend class integer_partition_generator<UnsignedInteger>;
    public:
        
        /** Initialize partition to 1 part of size n
         @param n is the size of the partition
         */
        integer_partition(UnsignedInteger n) { parts_of_size[n] = 1; }
        
        /** By default everything is empty */
        integer_partition() { }
        
        /** Clear the partition to be empty */
        void clear() { parts_of_size.clear(); }
        
        void print(std::ostream& out=std::cout) {
            for(auto& x : parts_of_size) {
                for(UnsignedInteger i=0;i<x.second; ++i)
                    out << x.first<<",";
            }
            return;
        }
        
        UnsignedInteger weight() {
            UnsignedInteger weight = 0;
            for(auto& x : parts_of_size)
                weight += x.first*x.second;
            
            return weight;
        }
        
    private:
        std::map<UnsignedInteger, UnsignedInteger> parts_of_size;
        
    };
    

    
    template<typename UnsignedInteger, typename Floating>
    class integer_partition_generator {
    public:
        integer_partition_generator(UnsignedInteger input_n) : c(3.14159/sqrt(6)), n(input_n), unif(0,1) {
            x = exp(-c/sqrt(n));
        }
        
        
        template<typename URNG>
        integer_partition<UnsignedInteger> Boltzmann_sampler(URNG&& generator) {
            
            integer_partition<UnsignedInteger> ip;
            
            Floating logx = -c/sqrt(n);
            
            for(UnsignedInteger i=1;i<n;++i) {
                ip.parts_of_size[i] = log(unif(generator))/(i*logx);
            }
            
            return ip;
        }
        
        
        template<typename URNG>
        integer_partition<UnsignedInteger> exact_Boltzmann_sampler(URNG&& generator) {
            
            integer_partition<UnsignedInteger> ip;
            
            while(ip.weight() != n) {
                ip = Boltzmann_sampler(generator);
            }
            
            return ip;
        }
        

        dsl::table<double> recursive_method_table(UnsignedInteger n, UnsignedInteger k) {

            dsl::table<double> t(k+1,n+1);

            for(auto& xx : t.row(0))
                xx = 0;

            int inc = 0;
            for(auto& xx : t.column(0))
                xx = pow(x,inc++);
                //xx = 1;
            
            
            for(size_t kk=1;kk<=k;++kk) {
                
                for(size_t nn=1;nn<kk;++nn)
//                    t(kk,nn) = t(nn,nn);
                    t(kk,nn) = t(nn,nn)*pow(x,nn);


                for(size_t nn=kk;nn<=n;++nn)
//                    t(kk,nn) = (t(kk,nn-kk) + t(kk-1,nn));
                    t(kk,nn) = (t(kk,nn-kk) + t(kk-1,nn))*pow(x,nn);

            }
            
            return t;
            
        }
        
        
        
    private:
        double c;
        double x;
        UnsignedInteger n;
        std::uniform_real_distribution<Floating> unif;
    };
    
    
    
}




#endif
