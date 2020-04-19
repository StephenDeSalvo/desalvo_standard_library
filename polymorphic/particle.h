//
//  particle.h
//  DeSalvo
//
//  Created by Stephen DeSalvo on 01/29/19.
//  Copyright (c) 2019 Stephen DeSalvo. All rights reserved.
//


#ifndef DeSalvoParticle_h
#define DeSalvoParticle_h



#include <iostream>
#include <vector>
#include <random>
//#include "graph.h"
#include "../timer.h"
#include "../std_cout.h"

namespace desalvo_standard_library {


    namespace polymorphic {
        /** @class Particle
            @brief Creates an object representing a classical notion of a particle on a graph.

            @tparam Universe is a Graph
            @tparam Position is an object which is a Node the Graph.
            @tparam URNG is the standard uniform random number generator type
            @tparam Timer is the internal clock on the particles.

        */
        template<typename Universe, typename Position, typename Timer=timer<>, typename URNG=std::default_random_engine >
        class Particle {
        public:

            Particle() = delete;
            Particle(Particle&) = delete;

            /** No resources itself, just the standard virtual destructor. */
            virtual ~Particle() = default;


            /** Construct a particle in the universe
                @param initial_universe is the universe the particle lives on.
                @param initial_position is the initial location of the particle in the universe.
                @param initial_generator is the random number generator.
            */
            Particle(Universe* const initial_universe, const Position* initial_position, URNG& initial_generator) : universe(initial_universe), position(initial_position), generator(initial_generator) { };


            /** Show the position of the particle.
                @param out is a stream object, by default std::cout
            */
            virtual void print(std::ostream& out = std::cout) {
                position->print(out);
            }

            /** Gets the jump times of the particle up until time epoch time.
                @param time is the time epoch to stop getting jump times.
                @returns a vector of jump times from 0 to time.
            */
            std::vector<double> jump_times(double time) {
                double current_time = 0;
                std::vector<double> jumps;

                while (current_time <= time) {
                    current_time += timer.next_occurrence();
                    if (current_time <= time)
                        jumps.push_back(current_time);
                }

                return jumps;

            }


            /** Updates the position of the particle by proposing a jump to a neighboring state at random according to 
                weights and then making the transition only if the exclusion function is satisfied.
                @param generator is the random number generator
                @param particles is the vector of all particles in the universe
                @param exclusion_function is the function to call which determines whether the particle can be updated to the proposed position.
                @tparam Function is me being lazy, it is the function signature of exclusion_function.  (The compiler will complain if you don't get it right anyway.)
            */
            // template<typename Function>
            // bool update(URNG& generator, std::vector<Particle*>& particles, Function exclusion_function) {
            template<typename Vector_Particles, typename Function>
            bool update(URNG& generator, Vector_Particles& particles, Function exclusion_function) {


                //position->print(std::cout); std::cout << std::endl;
                //std::pair<std::vector<const Position*>, std::vector<double>> neighbors_and_weights = universe->neighbors_and_weights(position, true);
                auto neighbors_and_weights = universe->neighbors_and_weights(position, true);

                std::vector<double>& transition_probabilities = neighbors_and_weights.second;

                //std::cout << transition_probabilities << std::endl;

                double norm = 0.;
                for (auto& x : transition_probabilities)
                    norm += x;

                for (auto& x : transition_probabilities)
                    x /= norm;

                //std::cout << transition_probabilities << std::endl;

                // Initialize the random generator and distribution
                //std::default_random_engine generator(seed);
                std::discrete_distribution<double> discrete(std::begin(transition_probabilities), std::end(transition_probabilities));

                //position->print(std::cout);

                auto index = discrete(generator);
                const Position* changed = position;

                // std::cout << position << " --> ";
                // std::cout << changed << std::endl;

                const Position* proposed_position = dynamic_cast<const Position*>((neighbors_and_weights.first)[index]);


                bool block = exclusion_function(proposed_position, particles);

                if (!block)
                    position = proposed_position;

                // changed->print(std::cout);
                // std::cout << " --> ";
                // position->print(std::cout);
                // std::cout << std::endl;


                if (changed != position)
                    return true;
                else
                    return false;

            }

            /** Gets the position of the particle
                @returns the position of the particle.
            */
            const Position* const get_position() const { return position; }


        private:
            Universe* const universe;
            const Position* position;
            Timer timer;
            URNG generator;
        };

    }
}


#endif