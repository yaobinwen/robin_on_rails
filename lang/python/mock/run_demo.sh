#!/bin/sh

python -m patch_demo
python -m mock_demo

rm -f *.pyc
