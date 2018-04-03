#!/bin/sh

# Test point: How quotes work in shell

# Main procedure

echo "> Main: Starts"

person="hatter"

echo "> Print the values of variable evaluation"
echo $person
echo "$person"
echo \$person
echo '$person'
echo "'$person'"
echo ~/something
echo "~/something"
echo '~/something'

# Print final message
echo "> Main: Ends"