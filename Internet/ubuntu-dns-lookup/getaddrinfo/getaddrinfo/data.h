#ifndef YWEN_GETADDRINFO_DATA_H
#define YWEN_GETADDRINFO_DATA_H 1

typedef unsigned char __uint8_t;

typedef int bool;   /* ??? */
#define true 1      /* ??? */
#define false 0     /* ??? */

typedef __uint8_t uint8_t;

struct gaih_typeproto
{
    int socktype;
    int protocol;
    uint8_t protoflag;
    bool defaultflag;
    char name[8];
};

struct gaih_service
{
    const char *name;
    int num;
};

struct gaih_servtuple
{
    struct gaih_servtuple *next;
    int socktype;
    int protocol;
    int port;
};

#endif
