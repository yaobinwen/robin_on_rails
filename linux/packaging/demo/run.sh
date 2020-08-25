#!/bin/sh

set -x

export DEBEMAIL="name@email.com"
export DEBFULLNAME="My Name"

export TMPDIR=$(pwd)
T=$(mktemp -d)
: "Create temporary directory: $T"

cleanup() {
    test -z "$T" && return
    rm -fr "$T" || echo "WARNING: cleanup() failed" >&2
}

trap cleanup EXIT INT TERM

O=$(pwd)
DSRC="src"
NAME="$(./$DSRC/demo --name)"
VER="$(./$DSRC/demo --version)"
: "Demo program: $NAME $VER"

DSRC_FULLNAME="$NAME-$VER"

# Copy the source code.
cp -r "$DSRC" "$T/$DSRC_FULLNAME" || exit

# ==============================
# Step 1: Create the Debian packaging template files.
# See: https://www.debian.org/doc/manuals/maint-guide/first.en.html (2.8)
cd "$T/$DSRC_FULLNAME" || exit
: "PWD: $(pwd)"
dh_make --createorig --single --yes || exit

# See all the current content.
tree "$T" || exit

# ==============================
# Step 2: Handle the template files.
# See: https://www.debian.org/doc/manuals/maint-guide/dother.en.html
(
    cd debian
    "$O/rules.py" --enable-verbose ./rules || exit
    cat ./rules
    mv manpage.1.ex "$NAME.1" || exit
    mv postinst.ex postinst || exit
    mv postrm.ex postrm || exit
    mv preinst.ex preinst || exit
    mv prerm.ex prerm || exit
    # NOTE(ywen): The ".install" file must use the package name as the main
    # file name. It should not use the target file ("demo" in this case) as
    # the file name.
    echo "demo usr/bin" > "$NAME.install"
) || exit

# See all the current content.
tree "$T" || exit

# ==============================
# Step 3: Build the package.
# See: https://www.debian.org/doc/manuals/maint-guide/build.en.html
: "PWD: $(pwd)"
dpkg-buildpackage -us -uc

# See all the current content.
tree "$T" || exit

# See the content of the ".deb" file. There should be exactly one such file.
# NOTE(ywen): To include the executable files, library files, and header files
# correctly in the Debian package, the source code Makefile must handle building
# and installing correctly.
DEB_FPATH="$(find $T -name "$NAME*$VER*.deb")"
dpkg --contents $DEB_FPATH
