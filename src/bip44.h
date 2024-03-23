//
// Created by Ariel Saldana on 3/23/24.
//

#ifndef BIP44_H
#define BIP44_H

#include "coin_type.h"
#include <string>

namespace walletpp {
    class bip44 {
        static constexpr auto bip44_purpose = 44;

        int purpose_;
        coin_type coin_type_;
        int account_;
        int change_;
        int address_index_;

    public:
        bip44() : purpose_(bip44_purpose), coin_type_(get_coin_type(Bitcoin)), account_(0), change_(0), address_index_(0) {}
        bip44(const coins coin_type, const int account, const int change, const int address_index)
            : purpose_(bip44_purpose), coin_type_(get_coin_type(coin_type)), account_(account), change_(change), address_index_(address_index) {}

        auto set_purpose(int purpose) -> void;
        auto set_coin(coins coin) -> void;
        auto set_account(int account) -> void;
        auto set_change(int change) -> void;
        auto set_address_index(int address_index) -> void;
        [[nodiscard]] auto get_purpose() const -> int;
        [[nodiscard]] auto get_account() const -> int;
        [[nodiscard]] auto get_coin() const -> coin_type;
        [[nodiscard]] auto get_change() const -> int;
        [[nodiscard]] auto get_address_index() const -> int;

        [[nodiscard]] static auto generate(const coins &coin, int index = 0) -> bip44;
        [[nodiscard]] auto to_path_string() const -> std::string;
    };
}// namespace walletpp


#endif//BIP44_H
