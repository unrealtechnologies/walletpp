//
// Created by Ariel Saldana on 3/26/24.
// PLEASE DO NOT USE THIS WITH A T VALUE OTHER THAN STD::STRING AND UINT8_T
// I HAVE NOT TESTED IT WITH OTHER TYPES
//

#ifndef SECURE_VECTOR_H
#define SECURE_VECTOR_H


#include <algorithm>
#include <vector>

namespace walletpp {
    template<typename T>
    class secure_vector {
    public:
        using value_type = T;
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;

        // Default constructor
        secure_vector() = default;

        // Initialize with initializer list
        secure_vector(std::initializer_list<T> init) : vec(init.begin(), init.end()) {}

        // Initialize with size
        explicit secure_vector(size_t size) : vec(size) {}

        // Explicitly defined copy constructor
        secure_vector(const secure_vector &other) : vec(other.vec) {}

        // Move constructor
        secure_vector(secure_vector &&other) noexcept : vec(std::move(other.vec)) {}

        secure_vector &operator=(const secure_vector &other) = default;


        // Range constructor
        template<typename InputIterator>
        secure_vector(InputIterator first, InputIterator last) : vec(first, last) {}

        // Destructor
        ~secure_vector() { secure_erase(); }

        // Other necessary vector operations can be delegated to vec...
        [[nodiscard]] auto size() const noexcept -> size_t { return vec.size(); }

        T *data() noexcept { return vec.data(); }
        const T *data() const noexcept { return vec.data(); }

        T &operator[](size_t idx) {
            if (idx >= vec.size()) throw std::out_of_range("Index out of range");
            return vec[idx];
        }

        const T &operator[](size_t idx) const {
            if (idx >= vec.size()) throw std::out_of_range("Index out of range");
            return vec[idx];
        }

        // Adding begin and end member functions
        iterator begin() noexcept { return vec.begin(); }
        const_iterator begin() const noexcept { return vec.begin(); }
        const_iterator cbegin() const noexcept { return vec.cbegin(); }

        iterator end() noexcept { return vec.end(); }
        const_iterator end() const noexcept { return vec.end(); }
        const_iterator cend() const noexcept { return vec.cend(); }

        // Insert a single element at a specific position
        iterator insert(const_iterator pos, const T &value) { return vec.insert(pos, value); }

        // Insert a single element at a specific position (move version)
        iterator insert(const_iterator pos, T &&value) { return vec.insert(pos, std::move(value)); }

        // Insert elements from an initializer list at a specific position
        iterator insert(const_iterator pos, std::initializer_list<T> ilist) { return vec.insert(pos, ilist); }

        // Insert elements from a range [first, last) at a specific position
        template<typename InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
            return vec.insert(pos, first, last);
        }

        // Insert 'count' copies of 'value' at position 'pos'
        iterator insert(const_iterator pos, size_t count, const T &value) { return vec.insert(pos, count, value); }

        // Erase a single element at a specific position
        iterator erase(const_iterator pos) { return vec.erase(pos); }

        // Erase elements in the range [first, last)
        iterator erase(const_iterator first, const_iterator last) { return vec.erase(first, last); }

        // Request a change in capacity
        void reserve(size_t new_cap) { vec.reserve(new_cap); }

        template<typename... Args>
        void emplace_back(Args &&...args) {
            vec.emplace_back(std::forward<Args>(args)...);
        }

    private:
        std::vector<T> vec;

    protected:
        void secure_erase() {
            // Check if T is std::string, requires #include <type_traits> and <string>
            if constexpr (std::is_same_v<T, std::string>) {
                for (auto &str: vec) {
                    // Using volatile pointer to attempt to prevent optimization of the write loop
                    volatile char *p = const_cast<char *>(str.data());
                    std::size_t len = str.size();
                    while (len--) { *p++ = 0; }
                    str.clear();// Deallocate the string's memory
                }
            } else {
                // General case for other types
                if (!vec.empty()) {
                    // Note: The use of volatile is removed since memset_s should not be optimized away.
                    memset_s(const_cast<T *>(vec.data()), vec.size() * sizeof(T), 0, vec.size() * sizeof(T));
                }
            }
            vec.clear();// Clear the vector after securely erasing its content
        }
    };
}// namespace walletpp


#endif//SECURE_VECTOR_H
