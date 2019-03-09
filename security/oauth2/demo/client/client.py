#!/usr/bin/env python3

# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import http.server
import jinja2
import re


# The global Jinja2 Template ENVironment.
T_ENV = jinja2.Environment(
    loader=jinja2.PackageLoader('client', 'templates'),
    autoescape=jinja2.select_autoescape(['html'])
)

# OAuth 2.0 arguments' default values
SERVER_RESPONSE_TYPE = 'code'
CLIENT_ID = 'client_1'
CLIENT_STATE = 'ABCDEFG'    # TODO(ywen): Generate random string


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
            html = T_ENV.get_template('index.html').render(
                server_url_port='127.0.0.1:8000',
                response_type=SERVER_RESPONSE_TYPE,
                client_id=CLIENT_ID,
                state=CLIENT_STATE,
            )
            self.wfile.write(bytes(html, 'utf8'))


def main():
    server_address = ('127.0.0.1', 8001)
    handler_cls = ClientRequestHandler
    server = http.server.HTTPServer(server_address, handler_cls)
    server.serve_forever()


if __name__ == '__main__':
    main()
