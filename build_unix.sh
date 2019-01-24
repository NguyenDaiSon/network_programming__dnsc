#!/bin/sh
# Nguyen Dai Son
mkdir -p ./build
cd ./build
rm -fr ./*
cmake ..
make
