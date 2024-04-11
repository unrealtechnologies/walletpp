//
// Created by ariel on 3/14/24.
//

#include "hd_node.h"
#include "hd_derivation.h"

namespace walletpp {
    auto hd_node::derive_child(size_t index) -> std::shared_ptr<hd_node> {
        // Check if the child already exists and return it if so
        if (const auto search = children.find(index); search != children.end()) {
            return search->second;// Return existing node
        }

        // Generate a new keypair and create a new node
        auto [priv_key, pub_key] = hd_derivation::private_and_public_key_pair_derivation(*this->private_key.get(), index);
        auto node_to_insert = std::make_shared<hd_node>(std::move(priv_key), std::move(pub_key), shared_from_this());

        // Insert the new node into the map
        auto [it, inserted] = children.emplace(index, std::move(node_to_insert));

        // Return the raw pointer to the newly inserted node
        return it->second;
    }

    auto hd_node::remove_child(const size_t index) -> void {
        if (const auto it = children.find(index); it != children.end()) { children.erase(it); }
    }

    auto hd_node::get_key_pair() const -> std::pair<extended_key, extended_key> { return std::make_pair(*private_key.get(), *public_key.get()); }
}// namespace walletpp
