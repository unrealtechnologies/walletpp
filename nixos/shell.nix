{ pkgs ? import <nixpkgs> {} }:

let
  unstable = import <nixpkgs-unstable> {};
in

pkgs.mkShell {
  name = "shell-dummy";
  buildInputs = with pkgs; [
    (import ./default.nix { inherit pkgs; })
    openssl
    autoconf
    unstable.cmake
    pkg-config
  ];

  shellHook = ''
      export OPENSSL_INCLUDE_DIR=$(pkg-config --variable=includedir openssl)
      export OPENSSL_CRYPTO_LIBRARY=${pkgs.openssl.dev}/../lib/libcrypto.so
      export OPENSSL_SSL_LIBRARY=${pkgs.openssl.dev}/../lib/libssl.so
  '';
}
