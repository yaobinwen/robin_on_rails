#!/bin/sh

export DEBEMAIL="name@email.com"
export DEBFULLNAME="My Name"

export TMPDIR=$(pwd)
T=$(mktemp -d)
echo "Create temporary directory: $T"

cleanup() {
    test -z "$T" && return
    rm -fr "$T" || echo "WARNING: cleanup() failed" >&2
}

trap cleanup EXIT INT TERM

DSRC="src"
NAME="$(./$DSRC/demo --name)"
VER="$(./$DSRC/demo --version)"
echo "Demo program: $NAME $VER"

DSRC_FULLNAME="$NAME-$VER"

# Copy the source code.
cp -r "$DSRC" "$T/$DSRC_FULLNAME" || exit

# Create the Debian packaging template files.
cd "$T/$DSRC_FULLNAME" || exit
echo "Creating the Debian packaging template files..."
dh_make --createorig --single --yes || exit
tree $(pwd)
