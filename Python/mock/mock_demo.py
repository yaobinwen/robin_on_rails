#!/usr/bin/env python3


'''This file contains examples of using `mock.Mock`. For its documentation,
see: https://docs.python.org/3/library/unittest.mock.html#the-mock-class
This document is referred to as `DOC` in the code.
'''

import mock
import unittest


class _Person(object):
    '''Acts as the mock object spec.
    '''

    def __init__(self, name, age):
        self._name = name
        self._age = age

    def name(self):
        return self._name

    def age(self):
        return self._age


def _fail_msg_not_in_spec(attr_name):
    return '{attr_name} is not in spec. AttributeError is expected.'.format(
        attr_name=attr_name
    )


class _TestMockBase(unittest.TestCase):
    '''Base class of all the mock test cases.
    '''

    def _assertAttrNotExist(self, mock_obj, attr_name):
        try:
            getattr(mock_obj, attr_name)
            self.fail(_fail_msg_not_in_spec(attr_name))
        except AttributeError:
            pass


class TestMockSpec(_TestMockBase):
    '''Demo how to create a Mock object from a spec.
    '''

    def test_Mock_spec_None(self):
        '''No spec. Accessing any member will work.
        '''
        m = mock.Mock(spec=None)

        # Access a member. Will not raise exception.
        m.hello()
        self.assertIsInstance(m.hello, mock.Mock)
        self.assertEqual(m.hello.call_count, 1)
        self.assertEqual(m.hello.call_args, ())

        # Access a member. Will not raise exception.
        self.assertIsInstance(m.world, mock.Mock)

    def test_Mock_spec_string_list(self):
        '''Create the mock from the given list of strings.
        '''
        # m has two members.
        m = mock.Mock(['hello', 'world'])

        # '__class__' is still mock.Mock because the spec is a list of strings.
        # TODO(ywen): Why are 'm.__class__' and 'mock.Mock' not equal?
        self.assertEqual(repr(m.__class__), repr(mock.Mock))

        m.hello()
        self.assertIsInstance(m.hello, mock.Mock)
        self.assertEqual(m.hello.call_count, 1)
        self.assertEqual(m.hello.call_args, ())

        self.assertIsInstance(m.world, mock.Mock)

        self._assertAttrNotExist(m, 'my_name')
        self._assertAttrNotExist(m, 'func')

    def test_Mock_spec_class(self):
        '''Create the mock out of another object.
        '''

        # Creating the mock out of a class. The methods will be present; the
        # member variables created at run-time won't.
        m = mock.Mock(spec=_Person)

        # NOTE(ywen): m.__class__ is still '_Person' even if it is created
        # out of the class itself.
        self.assertIs(m.__class__, _Person)
        self.assertIsNot(m.__class__, _Person.__class__)

        self.assertFalse(hasattr(m, '_name'))
        self.assertFalse(hasattr(m, '_age'))
        self.assertTrue(hasattr(m, 'name'))
        self.assertIsInstance(m.name, mock.Mock)
        self.assertTrue(hasattr(m, 'age'))
        self.assertIsInstance(m.age, mock.Mock)
        self._assertAttrNotExist(m, '_name')
        self._assertAttrNotExist(m, 'hello')

    def test_Mock_spec_object(self):
        # Creating the mock out of an object. Both the methods and member
        # variables are present.
        p = _Person(name='John Doe', age=23)
        m = mock.Mock(spec=p)

        # NOTE(ywen): m is the same class as p.
        self.assertIs(m.__class__, _Person)
        self.assertIs(m.__class__, p.__class__)

        self.assertTrue(hasattr(m, '_name'))
        self.assertIsInstance(m._name, mock.Mock)
        self.assertTrue(hasattr(m, '_age'))
        self.assertIsInstance(m._age, mock.Mock)
        self.assertTrue(hasattr(m, 'name'))
        self.assertIsInstance(m.name, mock.Mock)
        self.assertTrue(hasattr(m, 'age'))
        self.assertIsInstance(m.age, mock.Mock)
        # You can't access attributes outside spec.
        self._assertAttrNotExist(m, 'hello')


