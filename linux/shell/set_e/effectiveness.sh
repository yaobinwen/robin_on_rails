#!/bin/sh

# Test point: When '-e' is actually effective.
#
# See this section:
# http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_25
# The '-e' part tells the situations in which '-e' does not cause the script
# to exit immediately. The code below shows one of them, which is "...or any
# command of an AND-OR list other than the last."

# Main procedure

echo "> Main: Starts"

# Subshell: Test multi-command pipeline
(
    (
        set -e
        false | true | false | true
        echo ">> Multi-command pipeline: Last command succeeded."
    )
    (
        set -e
        false | true | false
        echo ">> Multi-command pipeline: Last command failed."
    )
)

# Subshell: Test AND-OR list
(
    set -e

    echo ">> Subshell: Starts"

    # Although 'false' is run, it is not the last command of this OR list, so
    # '-e' doesn't cause the script to exit.
    echo ">> Subshell: Something goes wrong: 'false || true'"
    false || true

    # Although 'false' is the last command of this OR list, it is not run because
    # 'true' has made the entire statement True, so '-e' doesn't cause the script
    # to exit.
    echo ">> Subshell: Something goes wrong: 'true || false'"
    true || false

    # The same as 'false || true': Not the last command in the OR list.
    echo ">> Subshell: Something goes wrong: 'false && true'"
    false && true

    # This causes the script to exit immediately.
    echo ">> Subshell: Something goes wrong: 'true && false'"
    true && false

    echo ">> Subshell: Ends"
)

# Subshell:
(
    echo ">> Subshell: Starts"

    # 
    (
        set -e
        echo ">>> Hello"
        false
        echo ">>> World"
    ) || (echo ">>> Inner subshell failed in the middle." && exit)

    (
        set -e
        echo ">>> Hello again"
        echo ">>> World"
        false
    ) || (echo ">>> Inner subshell's last command failed." && exit)

    (
        set -e
        echo ">>> Hola"
        echo ">>> Algo va mal..."
        false
        echo "Mundo"
    )
    test $? -eq 0 || exit

    echo ">> Subshell: Ends"
)

# This message won't be printed because of the error above.
echo "> Main: Ends"