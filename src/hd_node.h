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

    hd_node* insert_child(const size_t index) {
        // Check if the child already exists and return it if so
        auto search = children.find(index);
        if (search != children.end()) {
            return search->second.get(); // Return existing node
        }

        // Generate a new keypair and create a new node
        auto keypair_to_insert = hd_derivation::private_and_public_key_pair_derivation(k_pair.private_key, index);
        auto node_to_insert = std::make_unique<hd_node>(keypair_to_insert);

        // Store the raw pointer before moving the unique_ptr into the map
        hd_node* raw_ptr = node_to_insert.get();

        // Insert the new node into the map
        children.emplace(index, std::move(node_to_insert));

        // Return the raw pointer to the newly inserted node
        return raw_ptr;
    }
};


#endif//HD_NODE_H
