#!/usr/bin/env python3


'''This file contains examples of using `mock.patch`. For its documentation,
see: https://docs.python.org/3/library/unittest.mock.html#the-patchers
This document is referred to as `DOC` in the code.
'''

import mock
# Because the `mock.patch` is typically used in the testing context, I'll write
# the examples as unit tests.
import unittest

import A


def _altered_say_hi_producer_producer(altered_msg):

    def _altered_say_hi_producer():
        def _altered_say_hi():
            return altered_msg
        return _altered_say_hi

    return _altered_say_hi_producer


def _altered_say_hi():
    return AlterFunction.ALTERED_MSG


class AlterFunction(unittest.TestCase):
    '''Demo how to patch a function of an imported module.
    '''

    ALTERED_MSG = "Hello, it's A here!"

    # How: Patch the target function and alter its behavior inside the method.
    #
    # `DOC` says "If patch() is used as a decorator and new is omitted, the
    # created mock is passed in as an extra argument to the decorated function."
    # Here, the 'say_hi_mock' is this extra argument. After the target is
    # mocked, you can define its return value to alter its behavior.
    @mock.patch(target='A.say_hi')
    def test_alter_say_hi_1(self, say_hi_mock):
        say_hi_mock.return_value = AlterFunction.ALTERED_MSG
        self.assertEqual(A.say_hi(), AlterFunction.ALTERED_MSG)

    # How: Patch the target function and provide a 'new'.
    #
    # When 'new' is provided, there is no extra argument passed in to the
    # decorated function.
    @mock.patch(
        target='A.say_hi',
        new=_altered_say_hi,    # Altered version of the target.
    )
    def test_alter_say_hi_2(self):
        self.assertEqual(A.say_hi(), AlterFunction.ALTERED_MSG)

    # How: Patch the target function and provide a 'new_callable'.
    #
    # In this case, the mocked target is passed in as an extra argument because
    # it is the same as the case that 'new' is not provided.
    #
    # Note that the example below uses two levels of 'producer': the producer
    # on the 'new_callable' line is called when calling the 'mock.patch' method,
    # and inside 'mock.patch' the 'new_callable' is called again to produce the
    # mocked target. This is why the 'producer_producer' must return the
    # producer that produces the mocked target.
    @mock.patch(
        target='A.say_hi',
        new_callable=_altered_say_hi_producer_producer('Even more altered'),
    )
    def test_alter_say_hi_3(self, say_hi_mock):
        self.assertEqual(A.say_hi(), 'Even more altered')


# Patching the entire 'TestCase' class is equivalent to patching every test
# method, as 'DOC' says: "Patch can be used as a TestCase class decorator. It
# works by decorating each test method in the class. This reduces the
# boilerplate code when your test methods share a common patchings set."
#
# But you can still apply additional patches to each method.
@mock.patch(
    target='A.say_hi',
    new=mock.MagicMock(return_value=1024),    # Altered version of the target.
)
class AlterFunction2(unittest.TestCase):
    '''Demo how to patch a function of an imported module by patching the
    entire test class.
    '''

    @mock.patch(
        target='A.say_goodbye',
        new=mock.MagicMock(return_value=-1),
        create=True,
    )
    def test_alter_say_hi_1(self):
        self.assertEqual(A.say_hi(), 1024)
        self.assertEqual(A.say_goodbye(), -1)

    def test_alter_say_hi_2(self):
        self.assertEqual(A.say_hi(), 1024)
        # Because this test method does not have the 'say_goodbye' decoration,
        # its mocked A does not have the 'say_goodbye' attribute.
        self.assertFalse(hasattr(A, 'say_goodbye'))

    def test_alter_say_hi_3(self):
        self.assertEqual(A.say_hi(), 1024)


class AlterModule(unittest.TestCase):
    '''Demo how to patch an imported module.
    '''

    ALTERED_MSG = "Hello, it's A here!"

    # How: Patch the module.
    #
    # When patching the module, you need to start the target with '__main__'.
    # This is because when splitting 'target', the source code assumes at least
    # one '.' is present. See:
    # https://github.com/python/cpython/blob/3.7/Lib/unittest/mock.py#L1411
    @mock.patch(target='__main__.A')
    def test_alter_A_1(self, A_mock):
        def _altered_say_hi():
            return AlterModule.ALTERED_MSG
        A_mock.say_hi = _altered_say_hi
        A_mock.say_goodbye.return_value = -1
        self.assertEqual(A.say_hi(), AlterModule.ALTERED_MSG)
        # Module A does not have 'say_goodbye' but we add one to its mock which
        # makes it seem to have this method.
        self.assertEqual(A.say_goodbye(), -1)

    # How: Create the attribute if it does not exist.
    #
    # When patching the module, we can set 'create' to 'True' so it creates the
    # attribute that doesn't exist in the target module. In the example below,
    # an 'AttributeError' is thrown if 'create' is 'False'.
    # Again, because 'new' is provided to 'mock.patch', the decorated function
    # does not take the extra argument.
    @mock.patch(
        target='__main__.A.say_goodbye',
        new=mock.MagicMock(return_value=-1),
        create=True,
    )
    def test_alter_A_2(self):
        self.assertEqual(A.say_goodbye(), -1)


if __name__ == '__main__':
    unittest.main()
