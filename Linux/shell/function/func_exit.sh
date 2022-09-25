#!/bin/sh

# Test point: Exiting in a function causes its parent to exit, too.

# Main procedure

echo "> Main: Starts"

# Run the anonymous function
{
    echo ">> Function: Starts"

    echo "Hola"

    # Something goes wrong...
    echo ">> Subshell: Something goes wrong..."
    false || exit

    # Won't print the following messages because the function exits.
    echo "Mundo"

    echo ">> Function: Ends"
}

echo "> Main: Ends"