# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


import jinja2
import urllib.parse
import uuid

from shared.http_request_handler_helper import HTTPRequestHandlerHelper


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

# Session data.
SESSION = {}


class ClientRequestHandler(HTTPRequestHandlerHelper):

    def do_GET(self):
        r = urllib.parse.urlparse(self.path)
        q = urllib.parse.parse_qs(r.query)

        status_code = 200
        headers = {
            'Content-type': 'text/html',
        }
        html = ''
        if r.path == '/' or r.path == '/index.html':
            logged_in = None
            auth_request_denied = None
            if 'auth_request' in q:
                # This is the reply of the authorization request.
                req_result = q['auth_request'][0]
                if req_result == 'granted':
                    # TODO(ywen): Request the access token.
                    pass
                elif req_result == 'denied':
                    auth_request_denied = True
                else:
                    # TODO(ywen): The error case.
                    pass
            else:
                logged_in = 'access_token' in SESSION

            html = T_ENV.get_template('index.html').render(
                logged_in=logged_in,
                auth_request_denied=auth_request_denied
            )
        elif r.path == '/login':
            status_code = 302
            server_auth_url = (
                '{scheme}://{server_url_port}/oauth/authorize?'
                'response_type={response_type}&'
                'client_id={client_id}&'
                'state={client_state}&'
                'redirect_uri={redirect_uri}'.format(
                    scheme='http',
                    server_url_port='127.0.0.1:8000',
                    response_type=SERVER_RESPONSE_TYPE,
                    client_id=CLIENT_ID,
                    client_state=CLIENT_STATE,
                    # TODO(ywen): Encode the URI.
                    # NOTE(ywen): "In order to be secure, the redirect URL must
                    # be an https endpoint to prevent the code from being
                    # intercepted during the authorization process."
                    redirect_uri=urllib.parse.quote(
                        'http://127.0.0.1:8001'
                    ),
                )
            )
            headers['Location'] = server_auth_url
        else:
            html = self.return_404(T_ENV)

        # NOTE(ywen): The response must be written in the following order:
        # 1). Status code.
        # 2). Headers.
        # 3). Body.
        self.send_response(code=status_code)
        for k, v in headers.items():
            self.send_header(k, v)
        self.end_headers()
        self.wfile.write(bytes(html, 'utf8'))


def get_publish_address():
    return ('127.0.0.1', 8001)


def get_request_handler_class():
    return ClientRequestHandler
