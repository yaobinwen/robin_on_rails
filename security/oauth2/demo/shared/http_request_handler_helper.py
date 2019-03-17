import http.server
import re


class HTTPRequestHandlerHelper(http.server.BaseHTTPRequestHandler):

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
