# ============================================================
# GameEngine — Multi-stage Docker Build
# Uses Ubuntu 22.04 with SDL2 and Box2D for Linux builds
# ============================================================

# ---- Stage 1: Build ----
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    g++ \
    make \
    pkg-config \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
    libbox2d-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Create CMakeLists.txt if not present (for Linux build)
# Build the project
RUN mkdir -p build && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && cmake --build . --parallel $(nproc)

# ---- Stage 2: Runtime ----
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Install only runtime dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libsdl2-2.0-0 \
    libsdl2-image-2.0-0 \
    libsdl2-ttf-2.0-0 \
    libsdl2-mixer-2.0-0 \
    libbox2d2 \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for security
RUN useradd --create-home --shell /bin/bash gameuser
USER gameuser

WORKDIR /home/gameuser/app

# Copy built binary and assets from builder
COPY --from=builder --chown=gameuser:gameuser /app/build/GameEngine ./GameEngine
COPY --from=builder --chown=gameuser:gameuser /app/GameEngine/Assets ./Assets

# Run the game engine
ENTRYPOINT ["./GameEngine"]