class TestMockSpecSet(_TestMockBase):
    '''Demo how to create a Mock object from a spec_set.
    '''

    def test_mock_spec_set_None(self):
        '''No spec_set. Setting attributes still works.
        '''
        m = mock.Mock(spec=['hello'], spec_set=None)
        self.assertIsInstance(m.hello, mock.Mock)

        # You can't access a member outside the spec.
        self._assertAttrNotExist(m, 'world')
        # But you can still set it.
        m.world = 123
        self.assertEqual(m.world, 123)

    def test_mock_spec_set_string_list(self):
        '''Create the mock from the given list of strings.
        '''
        m = mock.Mock(spec=['hello'], spec_set=['world'])

        # 'spec_set' overrides 'spec', so 'hello' doesn't exist.
        self._assertAttrNotExist(m, 'hello')
        # 'world' exists, of course.
        self.assertIsInstance(m.world, mock.Mock)

        # However, you can't set any attributes outside 'spec_set'.
        try:
            m.hello = 123
            self.fail("'hello' cannot be set as it is outside 'spec_set'.")
        except AttributeError:
            pass

        # But you can set the attributes inside 'spec_set'.
        m.world = 456
        self.assertEqual(m.world, 456)


class TestMockSideEffect(_TestMockBase):
    '''Demo the use of side_effect.
    '''

    def test_same_func_args_as_mock(self):
        '''side_effect is called with the same arguments as the mock.
        '''
        def grow_one_year(person, *args, **kwargs):
            self.assertIs(person, p)
            self.assertTupleEqual(args, (1, 2, 3,))
            self.assertDictEqual(kwargs, {'msg1': 'hello', 'msg2': 'world'})
            person._age += 1
            return mock.DEFAULT

        p = _Person(name='John Doe', age=23)
        m = mock.Mock(side_effect=grow_one_year)
        m(p, 1, 2, 3, msg1='hello', msg2='world')

        self.assertEqual(p.age(), 24)

    def test_return_value(self):
        '''If not returning DEFAULT, side_effect return value is used as the
        mock's return value.
        '''
        def ret_1():
            return 1

        def ret_None():
            return None

        def ret_DEFAULT():
            return mock.DEFAULT

        m = mock.Mock(side_effect=ret_1)
        self.assertEqual(m(), 1)

        m = mock.Mock(side_effect=ret_None)
        self.assertIsNone(m())

        m = mock.Mock(side_effect=ret_DEFAULT)
        # The return value is not mock.DEFAULT but a new mock. See
        # 'Mock.return_value' attribute.
        self.assertIsInstance(m(), mock.Mock)

    def test_exception(self):
        '''If an exception class or object, it raises that exception.
        '''
        class MyExcept(Exception):
            pass

        m = mock.Mock(side_effect=MyExcept)
        try:
            m()
            self.fail('An exception of MyExcept is expected.')
        except MyExcept:
            pass

        m = mock.Mock(side_effect=MyExcept('error 123'))
        try:
            m()
            self.fail('An exception of MyExcept is expected.')
        except MyExcept as ex:
            self.assertIn('error 123', str(ex))

    def test_iterable(self):
        '''If an iterable object, side_effect returns the next value in each
        call.
        '''
        m = mock.Mock(side_effect=[1, 2, 3])
        self.assertEqual(m(), 1)
        self.assertEqual(m(), 2)
        self.assertEqual(m(), 3)
        self.assertRaises(StopIteration, m)

        m = mock.Mock(side_effect={'k1':1, 'k2':2})
        # NOTE(ywen): dict is iterable but the order is not intuitive. Use with
        # caution.
        self.assertEqual(m(), 'k2')
        self.assertEqual(m(), 'k1')


class TestMockReturnValue(_TestMockBase):
    def test_return_value(self):
        m = mock.Mock(return_value=1099)
        self.assertEqual(m(), 1099)

        # If given a list, `return_value` returns the whole list instead of
        # each value in the list at each call.
        m = mock.Mock(return_value=[1099])
        self.assertListEqual(m(), [1099])


if __name__ == '__main__':
    unittest.main()
