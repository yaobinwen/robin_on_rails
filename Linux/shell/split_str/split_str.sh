#!/bin/sh

STR="bla@some.com;john@home.com;Full Name <fulnam@other.org>"

C1="${STR%%;*}"
echo "STR = $STR"
echo " C1 = $C1"

STR="${STR#*;}"

C2="${STR%%;*}"
echo "STR = $STR"
echo " C2 = $C2"

STR="${STR#*;}"

C3="${STR%%;*}"
echo "STR = $STR"
echo " C3 = $C3"
