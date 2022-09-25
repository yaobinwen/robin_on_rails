#!/bin/sh

# Test point: If we run `set -e` inside a function, the parent shell will get
# affected after the function returns because, semantically, the function is
# executed quite similarly as 'source'.

fn_print_msg_err() {
    echo ">> Function 'fn_print_msg_err': starts"
    echo "Hola"

    echo ">> Function 'fn_print_msg_err': Something goes wrong..."
    false

    echo "Mundo"

    # Set in a function but has global effect.
    set -e

    echo ">> Function 'fn_print_msg_err': ends"
}

# Main procedure

echo "> Main: Starts"

# Call the function
fn_print_msg_err

echo "> Main: Something goes wrong..."
false

# This message won't be printed because of the error above.
echo "> Main: Ends"