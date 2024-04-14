//
// Created by ariel on 3/11/24.
//

#ifndef HD_DERIVATION_H
#define HD_DERIVATION_H

#include "extended_key.h"

namespace walletpp {
    class hd_derivation {
    public:
        [[nodiscard]] static auto is_hardened_key(size_t index) -> bool;
        [[nodiscard]] static auto private_child_key_derivation(const extended_key &parent_key, size_t index) noexcept -> std::unique_ptr<extended_key>;
        [[nodiscard]] static auto public_child_key_derivation(const extended_key &parent_key, size_t index) noexcept -> std::unique_ptr<extended_key>;
        [[nodiscard]] static auto private_and_public_key_pair_derivation(const extended_key &parent_key, size_t index) noexcept
                -> std::pair<std::unique_ptr<extended_key>, std::unique_ptr<extended_key>>;
    };
}// namespace walletpp


#endif//HD_DERIVATION_H
