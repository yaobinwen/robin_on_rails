# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import jinja2

from shared.http_request_handler_helper import HTTPRequestHandlerHelper


# The global Jinja2 Template ENVironment.
T_ENV = jinja2.Environment(
    loader=jinja2.PackageLoader('server', 'templates'),
    autoescape=jinja2.select_autoescape(['html'])
)


class OAuthRequestHandler(HTTPRequestHandlerHelper):

    def do_GET(self):
        path, queries, fragment = self._parse_path()
        self.send_response(code=200)
        self.send_header('Content-type','text/html')
        self.end_headers()
        if path == '/oauth/authorize':
            html = T_ENV.get_template('authorize.html').render()
            self.wfile.write(bytes(html, 'utf8'))


def get_publish_address():
    return ('127.0.0.1', 8000)


def get_request_handler_class():
    return OAuthRequestHandler
