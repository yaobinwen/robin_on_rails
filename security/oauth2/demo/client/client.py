#!/usr/bin/env python3

# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import http.server


class ClientRequestHandler(http.server.BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(code=200)
        self.send_header('Content-type','text/html')
        self.end_headers()


def main():
    server_address = ('127.0.0.1', 8001)
    handler_cls = ClientRequestHandler
    server = http.server.HTTPServer(server_address, handler_cls)
    server.serve_forever()


if __name__ == '__main__':
    main()
