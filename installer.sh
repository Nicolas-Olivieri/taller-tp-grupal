#!/usr/bin/env bash

set -e

REPO_URL="https://github.com/Nicolas-Olivieri/taller-tp-grupal"
PROJECT_DIR="taller-tp-grupal"

echo " Actualizando repositorios"

sudo apt update

echo " Instalando Qt6"

sudo apt install -y qt6-base-dev

echo " Instalando herramientas de compilación"

sudo apt install -y \
  git \
  cmake \
  build-essential \
  ninja-build \
  pkg-config

echo " Instalando dependencias SDL_mixer"

sudo apt install -y \
  libasound2-dev \
  libopus-dev \
  libopusfile-dev \
  libxmp-dev \
  libfluidsynth-dev \
  fluidsynth \
  libwavpack-dev

echo " Instalando dependencias SDL_ttf"

sudo apt install -y libfreetype-dev

echo " Clonando repositorio"

if [ ! -d "$PROJECT_DIR" ]; then
  git clone "$REPO_URL" "$PROJECT_DIR"
else
  cd "$PROJECT_DIR"
  git pull
  cd ..
fi

cd "$PROJECT_DIR"

echo " Configurando CMake"

cmake -S . -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug

echo " Compilando el juego"

cmake --build cmake-build-debug -j$(nproc)

cp ./client.sh ../.
cp ./server.sh ../.
cp ./editor.sh ../.

echo "========= Instalación finalizada ========="
echo " Inicie el Servidor con ./server.sh"
echo " Comience a jugar con ./client.sh"
echo " Crea nuevos mundos para jugar con ./editor.sh"

