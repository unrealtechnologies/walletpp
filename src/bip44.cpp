//
// Created by Ariel Saldana on 3/23/24.
//

#include "bip44.h"

namespace walletpp {
    auto bip44::set_purpose(const int purpose) -> void { this->purpose_ = purpose; }
    auto bip44::set_coin(coins coin) -> void { this->coin_type_ = get_coin_type(coin); }
    auto bip44::set_account(const int account) -> void { this->account_ = account; }
    auto bip44::set_change(const int change) -> void { this->change_ = change; }
    auto bip44::set_address_index(const int address_index) -> void { this->address_index_ = address_index; }
    auto bip44::get_purpose() const -> int { return this->purpose_; }
    auto bip44::get_coin() const -> coin_type { return this->coin_type_; }
    auto bip44::get_account() const -> int { return this->account_; }
    auto bip44::get_change() const -> int { return this->change_; }
    auto bip44::get_address_index() const -> int { return this->address_index_; }
    auto bip44::to_path_string() const -> std::string {
        return "m/" + std::to_string(this->purpose_) + "'/" + std::to_string(this->coin_type_.get_value()) + "'/" + std::to_string(this->account_) + "'/" +
               std::to_string(this->change_) + "/" + std::to_string(this->address_index_);
    }
    auto bip44::generate(const coins &coin, int index) -> bip44 { return bip44(coin, 0, 0, index); }

}// namespace walletpp
