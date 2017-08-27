//
//  set_partition.h
//  DeSalvo Standard Library
//
//  Created by Stephen DeSalvo on 12/14/16.
//  Copyright © 2016 Stephen DeSalvo. All rights reserved.
//

#ifndef set_partition_h
#define set_partition_h


#include <map>
#include "statistics.h"


namespace desalvo_standard_library {

template<typename KeyType=unsigned long long int, typename ValueType=unsigned long long int, typename WorkingPrecision=long double>
class set_partition {
public:
    
    template<typename Parameters = std::vector<int> >
    class generator;
        
    
    template<typename Parameters = std::vector<int> >
    class object {
        
        friend class generator<Parameters>;
        
    public:
        
        object(ValueType initial_size) : n(initial_size) {Z[n]=1;}
        
        typename std::map<KeyType,ValueType>::iterator begin() { return Z.begin(); }
        typename std::map<KeyType,ValueType>::iterator end() { return Z.end(); }
        
        class iterator_random_access;
        class iterator_bidirectional;
        class iterator_forward;
        
        ValueType weight() { return n; };
        
        ValueType number_of_components() {
            
            ValueType total = 0;
            
            for(auto& x : Z)
                total += x.second;
            
            return total;
        };

        
    private:
        ValueType n;
        std::map<KeyType,ValueType> Z;
    };
    
    template<typename Parameters>
    class generator {
    public:
        
        generator(ValueType initial_n) : n(initial_n) { }
        
        template<typename URNG>
        object<Parameters> operator()(SimulationMethod method = SimulationMethod::PDCDSH, URNG& generator = dsl::generator_64) {
            
            switch(method) {
                case SimulationMethod::BruteForce: return Brute_Force(n); break;
                case SimulationMethod::Boltzmann: return Boltzmann_set_partition(n, solve_for_x(n)); break;
                case SimulationMethod::BoltzmannExact: return exact_Boltzmann_set_partition(n); break;
                case SimulationMethod::PDCDSH: return PDC_DSH_set_partition(n); break;
                default: return PDC_DSH_set_partition(n);
            }
            
        }
    private:
        
        // More accurate solver for x, probably not needed, suffers from numerical instability.
        long double solve_for_x(long double n, long double eps=.0000001) {
            
            long double x = 1.;
            
            //long double xi = x;
            //long double ifactorial = 1.;
            
            long double numerator = 0.;
            long double denominator = 0.;
            long double factor = 1.;
            
            while(std::fabs(numerator-n)>=eps) {
                
                numerator = 0.;
                denominator = 0.;
                //xi = x;
                //ifactorial = 1.;
                
                factor = x;
                
                for(unsigned int i=1;i<n;++i) {
                    numerator += i*factor;
                    denominator += i*i*factor;
                    
                    factor *= x/(long double)(i+1);
                    //xi *= x;
                    //ifactorial *= (i+1);
                }
                denominator /= x;
                
                x -= (numerator+n*factor - n)/denominator;
                //std::cout << "error = " << numerator+n*xi/ifactorial-n << std::endl;
                //std::cout << "x = " << x << std::endl;
            }
            
            return x;
        }
        
        
        // 1.  Asymptotic expansion for solution of x*e^x = n, see deBruijn.
        long double approximate_x(long double n) {
            return log(n)-log(log(n))+log(log(n))/log(n); // + 0.5*std::pow(log(log(n)),2)/std::pow(log(n),2);
        }
        
        object<Parameters> Brute_Force(ValueType fixed_size) {
            return exact_Boltzmann_set_partition(fixed_size);
        }
        
        // 2.  The original Boltzmann sampler written as (Z_1, Z_2, ... , Z_n) where Z_i is Poi( x^i / i!)
        object<Parameters> Boltzmann_set_partition(ValueType fixed_size, long double x) {
            
            ValueType& n = fixed_size; // Create alias for size;
            
            // Z_1, Z_2, ..., Z_n are component counts.  Z[i] is number of components of size i
            // Z_0 will store the weight, i.e., sum_i i*Z[i], for each checking of exact sampling condition.
            //std::map<unsigned int, unsigned long long int> Z;
            object<Parameters> sp(n);
            sp.Z.clear();
            
            // generator_64 is a 64-bit random number generator
            static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
            
            // Error/Numerical tolerances, completely arbitrary
            long double eps = .000001;
            long double lambda = x;
            
            unsigned long long int total = 0;
            
