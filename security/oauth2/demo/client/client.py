#!/usr/bin/env python3

# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import http.server
import re


class ClientRequestHandler(http.server.BaseHTTPRequestHandler):

    def _parse_path(self):
        m = re.match(
            pattern=r'^(\/(?:[\w\.]+\/)*(?:[\w\.]+)?)(\?(?:(?:\w+)=(?:\w+)&?)+)?(#\w+)?$',
            string=self.path,
        )
        path, query_str, frag = m.group(1, 2, 3)

        queries = None
        if query_str is not None:
            query_parts = re.split(
                # Skip '?' so we have a even number of equations.
                pattern=r'[=&]', string=query_str[1:]
            )
            queries = {}
            for i in range(0, len(query_parts), 2):
                k = query_parts[i]
                v = query_parts[i+1]
                if k not in queries:
                    queries[k] = v
                else:
                    v0 = queries[k]
                    if not isinstance(v0, list):
                        queries[k] = [v0, v]
                    else:
                        queries[k].append(v)

        return path, queries, frag

    def do_GET(self):
        path, queries, fragment = self._parse_path()
        self.send_response(code=200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        if path == '/':
            self.wfile.write(bytes('<h1>Hello, client!</h1>', 'utf8'))


def main():
    server_address = ('127.0.0.1', 8001)
    handler_cls = ClientRequestHandler
    server = http.server.HTTPServer(server_address, handler_cls)
    server.serve_forever()


if __name__ == '__main__':
    main()
