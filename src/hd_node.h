//
// Created by ariel on 3/14/24.
//

#ifndef HD_NODE_H
#define HD_NODE_H
#include <utility>

#include "extended_key.h"
#include "key_pair.h"


struct hd_node {
    std::string path;
    key_pair k_pair;
    std::unique_ptr<hd_node> parent;
    Botan::secure_vector<std::unique_ptr<uint8_t>> children{};
    hd_node(std::string path, key_pair k_pair, std::unique_ptr<hd_node> parent = nullptr) : path(std::move(path)), k_pair(std::move(k_pair)), parent(std::move(parent)) {}
};


#endif//HD_NODE_H
