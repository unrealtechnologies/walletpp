//
// Created by ariel on 3/14/24.
//

#ifndef HD_NODE_H
#define HD_NODE_H

#include "extended_key.h"
#include <memory>
#include <unordered_map>
#include <utility>

namespace walletpp {
    class hd_node : public std::enable_shared_from_this<hd_node> {
        std::unordered_map<size_t, std::shared_ptr<hd_node>> children{};
        std::unique_ptr<extended_key> private_key;
        std::unique_ptr<extended_key> public_key;
        std::weak_ptr<hd_node> parent;

    public:
        // this constructor explicitly only allows for moving key_pair
        explicit hd_node(std::unique_ptr<extended_key> priv_key, std::unique_ptr<extended_key> pub_key, std::weak_ptr<hd_node> parent)
            : private_key(std::move(priv_key)), public_key(std::move(pub_key)), parent(std::move(parent)) {}

        std::shared_ptr<hd_node> derive_child(size_t index);
        auto remove_child(size_t index) -> void;
        auto get_key_pair() const -> std::pair<extended_key, extended_key>;
    };
}// namespace walletpp


#endif//HD_NODE_H
