#ifndef YWEN_GETADDRINFO_FUNC_DECL_H
#define YWEN_GETADDRINFO_FUNC_DECL_H 1

int
gaih_inet_serv(
    const char *servicename,
    const struct gaih_typeproto *tp,
    const struct addrinfo *req,
    struct gaih_servtuple *st,
    struct scratch_buffer *tmpbuf
);

#endif
