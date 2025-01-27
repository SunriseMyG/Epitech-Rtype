FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    python3 \
    python3-pip \
    build-essential \
    tzdata \
    pkg-config \
    libudev-dev \
    sudo \
    && rm -rf /var/lib/apt/lists/*

USER root

WORKDIR /app
COPY . .

RUN chmod +x /app/build.sh

CMD ["./build.sh"]
