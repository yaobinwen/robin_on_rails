'''This module implements classes and functions for patch demo.
'''


from __future__ import print_function


def say_hi():
    return 'Hi~ This is A.'


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

    def introduction(self):
        return 'Hi, my name is {name}'.format(name=self._name)
