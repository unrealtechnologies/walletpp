//
// Created by Ariel Saldana on 3/14/24.
//

#ifndef KEY_PAIR_H
#define KEY_PAIR_H
#include <utility>

#include "extended_key.h"


struct key_pair {
    extended_key private_key;
    extended_key public_key;

    key_pair(extended_key priv_key, extended_key pub_key) : private_key(std::move(priv_key)), public_key(std::move(pub_key)){};
};


#endif//KEY_PAIR_H
