#!/bin/sh

# Test point: If we run `set -e` inside a subshell, the parent shell won't get
# affected after the subshell returns.

# Main procedure

echo "> Main: Starts"

# Run the subshell
(
    echo ">> Subshell: Starts"

    # A local 'set -e'
    set -e
    
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