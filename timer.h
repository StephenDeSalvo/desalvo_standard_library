//
//  timer.h
//  DeSalvo
//
//  Created by Stephen DeSalvo on 01/29/19.
//  Copyright (c) 2019 Stephen DeSalvo. All rights reserved.
//

#ifndef DeSalvo_Standard_Library_timer_h
#define DeSalvo_Standard_Library_timer_h

#include <ctime>
#include <random>
#include <deque>

namespace desalvo_standard_library {


    /** @class timer
        @brief A class for timers for triggering events.
     
        @tparam Distribution is the probability distribution to draw from, should be positive, real-valued.
        @tparam URNG is the standard uniform random number generator type
    */
    template<typename Distribution=std::exponential_distribution<double>, typename URNG=std::default_random_engine>
    class timer {
    public:

        /** Constructs a timer, by default at 0 with standard Exponential distribution with a cache of size 10.
            @param initial_value is the starting time for the clock.
            @param distribution is the probability distribution to draw from, should be positive, real-valued.
            @param generator is the random engine generator.
            @param initial_cache_size is the size of the cache to store.
        */
        timer(double initial_value=0, Distribution distribution=Distribution(), URNG generator=URNG(), size_t initial_cache_size=10) : cache(initial_cache_size), cache_size(initial_cache_size), time_zero(initial_value) { 

            //generator.seed(initial_seed);
            if (cache_size != 0){
                cache[0] = time_zero + distribution(generator);
                for (size_t i=1;i<cache.size();++i)
                    cache[i] = cache[i-1] + distribution(generator);
            }

        }

        /** If you want to seed the generator, you can!
            @param seed is the seed.
        */
        void seed(unsigned int seed) { generator.seed(seed); }

        /** If the cache has values, returns the next entry.  
            Otherwise it regeneates the cache and returns the next occurrence of the timer.
            @returns the next occurrence of the timer
        */        
        double next_occurrence() {
    
            // If cache is populated, just use it
            double result = 0;
            if (cache.size() > 0) {
                result = cache[0];
                time_zero = result;
                cache.pop_front();
            }
            else { // Generate a value to return and then repopulate the cache.
                result = time_zero + distribution(generator);
                time_zero = result;
                cache.push_back(time_zero + distribution(generator));
                for (size_t i=0;i<cache_size;++i)
                    cache.push_back(cache[i-1] + distribution(generator));
            }

            //std::cout << result << std::endl;
            return result;
        }

    protected:
        /** Stores cache of values so that no need to invoke distribution every time. */
        std::deque<double> cache;
        size_t cache_size;
        double time_zero;
        Distribution distribution;
        URNG generator;
    };



}


#endif
