/**  @file statistics.h
    @Author Stephen DeSalvo
    @date July, 2015
    @brief Collection of functions and classes for random generation and statistics
 
 
  Example Usage:
  @code
     // Generate a distinct set of three elements from {1,2,...,10}
     partial_permutation(10,3);
  @endcode
 */

#ifndef DeSalvoRandomLibrary_h
#define DeSalvoRandomLibrary_h

#include <random>
#include <chrono>
#include <algorithm>
#include <iterator>
#include <valarray>
#include <complex>

#include "numerical.h" // external file in desalvo library
#include "file.h"


namespace desalvo_standard_library {
    namespace dsl = desalvo_standard_library;

    /** @typedef ull abbrev. for unsigned long long */
    typedef unsigned long long ull;
    
    /** @var generator_64 is a 64-bit random number generator */
    static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    
    /** @var generator_32 is a 32-bit random number generator */
    static std::mt19937 generator_32((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    
    enum class SimulationMethod { BruteForce, Boltzmann, BoltzmannExact, PDCDSH };
    
    
    
    /** @class RandomVariable
     @brief CRTP base class for random objects
     
     CRTP base class for objects that can be generated randomly.
     
     Requires:
     operator()(URNG& gen) overloaded for class Derived
     */
    template<typename T, typename Derived, typename URNG= std::mt19937_64>
    class random_variable {
    public:
        
        /** Generator for iid samples.  The container for output must be constructable using size_t input parameter.
         @param m is the number of samples
         @param gen is the random number generator, by default 64-bit
         @returns an iid sample stored in container of template type V=std::vector<T>.
         */
        template<typename V = std::vector<T> >
        V iid_sample(size_t m, URNG& gen = generator_64 ) {
            
            // Create variable of type Derived to obtain variate data
            Derived& variate = static_cast<Derived&>(*this);
            
            // V must have constructor accepting size_t parameter.
            V v(m);
            
            // Generate random variate for each element
            for(auto& x : v)   x = variate(gen);
            
            // RVO should kick in.
            return v;
        };
        
        
        /** Estimates the average using iid samples.  Return type F can be different from T
         @param m is the number of samples
         @param gen is the random number generator, by default 64-bit
         @returns an average of generated values by static_cast to element of type F
         */
        template<typename F = double>
        F estimate_mean(size_t m, URNG& gen = generator_64 ) {
            
            // CRTP
            Derived& variate = static_cast<Derived&>(*this);
            
            // 0 must be castable to type F
            F mean_estimate = static_cast<F>(0);
            
            // Take average by generating values one at a time, more numerically stable but performs
            // m divisions rather than one at the end.  This is to prevent overflow.
            for(size_t i=0;i<m;i++) mean_estimate += static_cast<F>(variate(gen))/m;
            
            return mean_estimate;
        }
        
        
    };
    
    /** @class DiscreteUniform
     @brief Uniform over set of elements {a,a+1,...,b-1,b}
     
     Inherits from RandomVariable so as long as operator()(URNG& gen) is overloaded to return a random value we can
     invoke its member functions.
     
     Store a lower and upper bound denoting the smallest and largest values capable of being generated.
     */
    template<typename ReturnType = int, typename ParameterType=ReturnType, typename URNG= std::mt19937_64>
    class discrete_uniform : public random_variable<ReturnType, discrete_uniform<ReturnType, ParameterType, URNG>, URNG> {
    public:
        
        /** Constructs a random variable for random values in {a,a+1,...,b-1,b}
         @param a is the lower bound
         @param b is the upper bound
         */
        discrete_uniform(ParameterType a, ParameterType b) : lower(a), upper(b) {  };
        
        /** Generates random value from distribution using the std distributions.
         @param gen is the random number generator, by default 64-bit.
         @returns random element
         */
        ReturnType operator()(URNG& gen = generator_64) {
            std::uniform_int_distribution<ReturnType> r(lower,upper);
            return r(gen);
        }
        
        /** Returns the expected value of the random variable
         ReturnType must have operator+(ReturnType, ReturnType) defined, and the return type of operator+ must be castable to F
         @returns (lower+upper)/2
         */
        template<typename F=double>
        F mean() {
            return (static_cast<F>(lower+upper))/2;
        }
        
        // TODO: Write more statistics properties like standard deviation, moments, kurtosis, etc.
        
    private:
        /** @var lower is the lower bound for random generation */
        ParameterType lower;
        /** @var upper is the upper bound for random generation */
        ParameterType upper;
    };
    
    
    /** @class real_uniform
     @brief Uniform over an interval [a,b]
     
     Inherits from RandomVariable so as long as operator()(URNG& gen) is overloaded to return a random value we can
     invoke its member functions.
     
     Store a lower and upper bound denoting the smallest and largest values capable of being generated.
     */
    template<typename ReturnType = double, typename ParameterType=ReturnType, typename URNG= std::mt19937_64>
    class real_uniform : public random_variable<ReturnType, real_uniform<ReturnType, ParameterType, URNG>, URNG> {
    public:
        
        /** Constructs a random variable for random values in {a,a+1,...,b-1,b}
         @param a is the lower bound
         @param b is the upper bound
         */
        real_uniform(ParameterType a, ParameterType b) : lower(a), upper(b) {  };
        
        /** Generates random value from distribution using the std distributions.
         @param gen is the random number generator, by default 64-bit.
         @returns random element
         */
        ReturnType operator()(URNG& gen = generator_64) {
            std::uniform_real_distribution<ReturnType> r(lower,upper);
            return r(gen);
        }
        
        /** Returns the expected value of the random variable
         ReturnType must have operator+(ReturnType, ReturnType) defined, and the return type of operator+ must be castable to F
         @returns (lower+upper)/2
         */
        template<typename F=double>
        F mean() {
            return (static_cast<F>(lower+upper))/2;
        }
        
    private:
        /** @var lower is the lower bound for random generation */
        ParameterType lower;
        /** @var upper is the upper bound for random generation */
        ParameterType upper;
    };
    
    
    
    /** @class rv_bernoulli
     @brief Bernoulli random variable in {0,1}
     
     Inherits from RandomVariable so as long as operator()(URNG& gen) is overloaded to return a random value we can
     invoke its member functions.
     
     Store a lower and upper bound denoting the smallest and largest values capable of being generated.
     */
    template<typename ReturnType = int, typename ParameterType=ReturnType, typename URNG= std::mt19937_64>
    class rv_bernoulli : public random_variable<ReturnType, rv_bernoulli<ReturnType, ParameterType, URNG>, URNG> {
    public:
        
        /** Constructs a random variable for random values in {a,a+1,...,b-1,b}
         @param a is the lower bound
         @param b is the upper bound
         */
        rv_bernoulli(ParameterType initial_p) : p(initial_p) {  };
        
        /** Generates random value from distribution using the std distributions.
         @param gen is the random number generator, by default 64-bit.
         @returns random element
         */
        ReturnType operator()(URNG& gen = generator_64) {
            std::uniform_real_distribution<long double> u(0.,1.);
            return u(gen) < p? 1 : 0;
        }
        
        /** Returns the expected value of the random variable
         ReturnType must have operator+(ReturnType, ReturnType) defined, and the return type of operator+ must be castable to F
         @returns (lower+upper)/2
         */
        template<typename F=double>
        F mean() {
            return static_cast<F>(p);
        }
        
        
        template<typename Range=int>
        ReturnType pdf(Range k) {
            if(k==0) return 1-p;
            if(k==1) return p;
            return 0;
        }
        
        template<typename Range=int>
        ReturnType cdf(Range k) {
            if(k < 0) return 0;
            if(k < 1) return 1-p;
            if(k >= 1) return p;
        }
        
        template<typename InputIterator, typename Container = std::vector<long double> >
        ReturnType pdf(InputIterator start, InputIterator stop) {
            
            Container v(stop-start);
            auto vit = v.begin();
            
            while(start != stop)
                *vit++ = pdf(*start++);
            
            return 0;
        }
        
        template<typename InputIterator, typename Container = std::vector<long double> >
        ReturnType cdf(InputIterator start, InputIterator stop) {
            
            Container v(stop-start);
            auto vit = v.begin();
            
            while(start != stop)
                *vit++ = cdf(*start++);
            
            return 0;
        }
        
        
    private:
        /** @var p is the success probability */
        ParameterType p;
    };
    
    /** @class rv_binomial
     @brief Binomial random variable in {0,1,...,n}
     
     Inherits from RandomVariable so as long as operator()(URNG& gen) is overloaded to return a random value we can
     invoke its member functions.
     
     Store a lower and upper bound denoting the smallest and largest values capable of being generated.
     */
    template<typename ReturnType = unsigned int, typename Parameter1Type=long double, typename Parameter2Type=ReturnType, typename URNG= std::mt19937_64>
    class rv_binomial : public random_variable<ReturnType, rv_binomial<ReturnType, Parameter1Type, Parameter2Type, URNG>, URNG> {
    public:
        
        /** Constructs a random variable for random values in {a,a+1,...,b-1,b}
         @param a is the lower bound
         @param b is the upper bound
         */
        rv_binomial(Parameter1Type initial_p, Parameter2Type initial_n) : p(initial_p), n(initial_n) {  };
        
        /** Generates random value from distribution using the std distributions.
         @param gen is the random number generator, by default 64-bit.
         @returns random element
         */
        ReturnType operator()(URNG& gen = generator_64) {
            std::uniform_real_distribution<long double> u(0.,1.);
            return u(gen) < p? 1 : 0;
        }
        
        /** Returns the expected value of the random variable
         ReturnType must have operator+(ReturnType, ReturnType) defined, and the return type of operator+ must be castable to F
         @returns (lower+upper)/2
         */
        template<typename F=double>
        F mean() {
            return static_cast<F>(p);
        }
        
    private:
        /** @var p is the success probability */
        Parameter1Type p;
        
        /** @var n is the number of trials */
        Parameter2Type n;
    };
    
    
    /** @class poisson_binomial
        @brief sum of bernoulli with different p_i probabilities
     
     @code
     std::vector<long double> p {.1,.2,.3};
     dsl::poisson_binomial<std::vector<long double>> pb(p);
     
     //    std::cout << pb.pdf(0) << "," << pb.pdf(1) << "," << pb.pdf(2) <<  "," << pb.pdf(3) << "," << pb.pdf(4) << std::endl;
     
     long double total = 0.;
     for(unsigned int i=0;i<=p.size();++i) {
     std::cout << pb.pdf(i) << ",";
     total +=pb.pdf(i);
     }
     
     std::cout <<"\nsum of all probabilities = " << total << std::endl;
    @endcode
     */
    template<typename Vector>
    class poisson_binomial {
    public:
        
        template<typename VectorType>
        poisson_binomial(VectorType initial_p) : p(initial_p) { };
        
        template<typename InputIterator>
        poisson_binomial(InputIterator start, InputIterator stop) {
            std::copy(start, stop, std::back_inserter(p));
        };
        
        template<typename Range, typename Density=long double>
        Density pdf_slower(Range kk) {
            
            //std::vector<long double> p {.2,.2,.2};
            auto n = p.size();
            
            if(kk > n || kk < 0) return 0.;
            
            
            std::complex<long double> c = std::exp(std::complex<long double>(0.,2.*3.14159265358)/std::complex<long double>(n+1.,0.));
            
            
            //std::cout << "c = " << c << std::endl;
            
            //for(unsigned int i=0;i<=n;++i) {
            //    std::cout << "c^"<<i<<" = " << std::pow(c,i) << std::endl;
            //}
            
            //for(unsigned int kk=0;kk<=n;++kk) {
                
                std::complex<long double> prob(0.,0.);
                std::complex<long double> k(kk,0.);
                
                std::complex<long double> product(1.,0);
                
                for(unsigned int i=0;i<=n;++i) {
                    
                    std::complex<long double> index(i);
                    
                    product = std::pow(c,-index*k);
                    //std::cout << "c^(-lk) = " << product << std::endl;
                    
                    //std::cout << "i = " << i << " pdf: c^(-lk) = " << product << std::endl;

                    
                    for(size_t j=0;j<n;++j){
                        auto term = std::complex<long double>(1.)+(std::pow(c,index)-std::complex<long double>(1.))*p[j];
                    //    std::cout << "product term = " << term << std::endl;
                        product *=term;
                    }
                    
                    prob += product;
                    
                }
                prob = prob / std::complex<long double>(n+1.);
                
                //std::cout << "prob["<<kk<<"] = " << prob << std::endl;
                
            //}
            return std::real(prob);
        }
        
        
        template<typename Range, typename Density=long double>
        Density pdf(Range kk) {
            
            //std::vector<long double> p {.2,.2,.2};
            auto n = p.size();
            
            if(kk > n || kk < 0) return 0.;
            
            
            //std::complex<long double> c = std::exp(std::complex<long double>(0.,2.*3.14159265358)/std::complex<long double>(n+1.,0.));
            std::complex<long double> c = std::exp(std::complex<long double>(0.,2.*3.14159265358/(n+1.)));
            
            
            std::complex<long double> one(1.);
            std::complex<long double> prob(0.,0.);
            std::complex<long double> k(kk,0.);
            
            //std::complex<long double> cinv = one/c;
            
            std::complex<long double> c_ell(one);
            
            std::complex<long double> c_ell_k = std::pow(c, -k);
            std::complex<long double> c_inv_lk = one;
            
            
            std::complex<long double> product(1.,0);
            
            
            for(unsigned int i=0;i<=n;++i) {
                
                std::complex<long double> index(i);
                
                product = c_inv_lk;
                //product = std::pow(c,-index*k);
                //std::cout << "c^(-lk) = " << product << std::endl;
                
                //std::cout << "i = " << i << " pdf2: c^(-lk) = " << product << std::endl;

                
                //product = one;
                for(size_t j=0;j<n;++j){
                    product *= one+(c_ell-one)*p[j];
                    //    std::cout << "product term = " << term << std::endl;
                    //product *=term;
                }
                c_ell *= c;
                c_inv_lk *= c_ell_k;
                
                //std::cout << c_inv_lk << std::endl;
                
                prob += product;
                //if(std::real(prob) < 0)
                //    std::cout << "negative!" << std::endl;
                
            }
            prob = prob / std::complex<long double>(n+1.);
            
            //std::cout << "prob["<<kk<<"] = " << prob << std::endl;
            
            //}
            return std::real(prob);
        }
       
    private:
        Vector p;
        
        
    };
    
    
    
#if 0
    /** @class DiscreteUniform
     @brief Uniform over set of elements {a,a+1,...,b-1,b}
     
     Inherits from RandomVariable so as long as operator()(URNG& gen) is overloaded to return a random value we can
     invoke its member functions.
     
     Store a lower and upper bound denoting the smallest and largest values capable of being generated.
     */
    template<typename ReturnType = int, typename ParameterType=ReturnType, typename URNG= std::mt19937_64>
    class independent_uniform_int_distributions : public RandomVariable<ReturnType, independent_uniform_int_distributions<ReturnType, ParameterType, URNG>, URNG> {
    public:
        
        /** Constructs a random variable for random values in {a,a+1,...,b-1,b}
         @param a is the lower bound
         @param b is the upper bound
         */
        template<typename InputIterator>
        independent_uniform_int_distributions(InputIterator start, InputIterator stop) {
            
            
            
        };
        
        /** Generates random value from distribution using the std distributions.
         @param gen is the random number generator, by default 64-bit.
         @returns random element
         */
        ReturnType operator()(URNG& gen = generator_64) {
            std::uniform_int_distribution<ReturnType> r(lower,upper);
            return r(gen);
        }
        
        /** Returns the expected value of the random variable
         ReturnType must have operator+(ReturnType, ReturnType) defined, and the return type of operator+ must be castable to F
         @returns (lower+upper)/2
         */
        template<typename F=double>
        F mean() {
            return (static_cast<F>(lower+upper))/2;
        }
        
        // TODO: Write more statistics properties like standard deviation, moments, kurtosis, etc.
        
    private:
        /** @var lower is the lower bound for random generation */
        std::uniform_int_distribution<ReturnType> unifs;
        /** @var upper is the upper bound for random generation */
        ParameterType upper;
    };
#endif
    
    /** @class truncated_geometric_distribution
     @brief truncated geometric distribution
     
     Samples from the distribution P(Z = j) = q^j / (1+q+...+q^(n-1)), where q>0 and n >=2, i.e., a geometric distribution conditioned to be less than n.
     
     */
    template<typename ReturnType = unsigned int, typename ParameterTypeInteger=ReturnType, typename ParameterTypeReal=long double, typename URNG= std::mt19937_64>
    class truncated_geometric_distribution : public random_variable<ReturnType, truncated_geometric_distribution<ReturnType, ParameterTypeInteger,ParameterTypeReal, URNG>, URNG> {
    public:
        
        /** Constructs a truncated geometric distribution, where minimum value is 1 and maximum value is n.
         
         P(Z = j) = q^(j-1) / (1+q+...+q^(n-1)), j=1,2,...,n
         @param n is the max possible value
         @param q is the probability of success
         */
        truncated_geometric_distribution(ParameterTypeInteger input_n, ParameterTypeReal input_q) : n(input_n), q(input_q), distribution(n) {
        
            auto start = std::begin(distribution);
            
            for(size_t i=0;i<n;++i)
                *start++ = std::pow(q,i);
            
        };
        
        ReturnType operator()(URNG& gen) {
            
            //std::discrete_distribution<ReturnType> discrete(std::begin(distribution), std::end(distribution));
            //std::cout << std::accumulate(std::begin(distribution), std::end(distribution),ParameterTypeReal(0.)) << std::endl;
            //return discrete(gen)+1;
            
            std::uniform_real_distribution<ParameterTypeReal> unif(0,std::accumulate(std::begin(distribution), std::end(distribution),ParameterTypeReal(0.)));
            
            double val = unif(gen);
            double index = 0;
            auto start = std::begin(distribution);
            double cum = *start++;;
            
            while(cum < val) {
                ++index;
                cum += *start++;
            }
            
            return index;
        }
        
        
    private:
        size_t n;
        double q;
                                                            
        std::vector<long double> distribution;
    };
    
    /** Quick random integer using 64-bit default generator
     @tparam a is the lower bound
     @tparam b is the upper bound
     @returns random number in {a,a+1,...,b-1,b}
     */
    template<typename T>
    T random_integer(T a, T b) {
        std::uniform_int_distribution<T> r(a,b);
        return r(generator_64);
    }
    
    /** Quick random integer using 64-bit default generator
     @tparam a is the lower bound
     @tparam b is the upper bound
     @returns random number in {a,a+1,...,b-1,b}
     */
    template<typename T, typename V = std::vector<T> >
    V random_integer_vector(T a, T b, size_t n) {
        std::uniform_int_distribution<T> r(a,b);
        V v(n);

        for(auto& x : v) x = r(generator_64);
        return v;
    }
    
    /** Quickly generate a random permutation of numbers {1,...,n} using default 64--bit generator
     @tparam n is the largest number
     @tparam gen is the random number generator, default at 64-bits
     @returns a permutation of elements 1 through n
     */
    template<typename N = std::size_t, typename V = std::vector<N>, typename URNG= std::mt19937_64 >
    V random_permutation(N n, URNG& gen = generator_64 ) {
        
        // Generate numbers 1 through n
        V v = dsl::range<N,V,N>(n,1);
        
        // permute them using the standard library
        std::shuffle(begin(v), end(v), gen );
        
        return v;
        
    };
    
    /** Returns a random permutation of the vector (val,val,...,val,[0],[0],...,[0]) of k vals and n-k [0]s, where [0] is the default value of the container.
     @param n is the size of the vector
     @param k is the number of vals
     @param val is the value to fill in
     @param gen is the uniform random number generator
     @tparam T is the type of each element
     @tparam Vector is the type which stores the collection of T objects
     @tparam URNG is the type of the random number generator
     @returns (val,val,...,val,[0],[0],...,[0]) of k vals and n-k [0]s
     
     */
    template< typename T=bool, typename Vector = std::vector<T>, typename URNG= std::mt19937_64 >
    Vector random_binary_row(size_t n, size_t k, T val=true, URNG& gen = generator_64) {
        // Put in tests for k>n and also separate cases k==0 and k==n.
        
        if(k > n) {
            k=n;
            std::cerr<<"binary_row number of values for default is larger than size of row\n";
        }
        
        Vector v(n);                      // Initialize values with default
        
        auto start = begin(v);            // Iterator to first element of v
        
        // Requires Forward Iterator type
        auto stop = std::next(start, k);  // Iterator to k-th element of v
        
        // Note that I did NOT write
        // auto stop = start+k;
        // If I use operator+(int), it assumes a Random Access Iterator
        
        // Requires Forward Iterator type
        std::fill(start, stop, val);        // Fill first k values with val
        
        std::shuffle(start, std::end(v), gen);
        
        return v;
    };
    
    
    /** Quickly generate a random permutation of numbers {1,...,n} using default 64--bit generator
     @tparam n is the largest number
     @tparam gen is the random number generator, default at 64-bits
     @returns a permutation of elements 1 through n
     
     Example 1:
     @code
     
#include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
#include <random>
#include <chrono>
    
    // avoiding 132
    template<typename T>
    bool contains_132_weakly(std::vector<T>& v) {
        
        size_t n = v.size();
        
        // Look at all triplets of indices to see if pattern is violated.
        // O(n^3) algorithm.
        for(size_t i=0;i<n-2;++i)
            for(size_t j=i+1;j<n-1;++j)
                for(size_t k=j+1;k<n;++k)
                    if( (v[i] < v[j] && v[i] < v[k] && v[j] >= v[k]) ){
                        std::cout << std::vector<size_t>({i,j,k}) << std::endl;
                        return true;
                    }
        
        return false;
    }
    
    
    int main(int argc, const char * argv[]) {
        
        auto v = dsl::random_permutation_mallows_in_mallows_form(10,1.);
        std::cout << v << std::endl;
        std::cout << contains_132_weakly(v) << std::endl;
        
        return 0;
    }
     @endcode
     
     Example 2:

     @code
    
    #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
    
    // avoiding 132
    template<typename T>
    bool contains_321_consecutively_weakly(const std::vector<T>& v) {
        
        size_t n = v.size();
        
        // Look at all triplets of indices to see if pattern is violated.
        // O(n) algorithm.
        for(size_t i=0;i<n-3;++i)
            if( (v[i] >= v[i+1] && v[i+1] >= v[i+2]) ){
                //std::cout << std::vector<size_t>({i,j,k}) << std::endl;
                return true;
            }
        //return true;
        
        return false;
        
    }
    
    
    int main(int argc, const char * argv[]) {
        
        // Create mesh grid
        double qmin = .01;
        double qmax = 4.;
        size_t mesh_size = 20;
        std::vector<double> vals(mesh_size);
        
        // n is the size of the permutation, m is the number of iterations.
        size_t n = 20;
        size_t m = 100000;
        
        // keeps track of which value of q we are using in the vector
        size_t index = 0;
        
        // q = qmin, qmin+delta, qmin+2delta, ..., qmax
        for(long double q = qmin; q <= qmax; q += (qmax-qmin)/(mesh_size-1)) {
            
            double avoids_321 = 0.; // count number that avoid 321
            
            for(size_t i=0;i<m;++i)
                
                // generate permutation using Mallows(q) distribution, test for whether it avoids 321 consecutively.
                if(contains_321_consecutively_weakly(dsl::random_permutation_mallows_in_mallows_form(n, q)))
                    avoids_321 = avoids_321 + 1.;
            
            // Keep track of which ones avoid, store the average^(1/n)
            if(avoids_321)
                vals[index++] = std::pow(avoids_321/m,1./n);
            else
                vals[index++] = 1.;
        }
        
        dsl::print(vals,"[", ",", "]");
        
        return 0;
    }
     @endcode
     */
    template<typename N = size_t, typename Float = long double, typename V = std::vector<N>, typename URNG= std::mt19937_64 >
    V random_permutation_mallows_in_mallows_form(N n, Float q, URNG& gen = generator_64 ) {
        
        // initialize array of size n
        V v(n);
        
        // check for empty set
        if(n == 0) return v;
        
        // initialize first entry to 1.
        auto start = std::begin(v);
        
        *start++ = 1;
        
        //size_t random_element;
        
        std::vector<Float> distribution(n+1);
        auto d_first = std::begin(distribution);
        auto stop = d_first;
        *stop++ = (Float)(1.);
        
        // for Mallows(q), we use Truncated Geometric(1/q).
        q = (Float)((Float)(1.)/q);
        
        //
        for(size_t index = 2; index <= n; ++index) {
            
            // generate Z, with P(Z = 1) = 1 / (1+q), P(Z=2) = q/(1+q).
            *stop++ = std::pow((Float)(q),(Float)(index-1));
            //std::cout << *stop << std::endl;
            
            //std::cout << *d_first << std::endl;
            // random number between {0,1,...,index-1} according to {1,q,q^2,q^3,...,q^(index-1)}
            std::discrete_distribution<size_t> discrete(d_first, stop);
            
            *start++ = discrete(gen)+1;
        }
        
        return v;
        
    };
    
    
    /** Returns a random permutation size n according to the Mallows(q) distribution.
     @param n is the size of the permutation
     @param q is the positive, real-valued weight
     @param gen is the uniform random number generator
     @tparam N is the type of the permutation elements
     @tparam Float is the type for q, which can be very sensitive for q small and q large
     @tparam URNG is the type of the random number generator
     @returns random permutation generated according to the Mallows(q) distribution
     
     
     
     Examples:
     @code
     #include <algorithm>
     #include <numeric>
     
     #include "std_cout.h"
     #include "permutation.h"
     #include "numerical.h"
     #include "statistics.h"
     
     namespace dsl = desalvo_standard_library;
     
     
     size_t number_of_123(const std::vector<size_t>& v) {
     
     size_t k = 0;
     
     for(size_t i=0,n=v.size()-2;i<n;++i)
     if(v[i]<v[i+1] && v[i+1]<v[i+2])
     ++k;
     
     return k;
     }
     
     
     
     size_t number_of_2341(const std::vector<size_t>& v) {
     
     size_t k = 0;
     
     for(size_t i=0,n=v.size()-3;i<n;++i)
     if(v[i+3]<v[i] && v[i+1]<v[i+2]&& v[i+2]<v[i+3])
     ++k;
     
     return k;
     }
     
     
     
     size_t number_of_1243(const std::vector<size_t>& v) {
     
     size_t k = 0;
     
     for(size_t i=0,n=v.size()-3;i<n;++i)
     if(v[i]<v[i+1]&& v[i+1]<v[i+3] && v[i+3] < v[i+2])
     ++k;
     
     return k;
     }
     
     size_t number_of_1432(const std::vector<size_t>& v) {
     
     size_t k = 0;
     
     for(size_t i=0,n=v.size()-3;i<n;++i)
     if(v[i]<v[i+3]&& v[i+3]<v[i+2] && v[i+2] < v[i+1])
     ++k;
     
     return k;
     }
     
     
     size_t number_of_1342(const std::vector<size_t>& v) {
     
     size_t k = 0;
     
     for(size_t i=0,n=v.size()-3;i<n;++i)
     if(v[i]<v[i+3]&& v[i+3]<v[i+1] && v[i+1] < v[i+2])
     ++k;
     
     return k;
     }
     
     
     
     int main(int argc, const char * argv[]) {
     
     //dsl::integer_partition_generator<unsigned int> ip_gen(100);
     
     //auto t = ip_gen.recursive_method_table(5,5);
     
     //std::cout << t << std::endl;
     
     
     
     size_t imax = 10000;
     size_t n = 1000;
     
     auto fun = number_of_1342;
     
     std::vector<size_t> counts(imax);
     
     std::cout << "datapoint2 = ";
     for(size_t i=0;i<imax;i++) {
     std::vector<size_t> v = dsl::random_permutation_mallows_ordering_construction<size_t,long double>(n,.2);
     counts[i]=fun(v);
     }
     
     std::cout << counts << std::endl;
     
     
     
     
     counts.resize(imax,0);
     std::cout << "datapoint5 = ";
     for(size_t i=0;i<imax;i++) {
     std::vector<size_t> v = dsl::random_permutation_mallows_ordering_construction<size_t,long double>(n,.5);
     counts[i]=fun(v);
     }
     
     std::cout << counts << std::endl;
     
     counts.resize(imax,0);
     std::cout << "data1 = ";
     for(size_t i=0;i<imax;i++) {
     std::vector<size_t> v = dsl::random_permutation_mallows_ordering_construction<size_t,long double>(n,1.);
     counts[i]=fun(v);
     }
     
     std::cout << counts << std::endl;
     
     
     
     counts.resize(imax,0);
     std::cout << "data1point3 = ";
     for(size_t i=0;i<imax;i++) {
     std::vector<size_t> v = dsl::random_permutation_mallows_ordering_construction<size_t,long double>(n,1.3);
     counts[i]=fun(v);
     }
     
     std::cout << counts << std::endl;
     
     
     
     counts.resize(imax,0);
     std::cout << "data2 = ";
     for(size_t i=0;i<imax;i++) {
     std::vector<size_t> v = dsl::random_permutation_mallows_ordering_construction<size_t,long double>(n,2.);
     counts[i]=fun(v);
     }
     
     std::cout << counts << std::endl;
     
     
     
     counts.resize(imax,0);
     std::cout << "data5 = ";
     for(size_t i=0;i<imax;i++) {
     std::vector<size_t> v = dsl::random_permutation_mallows_ordering_construction<size_t,long double>(n,5.);
     counts[i]=fun(v);
     }
     
     std::cout << counts << std::endl;
     
     return 0;
     }
     @endcode
     */
    template<typename N = size_t, typename Float = long double, typename URNG= std::mt19937_64 >
    std::vector<N> random_permutation_mallows_ordering_construction(N n, Float q, URNG& gen = generator_64 ) {
        
        // initialize array of size n
        std::vector<N> v(1);
        
        // check for empty set
        if(n == 0) return v;
        
        // initialize first entry to 1.
        auto start = std::begin(v);
        *start = 1;
        
        //size_t random_element;
        
        // Create location for distribution {1,1+q,1+q+q^2,...,1+q+q^2+...+q^n}
        std::vector<Float> distribution(n+1);
        distribution[0] = 1.;

        // Current index
        size_t d_index = 1;
        
        // for Mallows(q), we use Truncated Geometric(1/q).
        q = (Float)((Float)(1.)/q);
        
        // Put in second term, so distribution is now {1,1+q,0,0,...,0}
        Float current_power = q;
        
        distribution[d_index] = distribution[d_index-1]+current_power;
        
        size_t j=0;
        
        // This will be our newly generated value to determine Z
        Float variate = 0.;
        
        for(size_t index = 2; index <= n; ++index) {
            
            // generate Z, with P(Z = 1) = 1 / (1+q), P(Z=2) = q/(1+q).
            std::uniform_real_distribution<Float> unif(Float(0.), distribution[d_index]);
            
            // Get a number between 0 and 1+q+...+q^(index-1);
            variate = unif(gen);
            
            // find where variate lies in {1,1+q,1+q+q^2,...}
            j=0;
            start = std::begin(v);
            while(j < index && variate>=distribution[j++]) {
                ++start;
            }
            
            // insert latest element in appropriate location.
            //if(j==index)  v[index] = index;
            //else          v.insert(start, index);
            
            v.insert(start, index);
            
            ++d_index;
            current_power *= q;
            distribution[d_index] = distribution[d_index-1] + current_power;
            
            
            //*stop++ = std::pow((Float)(q),(Float)(index-1));
            //std::cout << *stop << std::endl;
            
            //std::cout << *d_first << std::endl;
            // random number between {0,1,...,index-1} according to {1,q,q^2,q^3,...,q^(index-1)}
            //std::discrete_distribution<size_t> discrete(d_first, stop);
            
            //*start++ = discrete(gen)+1;
        }
        
        return v;
        
    };
    
    
    /** Quickly generate a random permutation of numbers {1,...,n} using default 64--bit generator
     @tparam n is the largest number
     @tparam gen is the random number generator, default at 64-bits
     @returns a permutation of elements 1 through n
     */
    template<typename N = size_t, typename Float = long double, typename V = std::vector<N>, typename URNG= std::mt19937_64 >
    V random_permutation_mallows(N n, Float q, URNG& gen = generator_64 ) {
        
        // generate a random Mallows permutation in Mallows form
        V v = random_permutation_mallows_in_mallows_form(n,q,gen);
        
        // return if small enough, no transformation required.
        if(n <= 1) return v;
        
        // initialize first entry to 1.
        auto start = std::begin(v);
        auto stop = std::end(v);
        
        // set runner to first element, start to second element.
        auto runner = start++;
        
        while(start != stop) {
            runner = std::begin(v);
            while(runner != start) {
                if(*runner >= *start)
                    ++(*runner);
                ++runner;
            }
            ++start;
        }
        
        return v;
        
        /*
        
        
        auto start = beginv;
        *start++ = 1;
        
        size_t random_element;
        
        std::vector<Float> distribution(n);
        auto d_first = std::begin(distribution);
        auto stop = d_first;
        *stop++ = 1.;
        
        
        for(size_t index = 2; index <= n; ++index) {
            
            //std::cout << v << std::endl;
            // generate Z, with P(Z = 1) = 1 / (1+q), P(Z=2) = q/(1+q).
            
            *stop++ = std::pow(q,index-1);
            //std::cout << *stop << std::endl;
            
            //std::cout << *d_first << std::endl;
            // random number between {0,1,...,index-1} according to {1,q,q^2,q^3,...,q^(index-1)}
            std::discrete_distribution<size_t> discrete(d_first, stop);
            
            random_element = discrete(gen)+1;
            *start = random_element;
            //std::cout << *start << std::endl;
            
            // Promote all elements larger or equal to random one generated.
            auto promoter = beginv;
            while(promoter != start) {
                
                if(*promoter >= random_element)
                    ++(*promoter);
                ++promoter;
            }
            
            ++start;
            
        }
        
        return v;
         */
        
    };

    

    /** Quickly generate a random permutation of numbers {a,a+1,...,a+n-1} using default 64--bit generator
     @tparam n is the length of consecutive numbers
     @tparam gen is the random number generator, default at 64-bits
     @returns a permutation of elements {a,a+1,...,a_n-1}
     */
    template<typename N = std::size_t, typename V = std::vector<N>, typename URNG= std::mt19937_64 >
    V random_permutation_shifted(N n, N a, URNG& gen = generator_64 ) {
        
        // Generate numbers 1 through n
        V v = dsl::range<N,V,N>(n,a);
        
        // permute them using the standard library
        std::shuffle(begin(v), end(v), gen );
        
        return v;
        
    };

    
    /** Quickly generate a random permutation of numbers {1,...,n} using default 64--bit generator
     @tparam N is the data type of values, typically integer type
     @tparam V is the container for the collection of values of type N
     @tparam URNG is the random number generator type, by default mt19937_64
     @param n is the largest number
     @param gen is the random number generator, default at 64-bits
     @returns a permutation of elements 1 through n
     */
    template<typename N = std::size_t, typename V = std::vector<N>, typename URNG= std::mt19937_64 >
    V random_permutation_fixed_point_free(N n, URNG& gen = generator_64 ) {
        
        // Generate numbers 1 through n
        V v = dsl::range<N,V,N>(n,1);
        
        N fixed_points = static_cast<N>(1);
        do {
            
            fixed_points = static_cast<N>(1);
    
            // permute them using the standard library
            std::shuffle(begin(v), end(v), gen );
        } while( std::any_of(std::begin(v), std::end(v), [&](const N& i) { return i == (fixed_points++);} ));
            
        return v;
        
    };
    
    /** Returns a random index
     @param a is the lower bound
     @param b is the upper bound
     @tparam gen is the random generator, by default 32-bit
     @returns a random index between a and b.
     */
    template<typename URNG = std::mt19937 >
    inline size_t uniform_size_t(size_t a, size_t b, URNG& gen = generator_32) {
        return std::uniform_int_distribution<size_t>(a,b)(gen);
    };
    
    
    /** Randomly sample iid Bernoulli's conditional on having sum k.
     @mem  O(n)
     @time O(n log n) due to the shuffle operation
     @param n is the number of Bernoulli random variables
     @param k is the number of 1s.
     @tparam gen is the random generator, by default 64-bit
     @returns an ordered vector of n 0s and 1s, exactly k of which are 1s.
     */
    template< typename T=bool, typename Vector = std::vector<T>, typename N = size_t, typename URNG = std::mt19937_64 >
    Vector bernoull_iid_fixedsum(N n, N k, URNG& gen = generator_64) {
        
        // Create a row of size n initialized with exactly k ones.
        Vector v = dsl::binary_row(n,k);
        
        // Apply random permutation.
        std::shuffle(begin(v), end(v), gen );
        
        return v;
    }
    
    
    
    /** Randomly sample iid Bernoulli's conditional on having sum k.
     @mem  O(n)
     @time O(n) due to self-similar PDC
     @param n is the number of Bernoulli random variables
     @param k is the number of 1s.
     @tparam gen is the random generator, by default 64-bit
     @returns an ordered vector of n 0s and 1s, exactly k of which are 1s.
     
     Example 1:
     @code
     // Code to check the 6 choose 3 different sets, make sure each are occurring equally likely
     #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
     
     int main(int argc, const char * argv[]) {
     
     std::multiset< std::vector<bool> > s;
     
     size_t m = 100000;
     
     for(size_t i=0;i<m;++i) {
     
     
     s.insert(dsl::set_n_choose_k(6,3));
     }
     
     std::cout << s.count( {1,1,1,0,0,0} ) << std::endl;
     std::cout << s.count( {1,1,0,1,0,0} ) << std::endl;
     std::cout << s.count( {1,1,0,0,1,0} ) << std::endl;
     std::cout << s.count( {1,1,0,0,0,1} ) << std::endl;
     std::cout << s.count( {1,0,1,1,0,0} ) << std::endl;
     std::cout << s.count( {1,0,1,0,1,0} ) << std::endl;
     std::cout << s.count( {1,0,1,0,0,1} ) << std::endl;
     std::cout << s.count( {1,0,0,1,1,0} ) << std::endl;
     std::cout << s.count( {1,0,0,1,0,1} ) << std::endl;
     std::cout << s.count( {1,0,0,0,1,1} ) << std::endl;
     std::cout << s.count( {0,1,1,1,0,0} ) << std::endl;
     std::cout << s.count( {0,1,1,0,1,0} ) << std::endl;
     std::cout << s.count( {0,1,1,0,0,1} ) << std::endl;
     std::cout << s.count( {0,1,0,1,1,0} ) << std::endl;
     std::cout << s.count( {0,1,0,1,0,1} ) << std::endl;
     std::cout << s.count( {0,1,0,0,1,1} ) << std::endl;
     std::cout << s.count( {0,0,1,1,1,0} ) << std::endl;
     std::cout << s.count( {0,0,1,1,0,1} ) << std::endl;
     std::cout << s.count( {0,0,1,0,1,1} ) << std::endl;
     std::cout << s.count( {0,0,0,1,1,1} ) << std::endl;
     
    
    return 0;
     }
     @endcode
     
     Example 2:
     @code
     #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
     
     int main(int argc, const char * argv[]) {
     
     auto v = dsl::set_n_choose_k(1100,5);
     std::cout << v << std::endl;
     
     std::cout << std::count(std::begin(v), std::end(v), true) << std::endl;
     
     return 0;
     }
     @endcode
     
     Example 3:
     @code
     // Code to check the n choose k different sets, make sure each are occurring equally likely
     #include "desalvo/dsl_usings.h" // See documentation for list of keywords included in this file.
     
     int main(int argc, const char * argv[]) {
     
     // from (0,0) to (n,k)
     size_t n = 6;
     size_t k = 3;
     size_t m = 100000;
     
     std::multiset< std::vector<bool> > s;
     
     // Insert each generated set into s
     for(size_t i=0;i<m;++i)
     s.insert(dsl::set_n_choose_k(n,k));
     
     // Generate all possible paths from (0,0) to (n,k)
     dsl::north_east_lattice_path<bool> paths(n,k);
     
     // Print out each path along with the number of times it occurs in s
     for(auto& x : paths)
     std::cout << x << ": " << s.count(x) << std::endl;
     
     return 0;
     }
     @endcode
     */
    template< typename T=bool, typename Vector = std::vector<T>, typename N = size_t, typename URNG = std::mt19937_64 >
    Vector set_n_choose_k(N n, N k, URNG& gen = generator_64) {
        
        auto v = dsl::binary_row(n,k);
        std::shuffle(std::begin(v),std::end(v),gen);
        return v;
        
        /*
         
         // Attempt to use PDC to code the algorithm, suffers from numerical calculations and bounding maximal probability.
         
        // Create a row of size 2n initialized with all 0s.
        Vector v(n);
        
        // set a to the ceiling of n/2
        auto a = n%2? n/2+1 : n/2;
        auto b = n-a;
        
        // initialize iterators
        auto start = std::begin(v);
        auto stop = start;
        std::advance(stop, a);
        
        auto the_end = std::end(v);
        
        size_t total_1s = 0;
        
        auto PDC_k = k;
        
        std::uniform_real_distribution<double> unif(0.,1.);
        
        // while gap between elements is larger than 1.
        while(start != stop && stop != the_end  && total_1s < k) {
            
            // fill in 0s and 1s randomly
            auto save = start;
            
            // initialize pk = pn = 1 in case PDC_k == a+b and 1s are only outcome
            double pk = 1.;
            double pn = 1.;
            
            // count local number of 1s
            N local_k = 0;
            
            // only solution is all 1s
            if(PDC_k == a+b) {
                
                while(start != the_end) {
                    *start++ = 1; //bern(gen); //(gen()%n <= k);
                }
                return v;
                
            }
            else {
                
                // probability of success, may assume p < 1
                double p = static_cast<double>(PDC_k) / (a+b);
                std::bernoulli_distribution bern(p);
                
                while(start != stop)
                    *start++ = bern(gen); //(gen()%n <= k);
                
                // count the number of 1s
                local_k = std::count(save, stop, true);
                
                // automatic rejection if number of 1s generated exceeds total possible.
                if(local_k > PDC_k) {
                    pk = 0;
                    pn = 1;
                }
                else {
                
                    // Rejection probability
                    pk = dsl::binomial<double,double>(b,PDC_k-local_k)*std::pow(1./p-1.,local_k);
                    
                    // Can one prove?
                    // pk is either:
                    // 1.  Monotonically decreasing
                    // 2.  Monotonically increasing
                    // 3.  Monotonically increasing and then montonically decreasing.
                    
                    // If so, then we check pk evaluated at local_k = 0 and local_k = 1
                    // If decreasing, we use pk at local_k = 0 as max
                    // If not decreasing, then we continue until pk decreases, and use the value right before the decrease as max.
                    
         
                    // One attempt at bounding max_k pk, not good for large n
                    //N bmax = PDC_k > b/2 ? b/2 : PDC_k;
                    //pn = dsl::binomial<double,double>(b,bmax);
                
                    //if(p<0.5)
                    //    pn *= std::pow(1./p-1.,PDC_k);
         
                    
                    pn = dsl::binomial<double,double>(b,PDC_k);
                    for(auto i=0;i<=PDC_k;++i)
                        pn = std::max(pn, dsl::binomial<double,double>(b,PDC_k-i)*std::pow(1./p-1.,i));
                 
                    // check in case our bounds for pn were wrong.
                    if(pk > pn || pn == 0)
                        std::cout << "ERROR: pk="<<pk<<" pn="<<pn<<std::endl;
                    
                }
            }
            
            // Apply rejection probability
            if( unif(gen) <= pk/pn ) {
                
                // accept the sample!  update values
                total_1s += local_k;
                
                a= b%2? b/2+1 : b/2;
                b = b - a;
                std::advance(stop,a);

                PDC_k -= local_k;
            }
            else
                start = save;
        }
        
        // If the early abort condition is not met, then fill in final value
        if(total_1s < k) {
            
            // if the total number of 1s is less than n, then last element much be 1.
            *start = 1;
        }
        
        return v;
         */
    }
    
    /** Randomly sample iid Bernoulli's conditional on having sum k.
     @mem  O(n)
     @time O(n log n) due to the shuffle operation
     @param n is the number of Bernoulli random variables
     @param k is the number of 1s.
     @tparam gen is the random generator, by default 64-bit
     @returns an ordered vector of n 0s and 1s, exactly k of which are 1s.
     */
    template< typename T=bool, typename Vector = std::vector<T>, typename N = size_t, typename URNG = std::mt19937_64 >
    Vector set_2n_choose_n(N n, URNG& gen = generator_64) {
        
        return set_n_choose_k(2*n,n,gen);
    }
    
    
    
    
    /** Returns a vector of size k of the first k random indices from the set {1,2,...,n} by selecting independent indices.
     NOT RECOMMENDED FOR k >> sqrt(n).  Best for k = O(1).
     Duplicates are handled by the following algorithm:
     1.  Generate k independent indices
     2.  Sort indices using std::sort
     3.  remove duplicates using std::unique
     4.  regenerate indices until all are unique
     
     @mem O(k)
     @time O(k log(k))
     @tparam n is the full size of the permutation
     @tparam k is the size of the partial permutation
     @tparam gen is the random generator, by default 64-bit
     @returns the first k values in a random permutation of n.
     */
    template<typename N = size_t, typename V = std::vector<N>, typename URNG=std::mt19937_64 >
    inline V partial_permutation_rejection(N n, N k, URNG& gen = generator_64 ) {
        
        // Generator for random index in {one, one+1, ..., n}
        std::uniform_int_distribution< N > unif(1,n);
        
        // Generate a set of k random indices from {1,...,n}, possible duplicates
        V v(k);
        std::generate_n(begin(v), k, std::bind( unif, std::ref(gen)) );
        std::sort(begin(v), end(v));
        
        // Check for duplicates by:
        // 1.  Call std::sort
        // 2.  Call std::unique, which returns iterator to last element after duplicates deleted,
        // 3.  Repeat until iterator returned matches end iterator
        size_t max_iter = 10000;
        size_t iterations = 0;
        
        // Run until the generated values are unique
        while( auto it = std::unique(begin(v), end(v)) != end(v) && iterations++ < max_iter) {
        
            // Generate a set of k random indices from {1,...,n}, possible duplicates
            std::generate_n(begin(v), k, std::bind( unif, std::ref(gen)) );
            
            // Sort the generated values
            std::sort(begin(v), end(v));
        }
        
        // Print error in the event that the maximum iterations were reached.
        if(iterations >= max_iter) { std::cerr<<"Max iterations exceeded in partial_permutation_birthday."; };
        
        std::shuffle(begin(v), end(v), gen);
        
        return v;
    }
    
    /** Returns a vector of size k of the first k random indices from the set {1,2,...,n}.
     @mem O(n), or if k<sqrt(n) then $O(\sqrt{n} \log n)$
     @time O(n)
     @tparam n is the full size of the permutation
     @tparam k is the size of the partial permutation
     @tparam gen is the random generator, by default 64-bit
     @returns the first k values in a random permutation of n.
     */
    template<typename N = size_t, typename V = std::vector<N> , typename URNG = std::mt19937_64 >
    inline V partial_permutation(N n, N k, URNG& gen = generator_64 ) {
        
        // Again, test against k>n and also special cases k==0 and k==n
        if (k==0) return V( 0 );
        if (k==n) return random_permutation<N,V,URNG>(static_cast<size_t>(n),gen);
        
        // Select a cutoff based on birthday problem of duplicate birthdays
        if(k < sqrt(n) ) return partial_permutation_rejection<N,V,URNG>(n,k,gen);
        
        // else take first k elements of random permutation of {1,...,n}
        
        // Create random permutation of {1,...,n}
        V v = random_permutation<N,V,URNG>(n, gen);
        
        // MOVE first k values to partial
        V partial(k);
        std::move(begin(v), std::next(begin(v),k), begin(partial));
        
        return partial;
        
    }
    
    
    
    template< typename T=bool, typename Vector = std::vector<T>, typename N = size_t, typename Real = double, typename URNG = std::mt19937_64 >
    Vector set_n_choose_k_repeated(N n, N k, URNG& gen = generator_64) {
        
        Real p = (Real)(k)/n;
        
        std::bernoulli_distribution bern(p);
        
        Vector v(n);
        N total = 0;
        
        // generate random 0s 1s, keep track of total
        for(auto& x : v)
            total += (x = bern(gen));
        
        // while total does not equal the target, thin out the sample
        while(total != k) {
            
            auto permutation = dsl::random_permutation(n);
            
            // if we have less 1s than the target, add random 1s into buckets that don't already have 1s.
            if(total < k)
                
                for(size_t i=0,m=permutation.size();i<m;++i)
                    if( !v[permutation[i]] )
                        v[permutation[i]] = bern(gen);
            
            
            
            
        }
     
        
    }
        
        

    
    
    /** Randomly sample Bernoulli's with different parameters conditional on having sum k.
     @param p is a vector of probabilities
     @param k is the number of 1s.
     @tparam gen is the random generator, by default 64-bit
     @returns a vector of n 0s and 1s, exactly k of which are 1s in some random locations.
     */
    template< typename V = std::vector<bool> , typename T = std::vector<double>, typename N = size_t, typename URNG=std::mt19937_64 >
    V bernoulli_fixedsum_rejection(const T& p, N k, URNG& gen = generator_64) {

        size_t n = p.size();

        // Go through various fringe cases
        if(n == 0) return p;
        if(k == 0) return V(n);
        if(k == n) return V(n,true);
        if(k > n) std::cerr<<"Error: bernoulli_fixedsum_rejection number of 1s demanded exceeds number possible\n";

        // Output vector
        V v( p.size() );
        
        // Start your Iterators!
        auto start = begin(p);
        auto stop = end(p);
        
        auto vstart = begin(v);
        auto vstop = end(v);
        
        // Waiting to Get Lucky algorithm.
        while(std::accumulate( start, stop, 0) != k) {
            
            auto it = start;
            auto vit = vstart;
            while(it != stop) {
                std::bernoulli_distribution bern( *it++ );
                *vit++ = bern(gen);
                
            }
        }
        
        return v;
    }
    
    
    /** Randomly sample Bernoulli's with different parameters conditional on having sum k using Poisson Process.
     @param p is a vector of probabilities
     @param k is the number of 1s.
     @tparam gen is the random generator, by default 64-bit
     @returns an ordered vector of n 0s and 1s, exactly k of which are 1s.
     @returns an ordered vector of n 0s and 1s, exactly k of which are 1s.
     */
    template< typename V = std::vector<bool> , typename T = std::valarray<double>, typename N = size_t, typename URNG=std::mt19937_64, typename F = double >
    V poisson_fixedsum_poisson_process(const T& p, N k, URNG& generator= generator_64) {
        
        const size_t n = p.size();
        
        if(n == 0 || k == 0) return V(n);
        
        // If k is 0, no arrivals, all variables are 0
        if(k==0) {
            // If we assume default constructor initializes to dsl::zero<N>() equivalent, then
            return V(0);
            
            // If we can't assume V is initialized to dsl::zero<N>() or equivalent, then
            //V v(n); std::fill_n(begin(v), n, dsl::zero<N>() );
            //return v;
        }
        
        // Output vector
        V v( n );
        
        // Eventually will be able to replace with something like iid_uniform(generator, k);
        std::uniform_real_distribution<F> uniform(0,1);
        V PoissonArrivals(k);  // Conditional on k arrivals, they are uniformly distributed in the interval [0,1].
        auto pastart = begin(PoissonArrivals);
        std::generate_n(pastart, k, std::bind(uniform, std::ref(generator)) );
        
        
        // Start your Iterators!
        auto start = begin(p);
        auto stop = end(p);
        
        auto vstart = begin(v);
        auto vstop = end(v);
        
        auto pastop = end(PoissonArrivals);
        
        auto it = start;
        auto vit = vstart;
        auto pait = pastart;
        
        
        // Sort so arrivals are x1<x2<x3<...<xk
        dsl::matlab::sort(PoissonArrivals);
        
        // Compute partial sums of p, 0<s1<s2<...,sn=1
        //std::partial_sum(begin(p), end(p), begin(p));
        dsl::partial_sum_in_place(p);
        
        while( *pait != pastop) {
            
            while( *pait > *it++) { ++vit; }
            
            ++(*vit);
            
            
        }
        
        
        
        return v;
    }
    

    /** @class random_distinct_subset
        @brief Generates a subset of size k from {1,2,...,n}
     
        Many times I was wanting to generate a distinct set of indices from among {1,2,...,n}.  For example, placing rooks on a chess board the locations can be indexed by {1,2,...,n choose 2}, and if there are k rooks then we can generate a distinct subset of size k from this set and then map these indices to coordinates on a board.
    */
    template<typename T, typename V = std::vector<T>, typename URNG = std::mt19937_64>
    class random_distinct_subset : public random_variable<V, random_distinct_subset<T, V, URNG> > {
    public:
        
        /** Initialize the set and subset size
            @param input_n is the entire set of possible indices
            @param input_k is the size of the set of distinct elements
        */
        random_distinct_subset(T input_n, T input_k) : n(input_n), k(input_k) {  };

        /** Initialize the set and subset size to 0 and 0 by default, empty sets. */
        random_distinct_subset() : random_distinct_subset(0,0) { };

        /** Overloaded operator for use with CRTP
            @param gen is the random number generator, by default 64 bits
            @returns a random subset of k distinct numbers from the set {1,2,...,n}
        */
        V operator()(URNG& gen = generator_64) {
            return partial_permutation<T,V,URNG>(n, k);
        }
        
        /** Resets the parameters
            @param input_n is the new entire set of possible indices
            @param input_k is the new size of the set of distinct elements
        */
        void set_param(T input_n, T input_k) {
            n = input_n;
            k = input_k;
        }
        
    private:
        /** @var n is the set of indices */
        T n;
        
        /** @var k is the size of the set of distinct indices */
        T k;
    };
    
    
    
    
    /** @class numeric_data
        @brief stores collections of numeric data, calculates statistics
     
     This class is designed to quickly obtain and process numerical data and provide various simple statistics.
     
     Example 1:
     @code
     #include <iostream>
     #include "desalvo/dsl_usings.h"
     
     int main(int argc, const char * argv[]) {
     
         dsl::numeric_data<double> points;
         points.add_point(5);
         points.add_point(12);
         points.add_point(13);
         points.add_point(2);
         points.add_point(-123.3);
     
         points.print_points(std::cout, ",", "[","]"); std::cout << std::endl;
     
         std::cout << points << std::endl;
         std::cout << points.mean() << std::endl;
     
         return 0;
     }
     @endcode
     
     Output:
     @code
     [5,12,13,2,-123.3]
     {5,12,13,2,-123.3}
     -18.26
     @endcode
     
    */
    template<typename T, typename Container = std::vector<T> >
    class numeric_data {

    public:
        numeric_data() { };
        
        template<typename F>
        void add_point(F&& data_point) { points.push_back( data_point ); };
        
        template<typename F, typename String = std::string>
        void print_points(F&& out = std::cout,
                          String&& sep = String(","),
                          String&& open_bracket = String("{"),
                          String&& close_bracket = String("}")) const {
            // This would requires operator<< for the container class, bad idea in general
            //return out << points;
            
            auto last = --std::end(points);

            //const String& const_sep = sep;
            
            out << open_bracket; // {
            
            std::for_each(std::begin(points), last, [&sep,&out](const T& p) { out << p << sep;});
            
            out << *last << close_bracket; // [last point]}
            
        };
        
        void write_to_file(std::string filename) const {
            dsl::file< dsl::file_type::output > file(filename);
            file << points;
        }
        
        template<typename RealType = T>
        RealType mean() const {
            return std::accumulate(std::begin(points), std::end(points), static_cast<RealType>(0)) / static_cast<RealType>(points.size());
        }
        
        Container points;
    private:
        friend std::ostream& operator<<(std::ostream& out, const numeric_data& dp) {
            dp.print_points(out);
            return out;
        }
    };
    
    
    template<typename T=double, typename URNG=std::mt19937_64>
    T rand(T a=0., T b=1., URNG& gen=dsl::generator_64) {
        return std::uniform_real_distribution<T>(a,b)(gen);
    }
    
    
    
    
    
};




#endif
