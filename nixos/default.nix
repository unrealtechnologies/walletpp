{ pkgs ? import <nixpkgs> { config = { allowUnfree = true; }; } }:

let
  unstable = import <nixpkgs-unstable> {};
in

pkgs.buildEnv {
  name = "my-tools";
  paths = with pkgs; [
    gcc
    catch2
    openssl
    unstable.cmake
    unstable.gnumake
    unstable.pkg-config
    unstable.zlib
    unstable.python39
  ];
}
