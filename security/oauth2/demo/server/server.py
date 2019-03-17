# The `http.server` documentation is referred to as `DOC` in the code.
# https://docs.python.org/3/library/http.server.html


from shared.http_request_handler_helper import HTTPRequestHandlerHelper


class OAuthRequestHandler(HTTPRequestHandlerHelper):

    def do_GET(self):
        self.send_response(code=200)
        self.send_header('Content-type','text/html')
        self.end_headers()


def get_publish_address():
    return ('127.0.0.1', 8000)


def get_request_handler_class():
    return OAuthRequestHandler
