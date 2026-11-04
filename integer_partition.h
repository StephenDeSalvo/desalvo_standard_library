//
//  integer_partition.h
//  DeSalvo Standard Library
//
//  Uniformly random integer partitions of a given size, optionally restricted
//  to a set of allowed part sizes.
//
//  Three samplers return a partition of exactly n: RejectionSampling,
//  PDCDeterministicSecondHalf (probabilistic divide-and-conquer, faster), and
//  operator(). RandomSize is faster still and leaves the size random.
//
//  Restrictions are policy classes with operator()(IndexType i) returning the
//  i-th allowed part size; Unrestricted, Even, Odd, Triangular and JmodM are
//  supplied, and any type with that call signature works.
//
//  If you use this code, please credit the repository -- see the Attribution
//  section of README.md. PDCDeterministicSecondHalf implements "Improving
//  probabilistic divide-and-conquer: deterministic second half" (S. DeSalvo);
//  please cite the paper as well.
//

#ifndef DeSalvo_Standard_Library_integer_partition_h
#define DeSalvo_Standard_Library_integer_partition_h

#include <iostream>
#include <cmath>
#include <map>
#include <set>

#include <random>
#include <chrono>

namespace desalvo_standard_library {

namespace IP {
    

    /** @typedef ull is short for unsigned long long, the longest built-in integer type in the standard. */
    typedef unsigned long long ull;
    
