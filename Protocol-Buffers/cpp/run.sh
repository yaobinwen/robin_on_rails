#!/bin/sh

set -ex

./provider_example.out write ./tpm_provider_output.json
./provider_example.out read ./tpm_provider_input.json

./read_struct_example.out ./tpm_provider_input_struct.json
