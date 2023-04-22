#include <netdb.h>

#include "data.h"

/**
 * [ywen] This is a `static` function in the real code.
 */
/* static */ int
gaih_inet_serv (const char *servicename, const struct gaih_typeproto *tp,
        const struct addrinfo *req, struct gaih_servtuple *st,
        struct scratch_buffer *tmpbuf)
{
  struct servent *s;
  struct servent ts;
  int r;

  do
    {
      r = __getservbyname_r (servicename, tp->name, &ts,
                 tmpbuf->data, tmpbuf->length, &s);
      if (r != 0 || s == NULL)
    {
      if (r == ERANGE)
        {
          if (!scratch_buffer_grow (tmpbuf))
        return -EAI_MEMORY;
        }
      else
        return -EAI_SERVICE;
    }
    }
  while (r);

  st->next = NULL;
  st->socktype = tp->socktype;
  st->protocol = ((tp->protoflag & GAI_PROTO_PROTOANY)
          ? req->ai_protocol : tp->protocol);
  st->port = s->s_port;

  return 0;
}
