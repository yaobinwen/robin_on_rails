import http.server


class HTTPRequestHandlerHelper(http.server.BaseHTTPRequestHandler):

    def return_404(self, t_env):
        return t_env.get_template('404.html').render()
