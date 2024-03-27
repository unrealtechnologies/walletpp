//
// Created by Ariel Saldana on 3/26/24.
//

#ifndef SECURE_VECTOR_H
#define SECURE_VECTOR_H


#include <algorithm>
#include <cstring>
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

        // Range constructor
        template<typename InputIterator>
        secure_vector(InputIterator first, InputIterator last) : vec(first, last) {}

        // Destructor
        ~secure_vector() {
            //Securely zero out memory
            for (auto &item: vec) { std::memset(&item, 0, sizeof(T)); }
            vec.clear();
        }

        // Other necessary vector operations can be delegated to vec...
        [[nodiscard]] auto size() const noexcept -> size_t { return vec.size(); }

        T *data() noexcept { return vec.data(); }
        const T *data() const noexcept { return vec.data(); }

        auto simulate_destruction() -> void {
            // for (auto &item: vec) { std::memset(&item, 0, sizeof(T)); }
            // vec.clear();
            if constexpr (std::is_same_v<T, std::string>) {
                // Explicitly zero out the heap memory used by each string.
                for (auto &str: vec) {
                    if (!str.empty()) {
                        std::fill(std::begin(str), std::end(str), 0);
                        str.clear();
                    }
                }
            } else {
                std::fill_n(vec.begin(), vec.size(), 0);
            }
            vec.clear();
        }

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

    private:
        std::vector<T> vec;
    };
}// namespace walletpp


#endif//SECURE_VECTOR_H
