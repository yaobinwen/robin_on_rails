#!/usr/bin/python3


def F(*args, **kwargs):
    print(type(args))
    print(args)
    print(type(kwargs))
    print(kwargs)


F(1, 2, 3, a=10, b=20)
