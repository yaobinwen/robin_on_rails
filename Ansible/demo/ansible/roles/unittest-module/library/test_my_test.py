# -*- mode: python; coding: UTF-8; -*-

from __future__ import absolute_import, division, print_function, unicode_literals

import ansible.module_utils.basic as AnsibleModuleBasic
import six
import unittest

if six.PY2:
    import mock
elif six.PY3:
    import unittest.mock as mock
else:
    raise RuntimeError("Unrecognized Python version")

import my_test


class MyAnsibleModuleExit(SystemExit):
    """Raised by `MyAnsibleModule.exit_json`.
    """

    def __init__(self, code, args, message, exit_json_kwargs):
        super(MyAnsibleModuleExit, self).__init__(code)
        self.args = args
        self.message = message
        self.exit_json_kwargs = exit_json_kwargs

    def __str__(self):
        return "{name}: args={args}; message={message}; exit_json_kwargs={exit_json_kwargs}".format(
            name=self.__class__.__name__,
            args=self.args,
            message=self.message,
            exit_json_kwargs=self.exit_json_kwargs,
        )


class MyAnsibleModuleFail(SystemExit):
    """Raised by `MyAnsibleModule.fail_json`.
    """

    def __init__(self, code, args, message, fail_json_kwargs):
        super(MyAnsibleModuleFail, self).__init__(code)
        self.args = args
        self.message = message
        self.fail_json_kwargs = fail_json_kwargs

    def __str__(self):
        return "{name}: args={args}; message={message}; fail_json_kwargs={fail_json_kwargs}".format(
            name=self.__class__.__name__,
            args=self.args,
            message=self.message,
            fail_json_kwargs=self.fail_json_kwargs,
        )


class MyAnsibleModule(AnsibleModuleBasic.AnsibleModule):
    """Subclass `AnsibleModule` so we can customize the behavior of `exit_json`
    and `fail_json` (and all the customization if needed).
    """

    @staticmethod
    def set_module_args(args=None):
        # NOTE(ywen): I could have use `AnsibleModuleBasic` but that makes this
        # method depend on the developer to import `ansible.module_utils.basic`
        # in a specific way. So I'd rather import and use it locally.
        import ansible.module_utils.basic as ansible_module_basic
        import json

        if args is None:
            args = {}

        module_args = {"ANSIBLE_MODULE_ARGS": args}
        ansible_module_basic._ANSIBLE_ARGS = json.dumps(module_args)

    def __init__(self, argument_spec, **kwargs):
        super(MyAnsibleModule, self).__init__(argument_spec, **kwargs)

    def exit_json(self, **kwargs):
        try:
            super(MyAnsibleModule, self).exit_json(**kwargs)
        except SystemExit as ex:
            raise MyAnsibleModuleExit(
                code=ex.code, args=ex.args, message=ex.message, exit_json_kwargs=kwargs
            )

    def fail_json(self, **kwargs):
        try:
            super(MyAnsibleModule, self).fail_json(**kwargs)
        except SystemExit as ex:
            raise MyAnsibleModuleFail(
                code=ex.code, args=ex.args, message=ex.message, fail_json_kwargs=kwargs
            )


class Test_my_test(unittest.TestCase):
    @mock.patch(target="my_test.AnsibleModule", new=MyAnsibleModule)
    def test_real_mode_success(self):
        original_message = "This is the original message"

        try:
            MyAnsibleModule.set_module_args(
                {
                    "name": original_message,
                    "new": True,
                    "_ansible_check_mode": False,
                }
            )
            my_test.main()
        except MyAnsibleModuleExit as ex:
            exit_json_kwargs = ex.exit_json_kwargs
            self.assertTrue(exit_json_kwargs["changed"])
            self.assertNotIn("msg", exit_json_kwargs)
            self.assertEqual(exit_json_kwargs["original_message"], original_message)
            self.assertEqual(exit_json_kwargs["message"], "goodbye")
        except MyAnsibleModuleFail as ex:
            self.fail(msg=str(ex))

    @mock.patch(target="my_test.AnsibleModule", new=MyAnsibleModule)
    def test_real_mode_failure(self):
        original_message = "fail me"

        try:
            MyAnsibleModule.set_module_args(
                {
                    "name": original_message,
                    "new": False,
                    "_ansible_check_mode": False,
                }
            )
            my_test.main()
        except MyAnsibleModuleExit as ex:
            self.fail(msg=str(ex))
        except MyAnsibleModuleFail as ex:
            fail_json_kwargs = ex.fail_json_kwargs
            self.assertFalse(fail_json_kwargs["changed"])
            self.assertEqual(fail_json_kwargs["msg"], "You requested this to fail")
            self.assertEqual(fail_json_kwargs["original_message"], original_message)
            self.assertEqual(fail_json_kwargs["message"], "goodbye")

    @mock.patch(target="my_test.AnsibleModule", new=MyAnsibleModule)
    def test_check_mode(self):
        original_message = "This is the original message"

        try:
            MyAnsibleModule.set_module_args(
                {
                    "name": original_message,
                    "new": False,
                    "_ansible_check_mode": True,
                }
            )
            my_test.main()
        except MyAnsibleModuleExit as ex:
            exit_json_kwargs = ex.exit_json_kwargs
            self.assertFalse(exit_json_kwargs["changed"])
            self.assertEqual(exit_json_kwargs["original_message"], "")
            self.assertEqual(exit_json_kwargs["message"], "")
        except MyAnsibleModuleFail as ex:
            self.fail(msg=str(ex))



if __name__ == "__main__":
    unittest.main()

# vim: set expandtab shiftwidth=4 filetype=python:
