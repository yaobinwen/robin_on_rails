#!/usr/bin/env python3

# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import argparse
import http.server
import importlib.util as imp
import os.path


def _syntax():
    parser = argparse.ArgumentParser(description='A general server')
    parser.add_argument(
        '-m', '--module-path', required=True,
        help='Path to the Python module to load',
    )
    return parser


def main(module_path=None):
    module_name = os.path.basename(module_path)[:-len('.py')]

    # NOTE(ywen): Refer to "Importing a source file directly"
    # https://docs.python.org/3.6/library/importlib.html#importing-a-source-file-directly
    spec = imp.spec_from_file_location(module_name, module_path)
    module = imp.module_from_spec(spec)
    spec.loader.exec_module(module)

    address = module.get_publish_address()
    handler_cls = module.get_request_handler_class()
    server = http.server.HTTPServer(address, handler_cls)
    server.serve_forever()


if __name__ == '__main__':
    main(**vars(_syntax().parse_args()))
