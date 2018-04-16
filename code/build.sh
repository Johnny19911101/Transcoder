#!/bin/bash
cd src
GRPCCPPDIR=$(which "grpc_cpp_plugin")
protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` transcode_rpc.proto
protoc --cpp_out=. ./transcode_rpc.proto
cd ..
rm -rf build
mkdir build
cd build
cmake ..
make