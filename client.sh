#!/usr/bin/env bash

echo " Abriendo el launcher..."

if [ ! -d ./cmake-build-debug/ ]; then
  cd ./taller-tp-grupal
fi

./cmake-build-debug/taller_client -platform xcb
