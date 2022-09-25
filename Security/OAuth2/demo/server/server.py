# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import jinja2
import urllib.parse
import uuid

from shared.http_request_handler_helper import HTTPRequestHandlerHelper


# The global Jinja2 Template ENVironment.
T_ENV = jinja2.Environment(
    loader=jinja2.PackageLoader('server', 'templates'),
    autoescape=jinja2.select_autoescape(['html'])
)


# Current request. For simplicity, we don't consider the case of multiple
# requests.
REQUEST = {}


class OAuthRequestHandler(HTTPRequestHandlerHelper):

    def do_GET(self):
        global REQUEST

        r = urllib.parse.urlparse(self.path)
        q = urllib.parse.parse_qs(r.query)

        status_code = 200
        headers = {
            'Content-type': 'text/html',
        }
        html = ''
        if r.path == '/oauth/authorize':
            html = T_ENV.get_template('authorize.html').render()
            # For simplicity, we don't check if client ID already exists.
            # We also simply store the queries into the request, even the
            # client ID is not needed.
            REQUEST = q
        elif r.path == '/oauth/authorize/grant':
            status_code = 302
            grant_code = uuid.uuid4().hex.upper()
            grant_url = (
                '{redirect_uri}?{query_str}'.format(
                    redirect_uri=REQUEST['redirect_uri'][0],
                    query_str=urllib.parse.urlencode(
                        query={
                            'auth_request': 'granted',
                            'grant_code': grant_code,
                            'state': REQUEST['state'][0]
                        },
                        quote_via=urllib.parse.quote,
                    ),   # grant_code and client_state
                )
            )
            headers['Location'] = grant_url
        elif r.path == '/oauth/authorize/deny':
            status_code = 302
            deny_url = (
                '{redirect_uri}?{query_str}'.format(
                    redirect_uri=REQUEST['redirect_uri'][0],
                    query_str=urllib.parse.urlencode(
                        query={
                            'auth_request': 'denied',
                        },
                        quote_via=urllib.parse.quote,
                    )
                )
            )
            headers['Location'] = deny_url
        else:
            html = self.return_404(T_ENV)

        self.send_response(code=status_code)
        for k, v in headers.items():
            self.send_header(k, v)
        self.end_headers()
        self.wfile.write(bytes(html, 'utf8'))


def get_publish_address():
    return ('127.0.0.1', 8000)


def get_request_handler_class():
    return OAuthRequestHandler
