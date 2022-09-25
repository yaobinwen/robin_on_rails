#!/usr/bin/python3
import os.path
def func():
    if os.path.exists("/tmp/data.txt"):
        fh = open("/tmp/data.txt", "rb")
        data = fh.read()
        print(str.upper(data))

func()
