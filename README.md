# WalletPP

WalletPP stands for Wallet++ and is a collection of tools for creating cryptocurrency wallets.

## Building image for vanity address generator

```bash
docker build . -t walletpp/vanity-address-generator --progress=plain --no-cache
docker run -i -t docker.io/walletpp/vanity-address-generator
```
