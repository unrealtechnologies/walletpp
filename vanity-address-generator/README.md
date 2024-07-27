# Vanity Address Generator

# Dependencies

- OpenSSL

# Instructions to run

In order to run this application you will need to generate a private / public key pair for decrypting / encrypting the 
generated encrypted binary data.

To generate a private key run the following command
```sh
openssl genpkey -algorithm RSA -out private_key.pem -aes256
```

To generate the corresponding private key run the following command
```sh
openssl rsa -pubout -in private_key.pem -out public_key.pem
```


Run application. Note: public_key is a required parameter
```sh 
./vanity-address-generator --public_key /path/to/public_key.pem
```
