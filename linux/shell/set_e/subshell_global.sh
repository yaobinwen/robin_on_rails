#!/bin/sh

# Test point: If we run `set -e` globally, both the parent shell and subshell
# get affected.

set -e

# Main procedure

echo "> Main: Starts"

echo "> Main: 'set' flags: '$-'"

# Run the subshell
(
    echo ">> Subshell: Starts"

    echo ">> Subshell: 'set' flags: '$-'"

    echo "Hola"

    # Something goes wrong...
    echo ">> Subshell: Something goes wrong..."
    false

    # Won't print this message because of 'set -e'.
    echo "Mundo"

    echo ">> Subshell: Ends"
)

# Something goes wrong. But the parent shell won't exit because 'set -e' is
# only effective inside the subshell.
echo "> Main: Something goes wrong..."
false

# Print final message
echo "> Main: Ends"