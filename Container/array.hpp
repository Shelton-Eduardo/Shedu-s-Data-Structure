#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

// includes
#include <initializer_list>
#include <type_traits>
#include <concepts>
#include <cstddef>
#include <utility>
#include <exception>

namespace Container{
    template <typename T> concept Custom = sizeof(T) <= 24;

    class OutOfBoundsException : public std::exception{
        private:
            const char* message;
        public:
            OutOfBoundsException(const char* ms = "Out of Bounds exception occoured") : message(ms) {}
            const char* what() const override{
                return message;
            }
    };

    class UnderFlowException : public std::exception{
        private:
            const char* message;
        public:
            UnderFlowException(const char* ms = "Underflow exception occoured") : message(ms) {}
            const char* what() const override{
                return message;
            }
    };

    template <Custom T>
    class Array{
        private:
            T* arr;
            std::size_t counter,
            m_capacity;
        public:
            static constexpr std::size_t npos{static_cast<std::size_t>(-1)};

            Array();
            explicit Array(std::size_t reserve_space);
            Array(std::initializer_list<T> reserved_items);
            Array(std::size_t reserve_space, T&& fill_item);
            Array(std::size_t reserve_space, const T& fill_item);
            // Copy & Move constructorss
            Array(const Array& source);
            Array(Array&& source) noexcept;
            // Copy & Move assigments
            Array& operator=(const Array& source);
            Array& operator=(Array&& source) noexcept;
            
            ~Array() noexcept;

            // Key Functionality
            [[nodiscard]] bool append(std::initializer_list<T> items) noexcept(std::is_nothrow_copy_constructible_v<T>);
            [[nodiscard]] bool append(const T& item) noexcept(std::is_nothrow_copy_constructible_v<T>);
            [[nodiscard]] bool append(T&& item) noexcept(std::is_nothrow_move_constructible_v<T>);
            [[nodiscard]] bool insert(std::size_t idx, std::initializer_list<T> items);
            [[nodiscard]] bool insert(const T& item, std::size_t idx);
            [[nodiscard]] bool insert(T&& item, std::size_t idx);
            template <typename... Args>
            requires std::is_constructible_v<T, Args...>
            [[nodiscard]] bool emplace(Args&&... args);
            template <typename... Args>
            requires std::is_constructible_v<T, Args...>
            [[nodiscard]] bool emsert(const std::size_t idx, Args&&... args);
            T& operator[](std::size_t idx);
            const T& operator[](std::size_t idx) const;
            T pop();
            T pop(std::size_t idx);
            void remove();
            void remove(const T& item);
            void remove(std::size_t idx);

            // Helper Functions
            std::size_t len() const noexcept; // get counter
            std::size_t len(const T& item) const noexcept; // counter for spcific items
            std::size_t maxCapacity() const noexcept;

            // Utils Functions
            std::size_t memory(bool flag = false) const noexcept; // how much memory current elements are taking (flag), both initialized and uninitialized elements (!flag)

            // Programmer Friendly Functions
            Array& operator+=(const T& item);
            Array& operator+=(T&& item);
            Array& operator-=(const T& item);

            explicit operator bool() const noexcept;

            // Friend Functions
            friend std::ostream& operator<<(std::ostream& printer, const Array& source){
                printer << '[';
                for(std::size_t i{}; i<source.counter; ++i){
                    printer << source.arr[i] << ((i+1!=source.counter) ? ", ": "");
                }
                printer << ']';
                return printer;
            }

            friend std::istream& operator>>(std::istream& writter, Array& source){
                T temp;
                while(writter >> temp){
                    source.append(temp);
                    if(writter.eof() || writter.peek() == '\n') break;
                }
                writter.clear();
                return writter;
            }

            friend Array operator+(const Array& lhs, const Array& rhs){
                Array<T> temp(lhs.len()+rhs.len());
                for(std::size_t i{}; i<lhs.len(); ++i) temp.append(lhs[i]);
                for(std::size_t i{}; i<rhs.len(); ++i) temp.append(rhs[i]);
                return temp;
            }

