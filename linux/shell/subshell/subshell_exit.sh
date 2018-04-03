#!/bin/sh

# Test point: Exiting from subshell doesn't cause the parent shell to exit.

# Main procedure

echo "> Main: Starts"

# Run the subshell
(
    echo ">> Subshell: Starts"

    # set -e    # NOTE: 'set -e' wouldn't cause the parent shell to exit.

    echo "Hola"

    # Something goes wrong...
    echo ">> Subshell: Something goes wrong..."
    false || exit

    # Won't print this message because of 'set -e'.
    echo "Mundo"

    echo ">> Subshell: Ends"
) || exit

# Print final message
echo "> Main: Ends"