            // Boltzmann sampling, sample Poisson.  Can be encoded more efficiently using Poisson process.
            for(unsigned int i=1; i <= n && lambda >= eps; ++i) {
                //for(unsigned int i=1; i <= n && x_to_i >=eps && ifactorial < FACT_MAX; ++i) {
                
                // Initialize Poisson distribution
                std::poisson_distribution<unsigned long long int> p( lambda );
                
                // Generate random component counts using Poisson (x^i / i!)
                unsigned long long int count = p(generator_64);
                
                // Since most counts above a certain threshold are zero, only store those which are positive.
                if(count) {
                    sp.Z[i] = count;
                    
                    // Calculate running total
                    total += i*sp.Z[i];
                }
                
                // Update values of x^i and i! as we go.
                // x^i/i! = x^(i-1)/(i-1)!  * x / i
                lambda *= x / (long double)(i+1);
                
            }
            
            // Keep track of weighted total in Z[0], i.e., index 0.
            sp.n = total;
            
            
            return sp;
        }
        
        // 3.  Exact Boltzmann sampler for set partitions
        object<Parameters> exact_Boltzmann_set_partition(ValueType fixed_size) {
            
            ValueType& n = fixed_size;
            // Z_1, Z_2, ..., Z_n are component counts.  Z[i] is number of components of size i.
            // Map is efficient because we only expect about e*log(n) distinct component sizes, with almost no gaps.
            //std::map<unsigned int, unsigned long long int> Z;
            object<Parameters> sp(n);
            //sp.Z.clear();
            
            // x e^x = n implies x is approximately log(n) - log(log(n)) + log(log(n))/log(n) + O( (log(log(n))/log(n))^2 ), see deBruijn.
            long double x = approximate_x(n);
            //std::cout << x << std::endl;
            
            // generator_64 is a 64-bit random number generator
            static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
            
            size_t count = 0;
            
            do {
                ++count;
                sp = Boltzmann_set_partition(fixed_size,x);
                
            } while(sp.n != n);
            
            // Put in the number of rejections, which is an interesting statistic, at UINT_MAX.  Note that -1 gets converted to UINT_MAX because the index type is an unsigned int.
            //Z[-1] = count;
            
            return sp;
        }
        
        
        // 4.  PDC deterministic second half algorithm.
        object<Parameters> PDC_DSH_set_partition(ValueType fixed_size) {
            
            ValueType& n = fixed_size;
            
            // Z_1, Z_2, ..., Z_n are component counts.  Z[i] is number of components of size i.
            // Map is efficient because we only expect about e*log(n) distinct component sizes, with almost no gaps.
            //std::map<unsigned int, unsigned long long int> Z;
            object<Parameters> sp(n);
            //sp.Z.clear();
            
            // x e^x = n implies x is approximately log(n) - log(log(n)) + log(log(n))/log(n) + O( (log(log(n))/log(n))^2 ), see deBruijn.
            long double x = approximate_x(n);
            
            // generator_64 is a 64-bit random number generator
            static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
            
            std::uniform_real_distribution<long double> unif(0.,1.);
            long double rejection = 0.;
            
            size_t count = 0;
            
            unsigned int PDC_index = std::round(x);
            //std::cout << "PDC_index = " << PDC_index << std::endl;
            
            // Calculate the expected value of the distribution, also serves as the maximum point probability index
            long double lambdax = 1.;
            for(long double i=1.;i<=PDC_index;++i)
                lambdax *= (long double)(x)/i;
            
            //std::cout << "lambda = " << lambdax << std::endl;
            
            do {
                ++count;
                
                // Just run the whole Boltzmann sample, including the PDC index
                sp = Boltzmann_set_partition(fixed_size,x);
                
                // take out the PDC index from the total
                sp.n -= PDC_index*sp.Z[PDC_index];
                sp.Z[PDC_index] = 0;
                
                rejection = 0.;
                
                // If completable ...
                if(sp.n <= n && (n-sp.n)%PDC_index == 0) {
                    
                    // Find residual value that needs to survive rejection probability
                    unsigned int k = (unsigned int)(n-sp.n)/PDC_index;
                    
                    // Calculate rejection probability
                    rejection = 1.;
                    
                    if(std::round(lambdax) < k) {
                        for(long double i=k;i>=std::round(lambdax)+1;--i)
                            rejection *= (long double)(lambdax) / i;
                        
                    }
                    else if(std::round(lambdax) > k) {
                        for(long double i=std::round(lambdax); i >= k+1; --i)
                            rejection *= i / (long double)(lambdax);
                    }
                    
                    //std::cout << "rejection = " << rejection << std::endl;
                    
                    
                }
            } while(sp.n > n || unif(generator_64) >= rejection);
            
