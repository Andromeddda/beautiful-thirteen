#! /usr/bin/env bash

TARGET=beautiful-thirteen

cmake -S . -B build
cmake --build build --target $TARGET -j4