#!/usr/bin/env bash

mkdir -p build
pushd build
cmake -DCMAKE_TOOLCHAIN_FILE=$PLAYDATE_SDK_PATH/C_API/buildsupport/arm.cmake ..
make
popd