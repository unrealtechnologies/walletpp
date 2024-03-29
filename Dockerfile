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
    openssl-dev

RUN ln -sf /usr/bin/clang /usr/bin/cc \
  && ln -sf /usr/bin/clang++ /usr/bin/c++ \
  && update-alternatives --install /usr/bin/cc cc /usr/bin/clang 10\
  && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 10\
  && update-alternatives --auto cc \
  && update-alternatives --auto c++ \
  && update-alternatives --display cc \
  && update-alternatives --display c++ \
  && ls -l /usr/bin/cc /usr/bin/c++ \
  && cc --version \
  && c++ --version

COPY . /app

RUN ls -lrt /app

WORKDIR /app/build

# Run cmake to generate the Makefile
RUN cmake -DCMAKE_BUILD_TYPE=Release .. \
  && cmake --build . --parallel 8

RUN ls -lrt /app/build

RUN chmod +x /app/build/vanity-address-generator/vanity_address_generator
CMD ["./vanity-address-generator/vanity_address_generator"]
