//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef BIP32_H
#define BIP32_H

#include "extended_key.h"
#include "key_pair.h"

#include <array>
#include <cstddef>

class bip32 {
    key_pair from_base_58(const std::string_view &key);
    key_pair derive_path(const std::string_view &path);
};


#endif//BIP32_H
