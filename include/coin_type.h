//
// Created by Ariel Saldana on 3/23/24.
//

#ifndef COIN_TYPE_H
#define COIN_TYPE_H

#include <cstdint>
#include <map>
#include <string>

namespace walletpp {
    class coin_type {
        int value;
        uint8_t path_component;
        std::string symbol;
        std::string name;

    public:
        coin_type() : value(0), path_component(0) {}
        coin_type(const int value, const size_t path_component, std::string &&symbol, std::string name)
            : value(value), path_component(path_component), symbol(std::move(symbol)), name(std::move(name)) {}

        [[nodiscard]] auto get_value() const -> int { return value; }
    };

    enum coins { Bitcoin, Litecoin, Ethereum };

    inline std::map<coins, coin_type> create_coin_map() {
        std::map<coins, coin_type> coinMap;
        coinMap[Bitcoin] = coin_type(0, 0x80000000, "BTC", "Bitcoin");
        coinMap[Litecoin] = coin_type(1, 0x80000002, "LTC", "Litecoin");
        coinMap[Ethereum] = coin_type(60, 0x8000003c, "ETH", "Ethereum");

        return coinMap;
    }

    // Global map to hold the associations
    inline std::map<coins, coin_type> coin_map = create_coin_map();
    // Function to get a coin_type instance by coins
    [[nodiscard]] static auto get_coin_type(const coins id) -> coin_type { return coin_map[id]; }
}// namespace walletpp


#endif//COIN_TYPE_H
