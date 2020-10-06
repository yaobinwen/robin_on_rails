#!/bin/sh

# Demonstrate how to replace a string with the value of a variable.
#
# Task: We want to replace the substring "localhost:32778" in `db-uri` with
# the new value in ${NEW_ADDR}, but all the other parts of `db-uri` should
# remain unchanged.
NEW_ADDR="new_addr:39294"

# Firstly, create the old pattern of the text to be replaced.
# We use single quotes to avoid string interpolation, i.e., using the string
# literal.
#
# Notes:
# - 1). The regex pattern is: '\s*db-uri\s*=\s*(.+)@.+:[0-9]+(.*)'.
# - 2). However, because `sed` uses POSIX BRE and POSIX BRE doesn't support
#   the "one or more qulifier +" [1], we need to use "..*" to emulate it.
# - 3). We must escape the parentheses
OLD_PATTERN='[[:blank:]]*db-uri[[:blank:]]*=[[:blank:]]*\(..*\)@..*:[0-9][0-9]*\(..*\)'
NEW_PATTERN="db-uri = \1@${NEW_ADDR}\2"
sed "s/${OLD_PATTERN}/${NEW_PATTERN}/g" ./original.txt

# References:
# - [1] [9.3.3 BRE Special Characters](https://pubs.opengroup.org/onlinepubs/009696899/basedefs/xbd_chap09.html#tag_09_03_03)
