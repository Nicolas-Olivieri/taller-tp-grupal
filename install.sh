#!/bin/bash

sudo apt update

# TODO revisar que son innecesarias las siguientes dependencias
# - qt6-svg-dev
# - qt6-image-formats-plugins
# - ninja-build
# - gdb
# - fluidsynth (el binario es innecesario, su librería "libfluidsynth-dev" sí es necesaria)

echo "Instalando Qt6 para el lobby del cliente"
sudo apt install -y \
    qt6-base-dev \
    qt6-svg-dev \
    qt6-image-formats-plugins

echo "Instalando herramientas de compilación"
sudo apt install -y \
    cmake \
    build-essential \
    ninja-build \
    gdb \
    pkg-config

echo "Instalando dependencias de audio SDL_mixer"
sudo apt install -y \
    libasound2-dev \
    libopus-dev \
    libopusfile-dev \
    libxmp-dev \
    libfluidsynth-dev \
    fluidsynth \
    libwavpack-dev

echo "Instalando dependencias de fuentes SDL_ttf"
sudo apt install -y libfreetype-dev