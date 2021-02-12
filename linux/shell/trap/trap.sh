#!/bin/sh

action_1() {
  echo "Action 1"
}

action_2() {
  echo "Action 2"
}

echo "Checkpoint 1"
trap action_1 EXIT INT TERM

echo "Checkpoint 2"
# Overrides `action_1`.
trap action_2 EXIT INT TERM

echo "Checkpoint 3"
