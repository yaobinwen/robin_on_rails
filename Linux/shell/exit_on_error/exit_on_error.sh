#!/bin/sh

# This script demonstrates how to exit on error.
# We don't want to always use `set -e` which errors out on every failure because
# we prefer finer manipulation to the "error conditions". In some cases, we may
# not error out at all.

ADDR=$(./testee.py --message "http://localhost:3000" success) || {
    echo "testee 'http://localhost:3000' failed! Exiting..."
    exit
}

# This line should be printed.
echo "Address from testee: $ADDR"

ADDR=$(./testee.py --message "http://localhost:4000" failure) || {
    echo "testee 'http://localhost:4000' failed! Exiting..."
    exit
}

# This line should not be printed.
echo "Address from testee: $ADDR"
