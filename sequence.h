/* @file sequence.h
 @Author Stephen DeSalvo
 @date July, 2015
 @brief Classes for sequences using CRTP
 
 
 This file contains many classes useful for sequences that follow similar patterns.  It uses CRTP so it can be easily extended. \n
 
 1.  finite_sequence< ra/bi/for > handles finite sequences \n
 2.  infinite_sequence< bi/for > handles infinite sequences \n
 3.  integral_sequence< ra/bi/for> handles sequences as subsets of integers \n
 */

#ifndef DeSalvoLibraryTestKit_DeSalvoSequencesLibrary_h
#define DeSalvoLibraryTestKit_DeSalvoSequencesLibrary_h

#include <iostream>
#include <string>
#include <vector>
#include <thread>

namespace desalvo_standard_library {
    
    namespace dsl = desalvo_standard_library;
    
    /** @enum store is the internal storage */
    enum class store {random_access, bidirectional, forward};
    
    
    /** @class finite_sequence
        @brief A CRTP class for working with finite sequences
     
     The idea is to specify a sequence like permutations or Fibonacci numbers.  There are two aspects to storage: 1. How to store a particular value in the sequence; 2. How to store the collection of values. \n\n
     
     Each particular value is stored as type T, a template parameter.\n
     If the storage is one at a time, then point 2 is moot since we don't store the entire sequence, and we only work with objects of type T.
     If the storage is random access, then the template parameter V determines how the collection is stored.  Typically, V is an std::vector<T>, which is the default template parameter.\n
     
     There are currently three options for storage: 1. Random access; 2. Bidirectional; 3. Forward only.  Certain sequences like permutations can be easily incremented and decremented, whereas the partition numbers would need to be stored in a table in order to easily go backwards.  On the other hand, permutations up to order 10 can be enumerated completely, and so a random access table is reasonable to pre-compute and quickly access random elements.
     
     
     @tparam storage determines how the sequence is stored.
     @tparam Derived is assumed to be a class with member functions first_in_sequence(), last_in_sequence(), and next_in_sequence(V& v) defined.
    @tparam V is assumed to have a nested iterator type which is a random access iterator.
     */
    template<dsl::store storage, typename Derived, typename T, typename V = std::vector<T> >
    class finite_sequence;
    
    template<typename Derived, typename T, typename V>
    class finite_sequence<dsl::store::random_access, Derived, T, V> {
        friend std::ostream& operator<<(std::ostream& out, const finite_sequence& seq) {
            return out << seq.v;
        }
        
    public:
        finite_sequence() { };
        
        void store_sequence() {
            v.resize(0);
            
            const Derived& x = static_cast<const Derived&>(*this);
            auto start = x.first_in_sequence();
            //auto start = x.first_element;
            
            v.push_back(start);
            
            while( x.next_in_sequence(start) )
                v.push_back(start); 
        }
        
        template<typename String = std::string>
        void print(std::ostream& out, String ending = std::string("")) {
            out << v << ending;
        }
        
        size_t size() const { return v.size(); };
        
        T& operator[](size_t rank) { return v[rank]; }
        const T& operator[](size_t rank) const { return v[rank]; }
        
        
        //T* operator->() { return &v[0]; }
        //const T* operator->() const { return &v[0]; }
        
        class iterator : public std::iterator<std::random_access_iterator_tag, T> {
            
            friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs.rank == rhs.rank; }
            friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
            friend bool operator<(const iterator& lhs, const iterator& rhs) { return lhs.rank < rhs.rank; }
            friend bool operator<=(const iterator& lhs, const iterator& rhs) { return lhs==rhs || lhs < rhs; }
            friend bool operator>(const iterator& lhs, const iterator& rhs) { return !(lhs<=rhs);}
            friend bool operator>=(const iterator& lhs, const iterator& rhs) { return !(lhs<rhs);}
            
        public:
            