            // Put in the multiplicity which survived rejection
            sp.Z[PDC_index] = (n-sp.n)/PDC_index;
            
            // Update weighted total
            sp.n += PDC_index*sp.Z[PDC_index];
            
            // Add number of rejections to the structure
            //Z[-1] = count;
            
            return sp;
        }

        
        
        unsigned long long int n;
    };
    
    template<typename Parameters=std::vector<int>, typename URNG=std::mt19937_64>
    object<Parameters> random(ValueType n, URNG& g = generator_64)  {
        
        generator<Parameters> gen({n});
        
        return gen(SimulationMethod::PDCDSH,g);
        
    }

    
};

    
    template<typename ValueType = unsigned int, typename WorkingPrecision=unsigned long int, typename Parameters=std::vector<int>>
    typename set_partition<ValueType,WorkingPrecision>::template object<Parameters> random_set_partition(ValueType n)  {
        
        set_partition<ValueType,WorkingPrecision> sp;
        
        return sp.random(n);
        
    }

























#if 0
/** @file set_partition_sampling.cpp
 @author Stephen DeSalvo
 @date December 13, 2016.
 
 Code written in response to email sent by Aimene Belfodil.  Consists of only a few routines.
 
 1.  Approximate solution to x*e^x = n, tilting parameter in Boltzmann sampling of set partitions
 2.  The original Boltzmann sampling using independent Poisson random variables.
 3.  Exact Boltzmann sampling by simple rejection until target SUM_i i*Z_i = n is met.
 4.  Probabilistic divide-and-conquer algorithm deterministic second half, should have O(log(n)^(5/4)) rejection rate for an exact sample.
 (5. I did not include the more recent hybrid recursive method with Boltzmann sampling which would get the rejection down to O(1), because that requires O(log(n)) memory and would be a bit more complicated, but not too much more.)
 */

#include <iostream>
#include <map>
#include <cmath>
#include <random>





// 2.  The original Boltzmann sampler written as (Z_1, Z_2, ... , Z_n) where Z_i is Poi( x^i / i!)
std::map<unsigned int, unsigned long long int> Boltzmann_set_partition(unsigned int fixed_size, long double x) {
    
    const size_t& n = fixed_size; // Create alias for size;
    
    // Z_1, Z_2, ..., Z_n are component counts.  Z[i] is number of components of size i
    // Z_0 will store the weight, i.e., sum_i i*Z[i], for each checking of exact sampling condition.
    std::map<unsigned int, unsigned long long int> Z;
    
    // generator_64 is a 64-bit random number generator
    static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    
    // Error/Numerical tolerances, completely arbitrary
    long double eps = .000001;
    long double lambda = x;
    
    unsigned long long int total = 0;
    
    // Boltzmann sampling, sample Poisson.  Can be encoded more efficiently using Poisson process.
    for(unsigned int i=1; i <= n && lambda >= eps; ++i) {
        //for(unsigned int i=1; i <= n && x_to_i >=eps && ifactorial < FACT_MAX; ++i) {
        
        // Initialize Poisson distribution
        std::poisson_distribution<unsigned long long int> p( lambda );
        
        // Generate random component counts using Poisson (x^i / i!)
        unsigned long long int count = p(generator_64);
        
        // Since most counts above a certain threshold are zero, only store those which are positive.
        if(count) {
            Z[i] = count;
            
            // Calculate running total
            total += i*Z[i];
        }
        
        // Update values of x^i and i! as we go.
        // x^i/i! = x^(i-1)/(i-1)!  * x / i
        lambda *= x / (long double)(i+1);
        
    }
    
    // Keep track of weighted total in Z[0], i.e., index 0.
    Z[0] = total;
    
    
    return Z;
}

// 3.  Exact Boltzmann sampler for set partitions
std::map<unsigned int, unsigned long long int> exact_Boltzmann_set_partition(unsigned int fixed_size) {
    
    unsigned int& n = fixed_size;
    // Z_1, Z_2, ..., Z_n are component counts.  Z[i] is number of components of size i.
    // Map is efficient because we only expect about e*log(n) distinct component sizes, with almost no gaps.
    std::map<unsigned int, unsigned long long int> Z;
    
    // x e^x = n implies x is approximately log(n) - log(log(n)) + log(log(n))/log(n) + O( (log(log(n))/log(n))^2 ), see deBruijn.
    long double x = approximate_x(n);
    //std::cout << x << std::endl;
    
    // generator_64 is a 64-bit random number generator
    static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    
    size_t count = 0;
    
    do {
        ++count;
        Z = Boltzmann_set_partition(fixed_size,x);
        
    } while(Z[0] != n);
    
    // Put in the number of rejections, which is an interesting statistic, at UINT_MAX.  Note that -1 gets converted to UINT_MAX because the index type is an unsigned int.
    Z[-1] = count;
    
    return Z;
}


// 4.  PDC deterministic second half algorithm.
std::map<unsigned int, unsigned long long int> PDC_DSH_set_partition(unsigned int fixed_size) {
    
    unsigned int& n = fixed_size;
    
    // Z_1, Z_2, ..., Z_n are component counts.  Z[i] is number of components of size i.
    // Map is efficient because we only expect about e*log(n) distinct component sizes, with almost no gaps.
    std::map<unsigned int, unsigned long long int> Z;
    
    // x e^x = n implies x is approximately log(n) - log(log(n)) + log(log(n))/log(n) + O( (log(log(n))/log(n))^2 ), see deBruijn.
    long double x = approximate_x(n);
    
    // generator_64 is a 64-bit random number generator
    static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    
    std::uniform_real_distribution<long double> unif(0.,1.);
    long double rejection = 0.;
    
    size_t count = 0;
    
    unsigned int PDC_index = std::round(x);
    //std::cout << "PDC_index = " << PDC_index << std::endl;
    
    // Calculate the expected value of the distribution, also serves as the maximum point probability index
    long double lambdax = 1.;
    for(long double i=1.;i<=PDC_index;++i)
        lambdax *= (long double)(x)/i;
    
    //std::cout << "lambda = " << lambdax << std::endl;
    
    do {
        ++count;
        
        // Just run the whole Boltzmann sample, including the PDC index
        Z = Boltzmann_set_partition(fixed_size,x);
        
        // take out the PDC index from the total
        Z[0] -= PDC_index*Z[PDC_index];
        Z[PDC_index] = 0;
        
        rejection = 0.;
        
        // If completable ...
        if(Z[0] <= n && (n-Z[0])%PDC_index == 0) {
            
            // Find residual value that needs to survive rejection probability
            unsigned int k = (unsigned int)(n-Z[0])/PDC_index;
            
            // Calculate rejection probability
            rejection = 1.;
            
            if(std::round(lambdax) < k) {
                for(long double i=k;i>=std::round(lambdax)+1;--i)
                    rejection *= (long double)(lambdax) / i;
                
            }
            else if(std::round(lambdax) > k) {
                for(long double i=std::round(lambdax); i >= k+1; --i)
                    rejection *= i / (long double)(lambdax);
            }
            
            //std::cout << "rejection = " << rejection << std::endl;
            
            
        }
    } while(Z[0] > n || unif(generator_64) >= rejection);
    
    // Put in the multiplicity which survived rejection
    Z[PDC_index] = (n-Z[0])/PDC_index;
    
    // Update weighted total
    Z[0] += PDC_index*Z[PDC_index];
    
    // Add number of rejections to the structure
    Z[-1] = count;
    
    return Z;
}

int main(int argc, const char * argv[]) {
    
    // Note from Stephen DeSalvo to Aimene Belfodil:
    // Before you go crazy, note that there is some numerical instability around 10^7 or 10^8 in the PDC algorithm.
    // It is possible that I am just being careless in a few places, but I also suspect it is C++'s Poisson generator or
    // numerical error.  Supposing these could be fixed I imagine this algorithm could be run on as large a set partition as
    // desired, since the rejection rate and storage costs both grow logarithmically using PDC deterministic second half.
    
    // Also note I am not generating a random set partition in set partition form, simply the number of blocks of sizes 1, 2, etc.
    // One would still need to perform a random permutation of [n]={1,2,...,n} and fill them in.
    
    // size of set partition, also call it n
    const unsigned int  fixed_size = 10000000;
    const unsigned int& n = fixed_size;    // Create alias for size;
    
    // uncomment exactly one of these three lines below
    //auto Z = Boltzmann_set_partition(n, approximate_x(n));
    //auto Z = exact_Boltzmann_set_partition(n);
    auto Z = PDC_DSH_set_partition(n);
    
    for(auto& x : Z) std::cout << "(" << x.first << "," << x.second << ")" << std::endl;
    
    // Recall:
    // Z[0] stores the weight
    // Z[UINT_MAX] stores the number of rejections.
    
    return 0;
}

#endif




}


#endif /* set_partition_h */
