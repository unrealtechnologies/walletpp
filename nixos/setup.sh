#!/bin/sh

# add the unstable channel
nix-channel --add https://nixos.org/channels/nixpkgs-unstable nixpkgs-unstable
nix-channel --update

# set our env
nix-env -i -f /walletpp/nixos/default.nix
