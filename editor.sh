#!/usr/bin/env bash

if [ ! -d ./cmake-build-debug/ ]; then
  cd ./taller-tp-grupal
fi

./cmake-build-debug/taller_editor -platform xcb