            friend Array operator+(const Array& lhs, const T& rhs){
                Array<T> temp(lhs.len()+1);
                for(std::size_t i{}; i<lhs.len(); ++i) temp.append(lhs[i]);
                temp.append(rhs);
                return temp;
            }

            friend Array operator-(const Array& lhs, const Array& rhs){
                Array<T> temp{lhs.len()};
                for(std::size_t i{}; i<lhs.len(); ++i){
                    for(std::size_t j{}; j<rhs.len(); ++j) if (lhs[i] == rhs[j]) continue;
                    temp.append(lhs[i]);
                }
                return temp;
            }

            friend Array operator-(const Array& lhs, const T& rhs){
                Array<T> temp{lhs.len()};
                for(std::size_t i{}; i<lhs.len(); ++i) if(lhs[i] != rhs) temp.append(lhs[i]);
                return temp;
            }

            friend Array operator*(const Array& lhs, const T& rhs) 
                requires requires(T& a, const T& b){
                    {a*a} -> std::convertible_to<T>;
                    {a*b} -> std::convertible_to<T>;
                }{
                    Array<T> temp{lhs.len()};
                    for(const T& item : temp) temp.append(item*rhs);
                    return temp;
                }

            public:
            struct iterator{
                using pointer           = T*;
                using reference         = T&;
                using value_type        = T;

                value_type* m_ptr;
                
                iterator(pointer ptr);

                reference operator*() const noexcept;
                pointer operator->() const noexcept;
                
                iterator& operator++() noexcept;
                iterator operator++(int) noexcept;

                friend bool operator==(const iterator& lhs, const iterator& rhs){
                    return lhs.m_ptr == rhs.m_ptr;
                }
                friend bool operator!=(const iterator& lhs, const iterator& rhs){
                    return lhs.m_ptr != rhs.m_ptr;
                }

            };

            struct const_iterator{
                using pointer           = const T*;
                using reference         = const T&;
                using value_type        = const T;

                value_type* m_ptr;
                
                const_iterator(pointer ptr);

                reference operator*() const noexcept;
                pointer operator->() const noexcept;
                
                const_iterator& operator++() noexcept;
                const_iterator operator++(int) noexcept;

                friend bool operator==(const const_iterator& lhs, const const_iterator& rhs){
                    return lhs.m_ptr == rhs.m_ptr;
                }
                friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs){
                    return lhs.m_ptr != rhs.m_ptr;
                }

            };

            // Iterator Functions
            iterator begin() noexcept;
            iterator end() noexcept;
            const_iterator begin() const noexcept;
            const_iterator end() const noexcept;

