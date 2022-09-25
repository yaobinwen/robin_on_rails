'''This module implements classes and functions for patch demo.
'''


import B
from C import say_hi as c_says_hi


ORIGINAL_HI_MSG = "Hi~ This is A."


def say_hi():
    return ORIGINAL_HI_MSG


def introduce_B():
    return B.say_hi()


def say_hi_for_C():
    return c_says_hi()


class Foo(object):

    def __init__(self, name='foo'):
        self._name = name

    def __repr__(self):
        return (
            '<Foo('
            'name={name}'
            ')>'.format(
                name=self._name
            )
        )

    def self_introduce(self):
        return 'Hi, my name is {name}.'.format(name=self._name)
