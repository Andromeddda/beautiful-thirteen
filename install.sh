#! /usr/bin/env bash

TARGET=goodnum13

cmake -S . -B build
cmake --build build --target $TARGET -j4