# ------------------------------
# Stage 1: Build
# ------------------------------
FROM debian:trixie AS builder
RUN apt-get update && apt-get install -y \
    build-essential cmake pkg-config git \
    libpqxx-dev libssl-dev zlib1g-dev curl \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /opt
RUN git clone --depth=1 https://github.com/brainboxdotcc/DPP.git dpp \
    && cd dpp \
    && cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --target install
WORKDIR /app
COPY . .
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --target organizr

# ------------------------------
# Stage 2: Runtime
# ------------------------------
FROM debian:trixie-slim
RUN apt-get update && apt-get install -y \
    libpqxx-7.10 libssl3t64 zlib1g \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/local/bin
COPY --from=builder /app/build/organizr .
COPY --from=builder /usr/local/lib/libdpp.so* /usr/local/lib/
RUN ldconfig
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
ENTRYPOINT ["organizr"]
