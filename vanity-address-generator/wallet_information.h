//
// Created by Ariel Saldana on 7/27/24.
//

#ifndef WALLET_INFORMATION_H
#define WALLET_INFORMATION_H

#include <iostream>
#include <string>
#include <format>

struct wallet_information {
private:
    std::string address;
    std::string address_private_key;
    std::string address_public_key;
    std::string mnemonic_words;
    std::string mnemonic_passphrase;
    std::string bip_39_seed;

public:
    wallet_information(const std::string &address, const std::string &address_private_key, const std::string &address_public_key, const std::string &mnemonic_words,
                       const std::string &mnemonic_passphrase, const std::string &bip_39_seed) {
        this->address = address;
        this->address_private_key = address_private_key;
        this->address_public_key = address_public_key;
        this->mnemonic_words = mnemonic_words;
        this->mnemonic_passphrase = mnemonic_passphrase;
        this->bip_39_seed = bip_39_seed;
    }

    std::string to_json_string() {
        return std::format(
                R"({{"address":"{}", "address_private_key":"{}", "address_public_key": "{}", "mnemonic_words": "{}", "mnemonic_passphrase": "{}", "bip_39_seed": "{}"}})", address,
                address_private_key, address_public_key, mnemonic_words, mnemonic_passphrase, bip_39_seed);
    }
};

#endif //WALLET_INFORMATION_H
