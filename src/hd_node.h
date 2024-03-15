//
// Created by ariel on 3/14/24.
//

#ifndef HD_NODE_H
#define HD_NODE_H
#include <utility>

#include "extended_key.h"
#include "hd_derivation.h"
#include "key_pair.h"

#include <list>
#include <unordered_map>


struct hd_node {
    key_pair k_pair;
    // std::unique_ptr<hd_node> parent;
    std::unordered_map<size_t, std::unique_ptr<hd_node>> children{};
    explicit hd_node(key_pair k_pair) : k_pair(std::move(k_pair)) {}

    hd_node* insert_child(size_t index);
};


#endif//HD_NODE_H