            const_iterator cbegin() const noexcept;
            const_iterator cend() const noexcept;
    };

    // Constructors
    template <Custom T>
    Array<T>::Array() : counter(0), m_capacity(4){
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
    }

    template <Custom T>
    Array<T>::Array(std::size_t reserved_space) : counter(0), m_capacity((reserved_space < 1 || reserved_space == npos) ? 1 : reserved_space){
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
    }

    template <Custom T>
    Array<T>::Array(std::initializer_list<T> reserved_items) : counter(0), m_capacity((reserved_items.size() == 0) ? 1 : reserved_items.size()){
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
        try{
            for(const T& item : reserved_items){
                ::new(&arr[counter]) T(item);
                ++counter;
            }
        }catch(...){
            if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
            ::operator delete(arr);
            throw;
        }
    }

    template <Custom T>
    Array<T>::Array(std::size_t reserve_space, T&& fill_item) : counter(0), m_capacity((reserve_space < 1 || reserve_space == npos) ? 1 : reserve_space){
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
        try{
            while(counter < m_capacity-1){
                ::new(&arr[counter]) T(fill_item);
                ++counter;
            }
            ::new(&arr[counter]) T(std::move(fill_item));
            ++counter;
        }catch(...){
                if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
                ::operator delete(arr);
                throw;
            }
    }

    template <Custom T>
    Array<T>::Array(std::size_t reserve_space, const T& fill_item) : counter(0), m_capacity((reserve_space < 1 || reserve_space == npos) ? 1 : reserve_space){
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
        try{
            while(counter < m_capacity){
                ::new(&arr[counter]) T(fill_item);
                ++counter;
            }
        }catch(...){
            if constexpr(std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
            ::operator delete(arr);
            throw;
        }
    }

    // Copy & Move constructor
    template <Custom T>
    Array<T>::Array(const Array& source) : counter(0), m_capacity(source.m_capacity){
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
        try{
            while(counter < m_capacity){
                ::new(&arr[counter]) T(source.arr[counter]);
                ++counter;
            }
        }catch(...){
            if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
            ::operator delete(arr);
            throw;
        }
    }

    template <Custom T>
    Array<T>::Array(Array&& source) noexcept : counter(source.counter), m_capacity(source.m_capacity), arr(source.arr){
        source.arr = nullptr;
        source.m_capacity = source.counter = 0;
    }

    // Copy & Move constructor
    template <Custom T>
    Array<T>& Array<T>::operator=(const Array& source){
        if(this == &source) return *this;
        if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
        ::operator delete(arr);
        m_capacity = source.m_capacity;
        counter = 0;
        arr = static_cast<T*>(::operator new(m_capacity*sizeof(T)));
        arr[0];
        source.arr[0];
        try{
            while (counter < source.counter){
                ::new(&arr[counter]) T(source.arr[counter]);
                ++counter;
            }
        }catch(...){
            if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
            ::operator delete(arr);
            throw;
        }return *this;
    }

    template <Custom T>
    Array<T>& Array<T>::operator=(Array&& source) noexcept{
        if(this == &source) return *this;
        if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
        ::operator delete(arr);
        m_capacity = source.m_capacity;
        counter = source.counter;
        arr = source.arr;
        source.m_capacity = source.counter = 0;
        source.arr = nullptr;
        return *this;
    }

    template <Custom T>
    Array<T>::~Array() noexcept{
        if constexpr(!std::is_trivially_destructible_v<T>) while(counter > 0) arr[--counter].~T();
        ::operator delete(arr);
        arr = nullptr;
        m_capacity = counter = 0;
    }

    // Key Functionality
    template <Custom T>
    bool Array<T>::append(std::initializer_list<T> items) noexcept(std::is_nothrow_copy_constructible_v<T>){
        if(counter+items.size() > m_capacity) return false;
        std::size_t committed_items{0};
        try{
            for(const T& item : items){
                ::new(&arr[counter+committed_items]) T(item);
                ++committed_items;
            }
        }catch(...){
            if constexpr(!std::is_trivially_destructible_v<T>) while(committed_items > 0) arr[counter+(--committed_items)].~T();
            if constexpr(std::is_nothrow_copy_constructible_v<T>) return false;
            else throw;
        }counter += items.size(); 
        return true;
    }

    template <Custom T>
    bool Array<T>::append(const T& item) noexcept(std::is_nothrow_copy_constructible_v<T>){
        if(counter == m_capacity) return false;
        ::new(&arr[counter]) T(item);
        ++counter;
        return true;
    }

    template <Custom T>
    bool Array<T>::append(T&& item) noexcept(std::is_nothrow_move_constructible_v<T>){
        if(counter == m_capacity) return false;
        ::new(&arr[counter]) T(std::move(item));
        ++counter;
        return true;
    }

    template <Custom T>
    bool Array<T>::insert(std::size_t idx, std::initializer_list<T> items){
        if(counter+items.size() > m_capacity) return false;
        if(idx >= counter) throw OutOfBoundsException{"The Provided Insert Index is higher or equal to current counter\nNote: in case idx is equal to counter, one should be aware not to use insert as append"};

        std::size_t committed_items{0},
                    target_idx,
                    destination_idx;
        try{
            while((target_idx = counter-1-committed_items) != npos && target_idx >= idx){ // shifting elements
                destination_idx = target_idx+items.size();
                if(destination_idx >= counter) ::new(&arr[destination_idx]) T(std::move(arr[target_idx]));
                else arr[destination_idx] = std::move(arr[target_idx]);
                ++committed_items; 
            }
        }catch(...){
            while(committed_items > 0){
                target_idx = counter-1-(--committed_items);
                destination_idx = target_idx+items.size();
                arr[target_idx] = std::move(arr[destination_idx]);
                arr[destination_idx].~T(); // just safeguarding for append operations, it uses placement new and perhaps T is an unsafe Type 
            }throw;
        }
        committed_items = 0;
        try{ // inserting 
            for(const T& item : items){
                arr[idx+committed_items].~T();
                ::new(&arr[idx+committed_items]) T(item);
                ++committed_items;
            }
        }catch(...){
            while(committed_items > 0){
                target_idx = counter-1-(--committed_items);
                destination_idx = target_idx+items.size();
                arr[target_idx] = std::move(arr[destination_idx]);
                arr[destination_idx].~T(); // just safeguarding for append operations, it uses placement new and perhaps T is an unsafe Type 
            }throw;
        }
        counter+=items.size();
        return true;
    }

    template <Custom T>
    bool Array<T>::insert(const T& item, std::size_t idx){
        if(idx >= counter) throw OutOfBoundsException{"Out of Bounds, Provided Integer is higher or equal to the current idx\n Note: insert should never behave as append it that was the case"};
        else if(counter == m_capacity) return false;
        std::size_t committed_items{},
                    target_idx,
                    destin_idx;
        try{
            while((target_idx = counter-1-committed_items) != npos && target_idx >= idx){
                destin_idx = target_idx+1;
                if(destin_idx >= counter) ::new(&arr[destin_idx]) T(std::move(arr[target_idx]));
                else arr[destin_idx] = std::move(arr[target_idx]);
                ++committed_items;
            } arr[idx].~T();
            ::new(&arr[idx]) T(item);
        }catch(...){
            while(committed_items > 0){
                destin_idx = counter - committed_items;
                target_idx = destin_idx + 1;
                arr[destin_idx] = std::move(arr[target_idx]);
                if(target_idx = counter) arr[target_idx].~T();
                --committed_items;
            }
            throw;
        }
        ++counter;
        return true;
    }

    template <Custom T>
    bool Array<T>::insert(T&& item, std::size_t idx){
        if(idx >= counter) throw OutOfBoundsException{"Out of Bounds, Provided Integer is higher or equal to the current idx\n Note: insert should never behave as append it that was the case"};
        else if(counter == m_capacity) return false;
        std::size_t committed_items{},
                    target_idx,
                    destin_idx;
        try{
            while((target_idx = counter-1-committed_items) != npos && target_idx >= idx){
                destin_idx = target_idx+1;
                if(destin_idx >= counter) ::new(&arr[destin_idx]) T(std::move(arr[target_idx]));
                else arr[destin_idx] = std::move(arr[target_idx]);
                ++committed_items;
            }arr[idx].~T();
            ::new(&arr[idx]) T(item);
        }catch(...){
            while(committed_items > 0){
                destin_idx = counter - committed_items;
                target_idx = destin_idx + 1;
                arr[destin_idx] = std::move(arr[target_idx]);
                if(target_idx = counter) arr[target_idx].~T();
                --committed_items;
            }throw;
        }
        ++counter;
        return true;
    }

    template <Custom T>
    template <typename... Args>
    requires std::is_constructible_v<T, Args...>
    bool Array<T>::emplace(Args&&... args){
        if(counter == m_capacity) return false;
        ::new(&arr[counter]) T(std::forward<Args>(args)...);
        ++counter;
        return true;
    }

    template <Custom T>
    template <typename... Args>
    requires std::is_constructible_v<T, Args...>
    bool Array<T>::emsert(const std::size_t idx, Args&&... args){
        if(idx >= counter) throw OutOfBoundsException{"Out of Bounds, Provided Integer is higher or equal to the current idx\n Note: insert should never behave as append it that was the case"};
        else if(counter == m_capacity) return false;
        std::size_t committed_items{},
                    target_idx,
                    destin_idx;
        try{
            while((target_idx = counter-1-committed_items) != npos && target_idx >= idx){
                destin_idx = target_idx+1;
                if(destin_idx >= counter) ::new(&arr[destin_idx]) T(std::move(arr[target_idx]));
                else arr[destin_idx] = std::move(arr[target_idx]);
                ++committed_items;
            }arr[idx].~T();
            ::new(&arr[idx]) T(std::forward<Args>(args)...);
        }catch(...){
            while(committed_items > 0){
                destin_idx = counter - committed_items;
                target_idx = destin_idx + 1;
                arr[destin_idx] = std::move(arr[target_idx]);
                if(target_idx = counter) arr[target_idx].~T();
                --committed_items;
            }throw;
        }
        ++counter;
        return true;
    }

    template <Custom T>
    T& Array<T>::operator[](std::size_t idx){
        if (idx >= counter) throw OutOfBoundsException{"Index is larger than the accepted range of the contained objects"};
        return arr[idx];
    }

    template <Custom T>
    const T& Array<T>::operator[](std::size_t idx) const{
        if (idx >= counter) throw OutOfBoundsException{"Index is larger than the accepted range of the contained objects"};
        return arr[idx];
    }

    template <Custom T>
    T Array<T>::pop(){
        if(counter == 0) throw OutOfBoundsException{"Array is currenly empty, can't Pop from Empty array"};
        T temp {std::move(arr[--counter])};
        arr[counter].~T();
        return temp;
    }

    template <Custom T> 
    T Array<T>::pop(std::size_t idx){ 
        if (counter == 0) throw UnderFlowException{"Array is currently empty, can't Pop from Empty array"}; 
        if (idx >= counter) throw OutOfBoundsException{"IDX is out of bounds"};
        T temp{std::move(arr[idx])}; 
        std::size_t current_idx = idx;
        try { 
            while (current_idx < counter-1) { 
                arr[current_idx] = std::move(arr[current_idx + 1]); 
                ++current_idx; 
            }arr[current_idx].~T(); // safeguarding for append operations
        }catch (...) {
            while (current_idx > idx) {
                arr[current_idx] = std::move(arr[current_idx - 1]);
                --current_idx;
            }
            arr[idx] = std::move(temp);
            throw; 
        }
        --counter;
        return temp; // the compiler will optimize this by constructing on the callers address directly
    }

    template <Custom T>
    void Array<T>::remove(){
        if(counter == 0) throw UnderFlowException{"Array is currently empty, can't Remove from Empty array"};
        if constexpr(!std::is_trivially_destructible_v<T>) arr[counter-1].~T();
        --counter;
    }

    template <Custom T>
    void Array<T>::remove(const T& item){
        if(counter == 0) return ;
        for(std::size_t idx{}; idx < counter; ++idx){
            if (arr[idx] == item){//deletion in progress
                arr[idx] = std::move(arr[idx+1]);
                for(std::size_t j{idx+1}; j<counter-1; ++j) arr[j] = std::move(arr[j+1]);
                arr[--counter].~T();
            }
        }
    }

    template <Custom T>
    void Array<T>::remove(std::size_t idx){
        if (counter == 0) throw UnderFlowException{"Array is currently empty, can't Pop from Empty array"}; 
        if (idx >= counter) throw OutOfBoundsException{"IDX is out of bounds"};
        T temp{std::move(arr[idx])}; // just in case something goes wrong
        std::size_t current_idx = idx;
        try { 
            while (current_idx < counter-1) { 
                arr[current_idx] = std::move(arr[current_idx + 1]); 
                ++current_idx; 
            }arr[current_idx].~T(); // safeguarding for append operations
        }catch (...) {
            while (current_idx > idx) {
                arr[current_idx] = std::move(arr[current_idx - 1]);
                --current_idx;
            }
            arr[idx] = std::move(temp);
            throw; 
        }
        --counter;
    }

    // Helper Functions
    template <Custom T>
    std::size_t Array<T>::len() const noexcept{
        return counter;
    }

    template <Custom T>
    std::size_t Array<T>::len(const T& item) const noexcept{
        if (counter == 0) return 0;
        std::size_t co{};
        for(std::size_t i{}; i < counter; ++i) if(arr[i] == item) ++co;
        return co;
    }

    template <Custom T>
    std::size_t Array<T>::maxCapacity() const noexcept{
        return m_capacity;
    }

    // Utils Functions
    template <Custom T>
    std::size_t Array<T>::memory(bool flag) const noexcept{
        return (flag) ? counter*sizeof(T) : m_capacity*sizeof(T);
    }

    // Programmer Friendly Functions
    template <Custom T>
    Array<T>& Array<T>::operator+=(const T& item){
        append(item);
        return *this;
    }

    template <Custom T>
    Array<T>& Array<T>::operator+=(T&& item){
        append(std::move(item));
        return *this;
    }

    template <Custom T>
    Array<T>& Array<T>::operator-=(const T& item){
        remove(item);
        return *this;
    }

    template <Custom T>
    Array<T>::operator bool() const noexcept{
        return (counter > 0 && counter != npos) ? true : false;
    }

    // Array iterator func
    template <Custom T>
    Array<T>::iterator Array<T>::begin() noexcept{
        return iterator{arr};
    }

    template <Custom T>
    Array<T>::iterator Array<T>::end() noexcept{
        return iterator{&arr[counter]};
    }

    template <Custom T>
    Array<T>::const_iterator Array<T>::begin() const noexcept{
        return const_iterator{arr};
    }

    template <Custom T>
    Array<T>::const_iterator Array<T>::end() const noexcept{
        return const_iterator{&arr[counter]};
    }

    template <Custom T>
    Array<T>::const_iterator Array<T>::cbegin() const noexcept{
        return const_iterator{arr};
    }

    template <Custom T>
    Array<T>::const_iterator Array<T>::cend() const noexcept{
        return const_iterator{arr[counter]};
    }

    // Iterator
    template <Custom T>
    Array<T>::iterator::iterator(pointer ptr) : m_ptr(ptr){}

    template <Custom T>
    Array<T>::iterator::reference Array<T>::iterator::operator*() const noexcept{
        return *m_ptr;
    }

    template <Custom T>
    Array<T>::iterator::pointer Array<T>::iterator::operator->() const noexcept{
        return m_ptr;
    }

    template <Custom T>
    Array<T>::iterator& Array<T>::iterator::operator++() noexcept{
        m_ptr+=1;
        return *this;
    }

    template <Custom T>
    Array<T>::iterator Array<T>::iterator::operator++(int) noexcept{
        iterator temp{*this};
        ++(*this);
        return temp;
    }

    // Const Iterator
    template <Custom T>
    Array<T>::const_iterator::const_iterator(pointer ptr) : m_ptr(ptr){}

    template <Custom T>
    Array<T>::const_iterator::reference Array<T>::const_iterator::operator*() const noexcept{
        return *m_ptr;
    }

    template <Custom T>
    Array<T>::const_iterator::pointer Array<T>::const_iterator::operator->() const noexcept{
        return m_ptr;
    }

    template <Custom T>
    Array<T>::const_iterator& Array<T>::const_iterator::operator++() noexcept{
        m_ptr+=1;
        return *this;
    }

    template <Custom T>
    Array<T>::const_iterator Array<T>::const_iterator::operator++(int) noexcept{
        const_iterator temp{*this};
        ++(*this);
        return temp;
    }

} // Container Namespace
#endif