            iterator(const finite_sequence& seq) : sequence(seq), rank(0) { };
            iterator(const finite_sequence& seq, size_t index) : sequence(seq), rank(index) {
                object_pointer = &sequence.v[rank];
            };
            iterator(const iterator& it) : sequence(it.sequence), rank(it.rank), object_pointer(it.object_pointer) { }
            
            void swap(iterator& other) {
                std::swap(rank, other.rank);
                std::swap(object_pointer, other.object_pointer);
            }
            
            iterator& operator=(iterator it) {
                swap(it);
                return *this;
            }
            
            //T& operator[](size_t index) { return sequence.v[rank+index]; }
            const T& operator[](size_t index) const { return sequence.v[rank+index]; }
            
            //T* operator->() { return &sequence.v[rank]; }
            const T* operator->() const { return &sequence.v[rank]; }
            
            //T& operator*() { return sequence.v[rank]; }
            const T& operator*() const { return sequence.v[rank]; }
            
            iterator& operator+=(int increment) {
                rank += increment;
                object_pointer = &sequence.v[rank];
                return *this;
            }
            
            iterator& operator-=(int increment) {
                rank += increment;
                object_pointer = &sequence.v[rank];
                return *this;
            }
            
            
            
            iterator& operator++() {
                return (*this += 1);
            }
            
            iterator operator++(int unused) {
                auto clone(*this);
                operator++();
                return clone;
            }
            
            iterator& operator--() {
                return (*this -= 1);
            }
            
            iterator operator--(int unused) {
                auto clone(*this);
                operator--();
                return clone;
            }
            
            
            friend typename std::iterator<std::random_access_iterator_tag, T>::difference_type operator-(const iterator& lhs, const iterator& rhs) {
                return lhs.rank - rhs.rank;
            }
            
            friend iterator operator+(iterator lhs, int increment) { return lhs += increment; }
            friend iterator operator+(int increment, iterator lhs) { return lhs += increment; }
            friend iterator operator-(iterator lhs, int increment) { return lhs -= increment; }
            friend iterator operator-(int increment, iterator lhs) { return lhs -= increment; }
            
            
        private:
            const finite_sequence& sequence;
            const T* object_pointer;
            size_t rank;
        };
        
        iterator begin() const { return iterator(*this, 0); };
        iterator end()   const { return iterator(*this, this->size()); };
        
