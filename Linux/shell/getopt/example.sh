#!/bin/bash

# -----------------------------------------------------------------------------
# References
# - [1] getopt(1)
# - [2] [2. Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
#   - eval
#   - set
#   - shift

# -----------------------------------------------------------------------------
# Examples:
#
# - 1). ./example.sh -a -c candy
# - 2). Specify a required input value: because `charlie` requires an input
#   value, both `--charlie candy` and `--charlie=candy` can work:
#   - 2a). ./example.sh --charlie candy
#   - 2b). ./example.sh --charlie=candy
# - 3). Specify an option with an optional input value: because the input value
#   is optional, one must use `--dave=doodle` to provide the value.
#   `--dave doodle` would be considered `--dave` with no input value and
#   `doodle` being the next positional argument.
#   - 3a). ./example.sh --dave=doodle

# -----------------------------------------------------------------------------
# Define the short and long options.

# `:` means the option requires a value.
# `::` means the option does not require a value.

## Define the short, one-char options. Because every short option is one
## character, there is no need to use `,` to separate them.
OPTIONS="ac:h"

## Define the long options. `,` is used to separate the long options.
LONGOPTS="ada,bob,charlie:,dave::,help"

DAVE_DEFAULT="DAVID"

# -----------------------------------------------------------------------------
# Parse the options
echo "               Original: $@"

PARSED=$(getopt --options=$OPTIONS --longoptions=$LONGOPTS --name "$0" -- "$@")
echo "                 PARSED: $PARSED"
echo "Original (after getopt): $@"

# -----------------------------------------------------------------------------
# Set the parsed results as the new CLI options.
eval set -- "$PARSED"
echo "CLI options (after set): $@"

# -----------------------------------------------------------------------------
# Display help if needed.
while true; do
  case "$1" in
    -h|--help)
      echo "Help"
      exit 0
      ;;
    *)
      if [ -n "$1" ]; then
        shift
      else
        break
      fi
      ;;
  esac
done

# -----------------------------------------------------------------------------
# Set the parsed results as the new CLI options (again).
eval set -- "$PARSED"
echo "CLI options (after set): $@"

# -----------------------------------------------------------------------------
# Process the options and arguments
while true; do
  # "$1" is the first token after the program name (i.e., '$0').
  case "$1" in
    -a|--ada)
      echo "Option -a/--ada"
      # This option does not require additional input, so only need to shift by
      # one token.
      shift
      ;;
    -b|--bob)
      echo "Option -b/--bob"
      # This option does not require additional input, so only need to shift by
      # one token.
      shift
      ;;
    -c|--charlie)
      echo "Option -c/--charlie with argument: $2"
      # This option requires one additional input argument, so we need to shift
      # by two tokens.
      shift 2
      ;;
    -d|--dave)
      if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
        dave_value="$2"
        shift 2
      else
        dave_value="$DAVE_DEFAULT"
        shift
      fi
      echo "Option -d/--dave with argument: ${dave_value}"
      ;;
    --)
      shift
      # After `--` are the positional arguments that we are going to display in
      # the next `for` loop.
      break
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

# Process remaining positional arguments
for arg; do
  echo "Positional argument: $arg"
done
