#!/bin/sh

# Test point: How quotes work in shell

# Main procedure

echo "> Main: Starts"

person="hatter"

echo "> Print the values of variable evaluation"
echo "1)." $person
echo "2)." "$person"
echo "3)." \$person
echo "4)." '$person'
echo "5)." "'$person'"
echo "6)." ~/something
echo "7)." "~/something"
echo "8)." '~/something'

# Nested double-quotation marks.
echo "9)." "$(echo name is "$person")"

# Print final message
echo "> Main: Ends"