        size_t count() {
            return v.size();
        };
        
        
    private:
        V v; // stores all elements
    };
    
    
    
    
    
    template<typename Derived, typename T, typename V>
    class finite_sequence<dsl::store::bidirectional, Derived, T, V> {
        friend std::ostream& operator<<(std::ostream& out, const finite_sequence& seq) {
            return out << seq.v;
        }
        
    public:
        finite_sequence() { };
        
        /*template<typename String = std::string>
         void print(std::ostream& out, String ending = std::string("")) {
         out << v << ending;
         }
         */
        
        
        class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            
            friend bool operator==(const iterator& lhs, const iterator& rhs) {
                // If both are nullptr, return true
                if(lhs.current_element == nullptr && rhs.current_element == nullptr)
                    return true;
                
                // If neither is nullptr, check the state
                if(lhs.current_element != nullptr && rhs.current_element != nullptr)
                    return *lhs.current_element == *rhs.current_element;
                
                // If there is a mix, always return false.
                return false;
                
            }
            
            friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
            
        public:
            
            iterator(const finite_sequence& seq) : sequence(seq), current_element(new T) {
                const Derived& x = static_cast<const Derived&>(sequence);
                //auto first = x.first_in_sequence();
                
                // If the sequence is empty, i.e., no first element
                if( !x.first_in_sequence(*current_element) ) {
                    current_element = nullptr;
                }
                //ending = &current_element;
            };
            
            iterator(const finite_sequence& seq, T* heap_element) : sequence(seq), current_element(heap_element) {
                
            };
            
            iterator(const iterator& it) : sequence(it.sequence), current_element(new T) {
                if(it.current_element != nullptr)
                    *current_element = *it.current_element;
                else
                    current_element.reset(nullptr);
            }
            
            void swap(iterator& other) {
                //std::swap(rank, other.rank);
                std::swap(current_element, other.current_element);
                //std::swap(ending, other.ending);
            }
            
            iterator& operator=(iterator it) {
                swap(it);
                return *this;
            }
            
            //T* operator->() { return &sequence.v[0]; }
            const T* operator->() const { return current_element; }
            
            //T& operator*() { return sequence.v[rank]; }
            const T& operator*() const { return *current_element.get(); }
            
            
            iterator& operator++() {
                const Derived& x = static_cast<const Derived&>(sequence);
                
                if(current_element == nullptr) {
                    current_element.reset(new T);
                    x.first_in_sequence(*current_element);
                }
                else {
                    if(!x.next_in_sequence(*current_element)) {
                        current_element.reset(nullptr);
                        //rank = 0;
                    }
                }
                //else
                //++rank;
                return *this;
            }
            
            iterator operator++(int unused) {
                auto clone(*this);
                operator++();
                return clone;
            }
            
            iterator& operator--() {
                const Derived& x = static_cast<const Derived&>(sequence);
                
                if(current_element == nullptr) {
                    current_element.reset(new T);
                    *current_element = x.last_element;
                }
                
                if(!x.previous_in_sequence(*current_element)) {
                    current_element.reset(nullptr);
                }
                //else
                //--rank;
                return *this;
            }
            
            iterator operator--(int unused) {
                auto clone(*this);
                operator--();
                return clone;
            }
            
            
            
        private:
            const finite_sequence& sequence;
            std::unique_ptr<T> current_element;
        };
        
        iterator begin()  { return iterator(*this); };
        iterator end()    {
            auto it = iterator(*this, nullptr);
            return it;
        }
        
        const iterator begin() const { return iterator(*this); };
        const iterator end()   const {
            auto it = iterator(*this, nullptr);
            return it;
        }
        
        size_t count() {
            
            auto start = begin();
            auto stop = end();
            size_t iterations = 0;
            
            while(start != stop) {
                ++start;
                ++iterations;
            }
            
            return iterations;
            
        };
        
        
    private:
        
    };
    
    
    template<typename Derived, typename T, typename V>
    class finite_sequence<dsl::store::forward, Derived, T, V> {
        friend std::ostream& operator<<(std::ostream& out, const finite_sequence& seq) {
            return out << seq.v;
        }
        
    public:
        finite_sequence() { };
        
        /*template<typename String = std::string>
         void print(std::ostream& out, String ending = std::string("")) {
         out << v << ending;
         }
         */
        
        
        class iterator : public std::iterator<std::forward_iterator_tag, T> {
            
            friend bool operator==(const iterator& lhs, const iterator& rhs) {
                // If both are nullptr, return true
                if(lhs.current_element == nullptr && rhs.current_element == nullptr)
                    return true;
                
                // If neither is nullptr, check the state
                if(lhs.current_element != nullptr && rhs.current_element != nullptr)
                    return *lhs.current_element == *rhs.current_element;
                
                // If there is a mix, always return false.
                return false;
                
            }
            
            friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
            
        public:
            
            iterator(const finite_sequence& seq) : sequence(seq), current_element(new T) {
                const Derived& x = static_cast<const Derived&>(sequence);
                //auto first = x.first_in_sequence();
                *current_element = x.first_in_sequence();
                //ending = &current_element;
            };
            
            iterator(const finite_sequence& seq, T* heap_element) : sequence(seq), current_element(heap_element) {
                
            };
            
            iterator(const iterator& it) : sequence(it.sequence), current_element(new T) {
                if(it.current_element != nullptr)
                    *current_element = *it.current_element;
                else
                    current_element.reset(nullptr);
            }
            
            void swap(iterator& other) {
                //std::swap(rank, other.rank);
                std::swap(current_element, other.current_element);
                //std::swap(ending, other.ending);
            }
            
            iterator& operator=(iterator it) {
                swap(it);
                return *this;
            }
            
            //T* operator->() { return &sequence.v[0]; }
            const T* operator->() const { return current_element; }
            
            //T& operator*() { return sequence.v[rank]; }
            const T& operator*() const { return *current_element.get(); }
            
            
            iterator& operator++() {
                const Derived& x = static_cast<const Derived&>(sequence);
                
                if(current_element == nullptr) {
                    current_element.reset(new T);
                    *current_element = x.first_in_sequence();
                }
                else {
                    if(!x.next_in_sequence(*current_element)) {
                        current_element.reset(nullptr);
                        //rank = 0;
                    }
                }
                //else
                //++rank;
                return *this;
            }
            
            iterator operator++(int unused) {
                auto clone(*this);
                operator++();
                return clone;
            }
            
        private:
            const finite_sequence& sequence;
            std::unique_ptr<T> current_element;
        };
        
        iterator begin() const { return iterator(*this); };
        iterator end()   const {
            auto it = iterator(*this, nullptr);
            return it;
        }
        
        size_t count() {
            
            auto start = begin();
            auto stop = end();
            size_t iterations = 0;
            
            while(start != stop) {
                ++start;
                ++iterations;
            }
            
            return iterations;
            
        };
        
        
    private:
        
    };
    
    
    
    
    
    /** @class finite_sequence_threadable
     @brief A CRTP class for working with finite sequences
     
     The idea is to specify a sequence like permutations or Fibonacci numbers.  There are two aspects to storage: 1. How to store a particular value in the sequence; 2. How to store the collection of values. \n\n
     
     Each particular value is stored as type T, a template parameter.\n
     If the storage is one at a time, then point 2 is moot since we don't store the entire sequence, and we only work with objects of type T.
     If the storage is random access, then the template parameter V determines how the collection is stored.  Typically, V is an std::vector<T>, which is the default template parameter.\n
     
     There are currently three options for storage: 1. Random access; 2. Bidirectional; 3. Forward only.  Certain sequences like permutations can be easily incremented and decremented, whereas the partition numbers would need to be stored in a table in order to easily go backwards.  On the other hand, permutations up to order 10 can be enumerated completely, and so a random access table is reasonable to pre-compute and quickly access random elements.
     
     
     @tparam storage determines how the sequence is stored.
     @tparam Derived is assumed to be a class with member functions first_in_sequence(), last_in_sequence(), and next_in_sequence(V& v) defined.
     @tparam V is assumed to have a nested iterator type which is a random access iterator.
     */
    template<dsl::store storage, typename Derived, typename T, typename V = std::vector<T> >
    class finite_sequence_threadable;
    
    
    template<typename Derived, typename T, typename V>
    class finite_sequence_threadable<dsl::store::forward, Derived, T, V> {
        friend std::ostream& operator<<(std::ostream& out, const finite_sequence_threadable& seq) {
            return out << seq.v;
        }
        
    public:
        finite_sequence_threadable(size_t number_of_threads=std::thread::hardware_concurrency()) : threads(number_of_threads), count_total(0) {
        };
        
        /*template<typename String = std::string>
         void print(std::ostream& out, String ending = std::string("")) {
         out << v << ending;
         }
         */
        
        
        class iterator : public std::iterator<std::forward_iterator_tag, T> {
            
            friend bool operator==(const iterator& lhs, const iterator& rhs) {
                // If both are nullptr, return true
                if(lhs.current_element == nullptr && rhs.current_element == nullptr)
                    return true;
                
                // If neither is nullptr, check the state
                if(lhs.current_element != nullptr && rhs.current_element != nullptr)
                    return *lhs.current_element == *rhs.current_element;
                
                // If there is a mix, always return false.
                return false;
                
            }
            
            friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }
            
        public:
            
            iterator(const finite_sequence_threadable& seq) : sequence(seq), current_element(new T) {
                const Derived& x = static_cast<const Derived&>(sequence);
                //auto first = x.first_in_sequence();
                if(!x.first_in_sequence(*current_element)) {
                    current_element = nullptr;
                }
                //ending = &current_element;
            };

            iterator(const finite_sequence_threadable& seq, size_t in_thread) : sequence(seq), current_element(new T) {
                const Derived& x = static_cast<const Derived&>(sequence);
                //auto first = x.first_in_sequence();
                *current_element = x.first_in_sequence(in_thread);
                //ending = &current_element;
            };

            
            
            iterator(const finite_sequence_threadable& seq, T* heap_element) : sequence(seq), current_element(heap_element) {
                
            };
            
            iterator(const iterator& it) : sequence(it.sequence), current_element(new T) {
                if(it.current_element != nullptr)
                    *current_element = *it.current_element;
                else
                    current_element.reset(nullptr);
            }
            
            iterator(iterator&& it) : sequence(it.sequence), current_element(new T) {
                if(it.current_element != nullptr)
                    *current_element = *it.current_element;
                else
                    current_element.reset(nullptr);
            }
            
            
            void swap(iterator& other) {
                //std::swap(rank, other.rank);
                std::swap(current_element, other.current_element);
                //std::swap(ending, other.ending);
            }
            
            iterator& operator=(iterator it) {
                swap(it);
                return *this;
            }
            
            //T* operator->() { return &sequence.v[0]; }
            const T* operator->() const { return current_element; }
            
            //T& operator*() { return sequence.v[rank]; }
            const T& operator*() const { return *current_element.get(); }
            
            
            iterator& operator++() {
                const Derived& x = static_cast<const Derived&>(sequence);
                
                if(current_element == nullptr) {
                    current_element.reset(new T);
                    *current_element = x.first_in_sequence();
                }
                else {
                    if(!x.next_in_sequence(*current_element)) {
                        current_element.reset(nullptr);
                        //rank = 0;
                    }
                }
                //else
                //++rank;
                return *this;
            }
            
            iterator operator++(int unused) {
                auto clone(*this);
                operator++();
                return clone;
            }
            
        private:
            const finite_sequence_threadable& sequence;
            std::unique_ptr<T> current_element;
        };
        
        iterator begin() const { return iterator(*this); };
        iterator end()   const {
            auto it = iterator(*this, nullptr);
            return it;
        }
        
        
        iterator begin(size_t i) const { return iterator(*this, i); };
        iterator end(size_t i)   const {
            
            if(i == threads.size()-1)
                return iterator(*this, nullptr);
            else
                return iterator(*this, i+1);
            
        }
        
        
        void count_by_threads(iterator start, iterator stop) {
            
            //count_total = 0;
            while(start != stop) {
                ++start;
                ++count_total;
            }
        }

        
        size_t count_by_threads() {
            
            count_total = 0;
            
            for(size_t i=0,n=threads.size();i<n;++i) {
                auto start = this->begin(i);
                auto stop = this->end(i);
            
                //std::cout << *start << std::endl;
                //if(i!= n-1)
                //    std::cout << *stop << std::endl;
                
                threads[i] = std::move(std::thread([=] {this->count_by_threads(start,stop);}));
                //&finite_sequence_threadable::trythis, std::ref(*this));
                //threads[i] = std::move( std::thread(&finite_sequence_threadable::count_by_threads2, start, stop) );
                
                //std::thread(&finite_sequence_threadable::count_by_threads2, start, stop);
            }
            
            for(auto& x : threads)
                x.join();
            
            
            return count_total;
            
        };
        
        
    private:
        std::vector< std::thread > threads;
        std::atomic<unsigned int> count_total;
    };
    

    
}


#endif
