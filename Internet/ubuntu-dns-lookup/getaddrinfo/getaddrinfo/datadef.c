#include <netinet/in.h>
#include <sys/socket.h>

#include "data.h"

/* Values for `protoflag'.  */
#define GAI_PROTO_NOSERVICE 1
#define GAI_PROTO_PROTOANY  2


static const struct gaih_servtuple nullserv;

static const struct gaih_typeproto gaih_inet_typeproto[] =
{
    { 0, 0, 0, false, "" },
    { SOCK_STREAM, IPPROTO_TCP, 0, true, "tcp" },
    { SOCK_DGRAM, IPPROTO_UDP, 0, true, "udp" },
#if defined SOCK_DCCP && defined IPPROTO_DCCP
    { SOCK_DCCP, IPPROTO_DCCP, 0, false, "dccp" },
#endif
#ifdef IPPROTO_UDPLITE
    { SOCK_DGRAM, IPPROTO_UDPLITE, 0, false, "udplite" },
#endif
#ifdef IPPROTO_SCTP
    { SOCK_STREAM, IPPROTO_SCTP, 0, false, "sctp" },
    { SOCK_SEQPACKET, IPPROTO_SCTP, 0, false, "sctp" },
#endif
    { SOCK_RAW, 0, GAI_PROTO_PROTOANY|GAI_PROTO_NOSERVICE, true, "raw" },
    { 0, 0, 0, false, "" }
};
