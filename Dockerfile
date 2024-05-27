FROM alpine:edge

RUN apk update \
  && apk upgrade \
  && apk add --no-cache \
    clang \
    clang-dev \
    alpine-sdk \
    dpkg \
    cmake \
    ccache \
    python3 \
    openssl-dev \
    libc++

RUN ln -sf /usr/bin/clang /usr/bin/cc \
  && ln -sf /usr/bin/clang++ /usr/bin/c++ \
  && update-alternatives --install /usr/bin/cc cc /usr/bin/clang 10 \
  && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 10 \
  && update-alternatives --auto cc \
  && update-alternatives --auto c++ \
  && update-alternatives --display cc \
  && update-alternatives --display c++ \
  && ls -l /usr/bin/cc /usr/bin/c++ \
  && cc --version \
  && c++ --version

COPY . /app

WORKDIR /app/build

# Ensure C++ standard is set
RUN cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release .. \
  && cmake --build . --parallel 8

RUN chmod +x /app/build/vanity-address-generator/vanity_address_generator

# Define a volume for the /app/data directory to persist data
VOLUME /app/data

CMD ["./vanity-address-generator/vanity_address_generator"]
