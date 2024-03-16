//
// Created by ariel on 3/14/24.
//

#include "hd_node.h"
#include "hd_derivation.h"


hd_node *hd_node::insert_child(size_t index) {
    // Check if the child already exists and return it if so
    if (const auto search = children.find(index); search != children.end()) {
        return search->second.get(); // Return existing node
    }

    // Generate a new keypair and create a new node
    auto keypair_to_insert = hd_derivation::private_and_public_key_pair_derivation(k_pair.private_key, index);
    auto node_to_insert = std::make_unique<hd_node>(keypair_to_insert);

    // Insert the new node into the map
    auto [it, inserted] = children.emplace(index, std::move(node_to_insert));

    // Return the raw pointer to the newly inserted node
    return it->second.get();
}
