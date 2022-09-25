#!/usr/bin/env python3


'''This file contains examples of using `mock.patch`. For its documentation,
see: https://docs.python.org/3/library/unittest.mock.html#the-patchers
This document is referred to as `DOC` in the code.

unittest.mock.patch(
    target, new=DEFAULT, spec=None, create=False, spec_set=None, autospec=None,
    new_callable=None, **kwargs
)

`target`: A string of format "package.module.class_name.method_name". Note you
    need to patch in the right namespace.

`new`: The object that replaces _target_. `DEFAULT` is a MagicMock object.
`new_callable`: The class that is called to create the _new_ object. By default
    it is MagicMock.

`spec` and `spec_set`: Passed in the MagicMock.
`autospec`:
    - If `True`: Create _new_ with a spec from _target_.
    - If an object: Create _new_ with a spec from the passed in object.

`create`: If an attribute doesn't exist in _target_, create it in _new_.
'''

import six
import unittest
if six.PY2:
    import mock
else:
    import unittest.mock as mock

import A
import B
import C


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

    # How-to: Patch the entire module.
    #
    # When patching the module, you need to start the target with '__main__'.
    # This is because when splitting 'target', the source code assumes at least
    # one '.' is present. See:
    # https://github.com/python/cpython/blob/3.7/Lib/unittest/mock.py#L1411
    @mock.patch(target='__main__.A')
    def test_alter_entire_A(self, A_mock):
        def _altered_say_hi():
            return AlterModule.ALTERED_MSG

        A_mock.say_hi = _altered_say_hi
        A_mock.say_goodbye.return_value = -1
        self.assertEqual(A.say_hi(), AlterModule.ALTERED_MSG)
        # Module A does not have 'say_goodbye' but we add one to its mock which
        # makes it seem to have this method.
        self.assertEqual(A.say_goodbye(), -1)

        # Because the entire module `A` is patched, its original member class
        # `Foo` is replaced by the MagicMock class, too.
        self.assertIsInstance(A.Foo, mock.MagicMock)

    # How-to: Patch part of the module (using MagicMock).
    @mock.patch(target="__main__.A.Foo")
    def test_alter_A_Foo(self, Foo_mock):
        # Because `new` is not specified for `patch`, `target` is replaced with
        # a `MagicMock` object and is passed in as `Foo_mock` so the following
        # "isinstance" check can succeed.
        self.assertIsInstance(A.Foo, mock.MagicMock)

        # Always remember that `A.Foo` is a `MagicMock` now and we can't change
        # this fact. Therefore, instantiating it is the same as instantiating
        # `MagicMock`, and you will get another `MagicMock` object.
        self.assertIsInstance(A.Foo(), mock.MagicMock)

        # The modification to `Foo_mock` is applied to this `MagicMock` class
        # itself, not to the instances that are instantiated from it. Therefore,
        # the following `introduce` method is only accessible via `A.Foo` the
        # class, not via `A.Foo()`.
        Foo_mock.introduce.return_value = "Hello, my name is Alice."
        self.assertEqual(A.Foo.introduce(), "Hello, my name is Alice.")
        self.assertIsInstance(A.Foo().introduce(), mock.MagicMock)

        # Because we only patch `Foo`, `A.say_hi` is not affected.
        self.assertEqual(A.say_hi(), A.ORIGINAL_HI_MSG)

    # How-to: Create the attribute if it does not exist.
    def test_alter_A_2(self):
        def _say_goodbye():
            return "Goodbye!"

        # When patching the module, we can set 'create' to 'True' so it creates
        # the attribute that doesn't exist in the target module. In the example
        # below, an 'AttributeError' is thrown if 'create' is 'False'.
        with mock.patch(
            target='__main__.A.say_goodbye',
            new=_say_goodbye,
            create=True,
        ) as m:
            # The mocked target is returned as `m`. In this case, it is the
            # `_say_goodbye` function.
            self.assertEqual(m, _say_goodbye)
            self.assertEqual(A.say_goodbye(), "Goodbye!")

        # Because `create` is False, `mock.patch` will raise `AttributeError`
        # because `A` does not have `say_goodbye`. This helps prevent typos.
        attribute_error_raised = False
        try:
            # NOTE(ywen): `mock.patch` doesn't raise `AttributeError` unless
            # evaluated as a context manager. In other words, calling
            # `self.assertRaises(AttributeError, mock.patch, target=...)` or
            # `mock.patch(target=...)` won't raise `AttributeError`. That's why
            # I can't use `self.assertRaises` to test the behavior.
            with mock.patch(
                target='__main__.A.say_goodbye',
                new=_say_goodbye,
                create=False,
            ):
                self.fail("AttributeError is not raised.")
        except AttributeError:
            attribute_error_raised = True

        self.assertTrue(attribute_error_raised)


class AlterModuleInModule(unittest.TestCase):
    '''Demo how to patch an imported module inside another module.
    '''

    def test_no_patching(self):
        self.assertEqual(A.say_hi(), A.ORIGINAL_HI_MSG)
        self.assertEqual(A.introduce_B(), B.ORIGINAL_HI_MSG)
        self.assertEqual(A.say_hi_for_C(), C.ORIGINAL_HI_MSG)

    def test_patch_B_say_hi(self):
        B_mock = mock.MagicMock(return_value="Hey, B here!")

        # NOTE(ywen): You can specify the full reference to the mocking target.
        with mock.patch(target="__main__.A.B.say_hi", new=B_mock) as m1:
            self.assertIs(m1, B_mock)
            self.assertEqual(A.say_hi(), A.ORIGINAL_HI_MSG)
            self.assertEqual(A.introduce_B(), "Hey, B here!")
            self.assertEqual(A.say_hi_for_C(), C.ORIGINAL_HI_MSG)

        # NOTE(ywen): You can specify the partial reference to the mocking
        # target.
        with mock.patch(target="B.say_hi", new=B_mock) as m2:
            self.assertIs(m2, B_mock)
            self.assertEqual(A.say_hi(), A.ORIGINAL_HI_MSG)
            self.assertEqual(A.introduce_B(), "Hey, B here!")
            self.assertEqual(A.say_hi_for_C(), C.ORIGINAL_HI_MSG)

    def test_patch_C_say_hi(self):
        C_mock = mock.MagicMock(return_value="Hello, it's C!")
        with mock.patch(target="__main__.A.c_says_hi", new=C_mock) as m1:
            self.assertIs(m1, C_mock)
            self.assertEqual(A.say_hi(), A.ORIGINAL_HI_MSG)
            self.assertEqual(A.introduce_B(), B.ORIGINAL_HI_MSG)
            self.assertEqual(A.say_hi_for_C(), "Hello, it's C!")

        # NOTE(ywen): Because `C.say_hi` is imported into `A`'s namespace as
        # `c_says_hi`, patching `C.say_hi` has no effect here. Therefore,
        # `A.say_hi_for_C` doesn't return the altered message but the original
        # message.
        with mock.patch(target="C.say_hi", new=C_mock) as m2:
            self.assertIs(m2, C_mock)
            self.assertEqual(A.say_hi(), A.ORIGINAL_HI_MSG)
            self.assertEqual(A.introduce_B(), B.ORIGINAL_HI_MSG)

            # NOTE(ywen): NOT patched as expected.
            self.assertEqual(A.say_hi_for_C(), C.ORIGINAL_HI_MSG)


if __name__ == '__main__':
    unittest.main()
