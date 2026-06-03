#!/usr/bin/env bash

if [ ! -d ./cmake-build-debug/ ]; then
  cd ./taller-tp-grupal
fi

PORT=5050

echo " Corriendo servidor en puerto $PORT de localhost"
echo " Ingresar 'q' cuando desees cerrar el servidor"

./cmake-build-debug/taller_server $PORT
