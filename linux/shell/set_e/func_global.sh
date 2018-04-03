#!/bin/sh

# Test point: If we run `set -e` globally, it affects all its functions.
# Because a function is executed similarly to 'source', its exit causes the
# entire script to exit as well.

set -e

fn_print_msg_err() {
    echo ">> Function 'fn_print_msg_err': starts"
    echo "Hola"

    echo ">> Function 'fn_print_msg_err': Something goes wrong..."
    false

    echo "Mundo"

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