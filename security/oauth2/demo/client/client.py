#!/usr/bin/env python3

# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import http.server
import jinja2
import re
import uuid


# The global Jinja2 Template ENVironment.
T_ENV = jinja2.Environment(
    loader=jinja2.PackageLoader('client', 'templates'),
    autoescape=jinja2.select_autoescape(['html'])
)

# OAuth 2.0 arguments' default values
SERVER_RESPONSE_TYPE = 'code'
CLIENT_ID = 'client_1'

# NOTE(ywen): We want a random string for the state. This thread has discussed
# how to implement it in depth: https://stackoverflow.com/q/2257441/630364
# After reading all the responses, I think a UUID fits the need here well
# because, first, I don't need cryptographical security, and, second, I don't
# have the limit of using only upper case letter and digits. The hyphens in a
# UUID string still works for me.
CLIENT_STATE = uuid.uuid4().hex.upper()


class ClientRequestHandler(http.server.BaseHTTPRequestHandler):

    def _parse_path(self):
        m = re.match(
            # NOTE(ywen): This pattern considers the following cases:
            # /
            # /path.ico
            # /path
            # /path/to/file
            # /path/to/file
            # /path/to1/
            #
            # /p/t?a=1
            # /p/t?a=1&b=2&c=3
            #
            # /p/t#frag
            # /p/t?a=1#frag
            # /p/t?a=1&b=2#frag
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
                client_state=CLIENT_STATE,
            )
            self.wfile.write(bytes(html, 'utf8'))


def get_publish_address():
    return ('127.0.0.1', 8001)


def get_request_handler_class():
    return ClientRequestHandler
