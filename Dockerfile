# Use NixOS as the base image
FROM nixos/nix

WORKDIR /walletpp

# Copy your application files into the container
COPY src/ ./src
COPY include/ ./include
COPY thirdparty ./thirdparty
COPY CMakeLists.txt .
COPY cmake/ ./cmake
COPY vanity-address-generator/ ./vanity-address-generator
COPY nixos/ ./nixos

# add the unstable channel
RUN nix-channel --add https://nixos.org/channels/nixpkgs-unstable nixpkgs-unstable
RUN nix-channel --update

# set our env
RUN nix-env -i -f /walletpp/nixos/default.nix

WORKDIR /walletpp/build

RUN source /root/.nix-profile/etc/profile.d/nix.sh && \
    nix-shell /walletpp/nixos/shell.nix --command "\
    echo \$OPENSSL_INCLUDE_DIR && \
    which openssl && \
    echo \$OPENSSL_INCLUDE_DIR && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 8"

WORKDIR /walletpp/build/vanity-address-generator
RUN chmod +x vanity_address_generator

CMD ["./vanity_address_generator"]
