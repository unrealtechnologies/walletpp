//
// Created by Ariel Saldana on 3/26/24.
//

#include "secure_vector.h"

template<typename T>
void walletpp::secure_vector<T>::secure_erase() {
    if constexpr (std::is_same<T, std::string>::value) {
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
            std::fill_n(vec.data(), vec.size(), 0);
        }
    }
    vec.clear();// Clear the vector after securely erasing its content
}
