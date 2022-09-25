from __future__ import (
    absolute_import, division, print_function, unicode_literals
)

from ansible.module_utils.basic import AnsibleModule

import io
import os
import re


def run_module():
    module_args = dict(
        # The path of the data file to be parsed.
        path=dict(type="path", required=True),
    )

    result = dict(
        changed=False,
        machine="",
        username="",
        password="",
    )

    module = AnsibleModule(
        argument_spec=module_args,
        supports_check_mode=True,
    )

    fpath = module.params["path"]

    if module.check_mode:
        if (os.path.isfile(fpath)):
            module.exit_json(**result)
        else:
            module.fail_json(
                msg="'{fpath}' doesn't exist".format(fpath=fpath),
                **result
            )

    try:
        with io.open(fpath, mode="r", encoding="utf-8") as fh:
            lines = fh.readlines()
            for line in lines:
                line = line.strip()
                m = re.match(r"^([a-zA-Z]+) (.+)$", line)
                if m is None:
                    raise RuntimeError("malformed file")

                key = m.group(1)
                value = m.group(2)

                if "machine" == key:
                    result["machine"] = value
                elif "username" == key:
                    result["username"] = value
                elif "password" == key:
                    result["password"] = value
                else:
                    raise RuntimeError(
                        "unrecognized key '{key}'".format(key=key)
                    )
    except Exception as ex:
        module.fail_json(msg=str(ex), **result)
    else:
        module.exit_json(**result)


def main():
    run_module()


if __name__ == '__main__':
    main()