    static std::mt19937_64 generator_64((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    static std::mt19937 generator_32((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    
    template<typename IndexType=ull>
    struct Unrestricted {
        IndexType operator()(IndexType i) { return i; };
    };

    template<typename IndexType=ull>
    struct Even {
        IndexType operator()(IndexType i) { return 2*i; };
    };
    
    template<typename IndexType=ull>
    struct Odd {
        IndexType operator()(IndexType i) { return 2*i-1; };
    };
    
    template<typename IndexType=ull>
    struct Triangular {
        IndexType operator()(IndexType i) { return i*(i+1)/2; };
    };
    
    template<typename IndexType=ull, ull J=1, ull M=1>
    struct JmodM {
        IndexType operator()(IndexType i) { return M*(i-1)+J; };
    };

    
    template<typename U, typename IndexType=ull, typename MultiplicityType=IndexType>
    class IntegerPartition  {
        
    public:

        // There is a noticeable lack of any constructors.  This is intentional to discourage anything other than default construction.
        
        
        /** Returns a multiset of parts, storing each part as a separate element.
            Asymptotically there are sqrt(n) log(n) / c parts in a partition.
            Asymptotically there are sqrt(n) differently sized parts in a partition.
            Returning a large list of partitions as a multiset is slightly less efficient than our choice.
         
            @returns a multiset with parts in descending order.
        */
        template<typename PartsType = MultiplicityType>
        std::multiset<PartsType, std::greater<PartsType> > AsMultiset() const {
            
            std::multiset<PartsType, std::greater<PartsType> > Parts;
            auto it = Parts.begin();
            
            for(auto x : multiplicities) {
                for(PartsType i=0, n=x.second;i<n; i++)
                    it = Parts.insert(it, x.first );
            }
            
            return Parts;
        }
        
        /** Output operator, outputs parts one at a time from largest to smallest as a multiset
            @param out is the output stream
            @param ip is the integer partition object
            @returns the output stream for overloading.
         */
        friend std::ostream& operator<<(std::ostream& out, const IntegerPartition& ip) {
            
            auto Parts = ip.AsMultiset();
            for(auto x : Parts)
                out << x << ",";
            return out;
            
        }
        
        /** Prints out the Ferrers diagram of the partition
            @param out is the output stream.
        */
        void Ferrer(std::ostream& out=std::cout) {
            
            auto LargeParts = AsMultiset();
            
            auto rit = LargeParts.crbegin();
            auto ritend = LargeParts.crend();;
            
            for(rit=LargeParts.crbegin(); rit != ritend; rit++)
            {
                ull val = *rit;
                for(int i=1;i<=val;i++)
                {
                    out<<"* ";
                }
                out<<std::endl;
            }
        }
        
        // Random Partition Functions that use O(n) algorithm
        template<typename URNG= std::mt19937_64, typename FloatingType=long double>
        void RandomSize(IndexType m, FloatingType manual_x=1, URNG& gen = generator_64);
 
        template<typename URNG= std::mt19937_64, typename FloatingType=long double>
        void RejectionSampling(IndexType m, FloatingType manual_x=1, URNG& gen = generator_64);
        
        template<typename URNG= std::mt19937_64, typename FloatingType=long double>
        void PDCDeterministicSecondHalf(IndexType m, FloatingType manual_x=1, URNG& gen = generator_64);
        
        template<typename URNG= std::mt19937_64, typename FloatingType=long double>
        void operator()(IndexType m, FloatingType manual_x=1, URNG& gen = generator_64);
        
        /** Calculates the weight of the partition.
            @returns the weight of the partition
        */
        IndexType n() {
            IndexType temp = 0;
            
            for(auto x : multiplicities) {
                temp += x.first * x.second;
            }
            
            //for(IndexType i=1;i<multiplicities.size();i++)
            //    temp += i*multiplicities[i];
            
            return temp;
        }
        
    private:
        
        /** @var multiplicities stores (i, c_i) pairs of elements. */
        std::map<IndexType,MultiplicityType> multiplicities;
        /** @var u sets the policy for which parts are allowed */
        U u;
    };
    
    
    /**
     Computes the value of x that solves ET = n
     
     @param n is the target value
     @return 5 digits for n<200, and returns 1-c/sqrt(n) for n>201;
     */
    template<typename InputType, typename ReturnType=long double>
    ReturnType findx(InputType n)
    {
        
        ReturnType x1;
        if(n>201)
        {
            const ReturnType pi = 3.1415926535897932384626433832;
            x1 = 1.0-pi/sqrt(6.0)/sqrt(n);
        }
        else
        {
            ReturnType xx[201] = {0,0.5,0.54031,0.57202,0.59784,0.61942,0.63781,0.65374,0.6677,0.68009,0.69116,0.70114,0.7102,0.71847,0.72606,0.73306,0.73954,0.74555,0.75117,0.75641,0.76134,0.76597,0.77033,0.77445,0.77836,0.78206,0.78558,0.78892,0.79212,0.79516,0.79808,0.80087,0.80354,0.80611,0.80857,0.81094,0.81322,0.81542,0.81754,0.81959,0.82157,0.82348,0.82533,0.82712,0.82885,0.83054,0.83217,0.83375,0.83529,0.83679,0.83824,0.83966,0.84104,0.84238,0.84368,0.84496,0.8462,0.84741,0.8486,0.84975,0.85088,0.85198,0.85306,0.85411,0.85514,0.85615,0.85714,0.8581,0.85905,0.85998,0.86089,0.86178,0.86265,0.86351,0.86435,0.86517,0.86598,0.86677,0.86755,0.86832,0.86907,0.86981,0.87054,0.87125,0.87195,0.87264,0.87332,0.87399,0.87465,0.87529,0.87593,0.87656,0.87717,0.87778,0.87838,0.87897,0.87955,0.88012,0.88068,0.88124,0.88179,0.88233,0.88286,0.88339,0.8839,0.88442,0.88492,0.88542,0.88591,0.88639,0.88687,0.88734,0.88781,0.88827,0.88872,0.88917,0.88962,0.89005,0.89049,0.89091,0.89134,0.89175,0.89216,0.89257,0.89298,0.89337,0.89377,0.89416,0.89454,0.89492,0.8953,0.89567,0.89604,0.8964,0.89676,0.89712,0.89747,0.89782,0.89817,0.89851,0.89885,0.89918,0.89952,0.89984,0.90017,0.90049,0.90081,0.90113,0.90144,0.90175,0.90205,0.90236,0.90266,0.90296,0.90325,0.90354,0.90383,0.90412,0.90441,0.90469,0.90497,0.90524,0.90552,0.90579,0.90606,0.90633,0.90659,0.90685,0.90712,0.90737,0.90763,0.90788,0.90813,0.90838,0.90863,0.90888,0.90912,0.90936,0.9096,0.90984,0.91008,0.91031,0.91054,0.91077,0.911,0.91123,0.91145,0.91167,0.9119,0.91212,0.91233,0.91255,0.91276,0.91298,0.91319,0.9134,0.91361,0.91382,0.91402,0.91422,0.91443};
            x1 = xx[(size_t)n];
        }
        return x1;
    }
    
    
    /**
     Returns $\sum_{i\in U} \frac{i x^i}{1-x^i}$, which is the expected size of a random partition with parts in $U$ of size $\leq n$ using parameter $x$
     
     @param x is the tilt.
     @param n is the target.
     @param U is the function pointer to the set U
     @return expected value of the random partition.
     */
    template<typename U, typename IndexType=ull, typename ReturnType = long double>
    long double ExpectedSum(ReturnType x, IndexType n)
    {
        U u;
        ReturnType res = 0.0;
        ReturnType xi=0.5;
        
        IndexType j=1;
        
        // IMPORTANT!  The i!=0 is there if we want a finite sequence u(1),...,u(k).  We set u(j)=0 for j>k.
        for(IndexType i=u(j);i<=n && i!=0 ;i=u(++j))
        {
            xi = pow(x,(ReturnType)i);
            res += (ReturnType)i*xi/((ReturnType)1.0-xi);
            
        }
        return res;
    }
    
    /**
     
     */
    template<typename U, typename IndexType=ull, typename ReturnType=long double>
    ReturnType xsolvebisection(IndexType n)
    {
        const ReturnType c = 1.2825498301618643;

        ReturnType x0 = 1.-c/sqrt((ReturnType)n);
        //ReturnType xf = .99999999999;
        ReturnType xf = (ReturnType)1. - .0000000000000001;
        ReturnType xi = 0.1;
        
        //cout<<ExpectedSum<U>(x0,n)<<endl<<endl;
        
        ReturnType r1 = ExpectedSum<U,IndexType,ReturnType>(x0,n)-(ReturnType)n;
        ReturnType r2 = ExpectedSum<U,IndexType,ReturnType>(xf,n)-(ReturnType)n;
        ReturnType r3 = 0;
        
        size_t iters = 0;
        size_t max_iters = 1000;
        
        while(fabs(r1-r2)>.00001 && iters < max_iters)
        {
            xi = (x0+xf)/2.;
            r3 = ExpectedSum<U,IndexType,ReturnType>(xi,n)-(ReturnType)n;
            //std::cout << r1 << std::endl;
            //std::cout << r2 << std::endl;
            //std::cout << r3 << std::endl;
            if(r3<0)
            {
                x0 = xi;
                r1 = r3;
            }
            else
            {
                xf = xi;
                r2 = r3;
            }
            ++iters;
            //r4 = fabs(r1-r2);
        }
        
        //printf("%f %f %f\n", r1,r2,xi);
        return xi;
    }
    
    
    /** Finds the value of x when the parts have restrictions
        @param n is the size of the partition
        @returns either 1-c/sqrt(6n) for large n or a precomputed table value for small n
    */
    template<typename U, typename IndexType=ull, typename ReturnType = long double>
    long double findx(IndexType n) {
        return xsolvebisection<U,IndexType,ReturnType>(n);
    }

    
    
    /**
     Creates a random integer partition of random size, using Fristedt's method, overwrites current object.
     This implementation is O( u^(-1)(n) ).
     
     @param m is the expected size of the partition.
     @param gen is the random number generator.
     */
    template<typename U, typename IndexType, typename MultiplicityType>
    template<typename URNG, typename FloatingType>
    void IntegerPartition<U,IndexType, MultiplicityType>::RandomSize(IndexType m, FloatingType x_manual, URNG& gen) {
        
        multiplicities.clear();

        // Manual setting of the value x when U is unrestricted.
        //const FloatingType pi = 3.1415926535897932384626433832;
        //FloatingType logx = -sqrt(6.0*m)/pi;
        
        FloatingType x;
        
        // Due to numerical errors, giving a chance to manually input a value for x
        if(x_manual < 1) {
            x = x_manual;
        }
        else {
            x = findx<U,IndexType,FloatingType>(m);
        }
        
        FloatingType logx = log(x);
        
        std::uniform_real_distribution<FloatingType> A; // Default is uniform over [0,1]
        
        IndexType k=1;
        MultiplicityType value;
        
        // The i!=0 is for a finite set of part sizes u(1),u(2),...,u(k), with u(j)=0 for all j>k.
        for(IndexType i = u(k); i<=m && i!=0; i=u(++k)) {
            
            // Generate geometric random variables with different parameters.
            // I could have created an array of std::geometric_distribution<FloatingType> or
            // reset the parameters each time, but it is just easier to apply the transformation to
            // a uniform and will almost certainly be faster than changing parameters around.
            
            if( (value = static_cast<MultiplicityType>(floor(log( A(gen) )/(i*logx)))) )
                multiplicities[i] = value;
        }

    }
    
    
    /**
     Creates a random integer partition of size m, uniformly over all partitions using Fristedt's O(n) method, overwrites current object.
     
     @param m is the size of the partition.
     @param gen is the random number generator
     */
    template<typename U, typename IndexType, typename MultiplicityType>
    template<typename URNG, typename FloatingType>
    void IntegerPartition<U,IndexType,MultiplicityType>::RejectionSampling(IndexType m, FloatingType x_manual, URNG& gen) {
        
        // Rejection sampling.  Generate random partition of random size until the size is m.
        do {
            RandomSize(m, x_manual, gen);
        } while(n() != m);
        
    }
    
    /**
     Creates a random integer partition of size m, uniformly over all partitions using Fristedt's O(n) method, and PDC trivial second half with b=1, i.e., the set A = {2,3,...,n}, and trivial second half B = {1}.
     
     @param m is the size of the partition.
     @param gen is the random number generator.
     */
    template<typename U, typename IndexType, typename MultiplicityType>
    template<typename URNG, typename FloatingType>
    void IntegerPartition<U,IndexType,MultiplicityType>::PDCDeterministicSecondHalf(IndexType m, FloatingType x_manual, URNG& gen) {
        
        IndexType partial_total = 0;
        
        // By default, generates random uniform between 0 and 1
        std::uniform_real_distribution<FloatingType> unif { };
        bool accepted = 0;
        
        FloatingType x = findx<U,IndexType,FloatingType>(m);
        
        do {
            RandomSize(m,x_manual,gen);
            
            // Even if no parts of size 1 exist, this call will create a map with initial value 0.
            multiplicities[u(1)] = 0;
            
            partial_total = n();
            
            IndexType diff = m - partial_total;
            // Check the DSH condition.
            //FloatingType check =(FloatingType)pow(x,(FloatingType)(m-partial_total));
            if( (partial_total <= m) && (diff%u(1) == 0) && (unif(gen) <= (FloatingType)pow(x,u(1)*(FloatingType)(diff))) ) {
                multiplicities[u(1)] = (m-partial_total)/u(1);
                accepted = true;
            }
        
        } while(!accepted);
        
        
        return;
    }
    
    
    /** Default random generator set to PDC Deterministic Second Half.  Use this unless you want manual control over the algorithm, as this function can be improved in the future whereas the others fix a method.
     
        Justification: Currently, the algorithm is the PDC Deterministic Second Half, which is provably faster than the standard rejection sampling algorithm.  Both are implemented, but PDC DSH is clearly preferred.
     
        It is possible the algorithm may be improved in the future, which is why the function call is generic and the code inside can be replaced with a more efficient algorithm.
     
     @param m is the size of the partition.
     @param x_manual is the manually set value of x in cases of numerical instability
     @param gen is the random number generator.
     */
    template<typename U, typename IndexType, typename MultiplicityType>
    template<typename URNG, typename FloatingType>
    void IntegerPartition<U,IndexType,MultiplicityType>::operator()(IndexType m, FloatingType x_manual, URNG& gen) {
        PDCDeterministicSecondHalf(m,x_manual,gen);
    }

    
    typedef IP::IntegerPartition<IP::Unrestricted<ull>, ull, ull> UnrestrictedPartition;
    typedef IP::IntegerPartition<IP::Even<ull>, ull, ull> EvenPartition;
    typedef IP::IntegerPartition<IP::Odd<ull>, ull, ull> OddPartition;

    
}

}   // namespace desalvo_standard_library

// The library's own short alias, and the spelling this class has
// always been documented with, so existing code keeps compiling.
namespace dsl = desalvo_standard_library;
namespace IP  = desalvo_standard_library::IP;

#endif
