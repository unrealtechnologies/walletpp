//
// Created by Ariel Saldana on 3/27/24.
//

#ifndef SECURE_VECTOR_IMP_H
#define SECURE_VECTOR_IMP_H
#include "secure_vector.h"

template<typename T>
class secure_vector_imp : public walletpp::secure_vector<T> {
public:
    // Inherit constructors from secure_vector
    using walletpp::secure_vector<T>::secure_vector;

    // New method to simulate destruction
    void simulate_destruction() { this->secure_erase(); }
};
#endif//SECURE_VECTOR_IMP_H
