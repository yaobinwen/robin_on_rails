import unittest

import sandbox.sandbox as sandbox
import sandbox.sandbox_pb2 as sandbox_bp2


class TestImport(unittest.TestCase):
    def test_import(self):
        import sandbox.sandbox


class Test_to_str(unittest.TestCase):
    def test_to_str(self):
        ids = [1, 2, 10]
        r = sandbox_bp2.SandboxRequest()
        r.ids.extend(ids)
        s = sandbox.to_str(r)
        self.assertIn(str(ids), s)
