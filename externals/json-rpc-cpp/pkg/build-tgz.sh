#!/bin/bash

OUTPUT_DIR=libjson-rpc-cpp

mkdir $OUTPUT_DIR
echo "Fetching all together"
cp ../*.h $OUTPUT_DIR
cp ../*.cpp $OUTPUT_DIR
cp ../makefile* $OUTPUT_DIR
cp -r ../json $OUTPUT_DIR
cp -r ../connectors $OUTPUT_DIR

echo "Building .tar.gz"
tar --exclude ".*" -czf libjson-rpc-cpp-src.tar.gz $OUTPUT_DIR

echo "Cleaning up"
rm -rf $OUTPUT_DIR
