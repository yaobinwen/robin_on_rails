#!/bin/sh

set -ex

# Compile the protobuf files.
protoc --cpp_out="." ./tls/v3/private_key_provider.proto
protoc --cpp_out="." ./tpm_private_key_provider_config.proto

# Build the dependent libraries.
g++ -c -fPIC -o "tls_v3_private_key_provider.o" -I. "./tls/v3/private_key_provider.pb.cc"
g++ -c -fPIC -o "tpm_private_key_provider_config.o" "tpm_private_key_provider_config.pb.cc"

# Build the C++ program.
# NOTE: The `.o` files must come before `-lprotobuf` for the correct dependency
# order. See `yaobinwen/cpp-static-dynamic-linking` for more info.
g++ -o provider_example.out \
  ./tls_v3_private_key_provider.o \
  ./tpm_private_key_provider_config.o \
  -pthread -lprotobuf -pthread \
  ./main.cpp
