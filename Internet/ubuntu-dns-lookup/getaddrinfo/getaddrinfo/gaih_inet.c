#include "data.h"

#include "data.c"

static int
gaih_inet(
    const char *name,   /* hostname */
    const struct gaih_service *service, /* service info (name & port) */
    const struct addrinfo *req, /* hints about how to find the address */
    struct addrinfo **pai,
    unsigned int *naddrs,
    struct scratch_buffer *tmpbuf
)
{
    const struct gaih_typeproto *tp = gaih_inet_typeproto;
    struct gaih_servtuple *st = (struct gaih_servtuple *) &nullserv;
    struct gaih_addrtuple *at = NULL;
    bool got_ipv6 = false;
    const char *canon = NULL;
    const char *orig_name = name;

    /* Reserve stack memory for the scratch buffer in the getaddrinfo
       function.  */
    size_t alloca_used = sizeof (struct scratch_buffer);

    /**
     * [ywen] Find the `gaih_typeproto` instance that best matches what `req`
     * specifies. If no such instance is found, return error (EAI_SOCKTYPE or
     * EAI_SERVICE). When this `if` is finished, `tp` points to the best match.
     */
    if (req->ai_protocol || req->ai_socktype)
    {
        ++tp;

        while (
            tp->name[0]
            && ((req->ai_socktype != 0 && req->ai_socktype != tp->socktype)
            || (req->ai_protocol != 0
                && !(tp->protoflag & GAI_PROTO_PROTOANY)
                && req->ai_protocol != tp->protocol))
        )
            ++tp;

        if (! tp->name[0])
        {
            if (req->ai_socktype)
                return -EAI_SOCKTYPE;
            else
                return -EAI_SERVICE;
        }
    }

    int port = 0;
    if (service != NULL)
    {
        if ((tp->protoflag & GAI_PROTO_NOSERVICE) != 0)
            return -EAI_SERVICE;

        if (service->num < 0)
        {
            if (tp->name[0])
            {
                st = (struct gaih_servtuple *) alloca_account (
                    sizeof (struct gaih_servtuple), alloca_used
                );

                int rc = gaih_inet_serv (service->name, tp, req, st, tmpbuf);
                if (__glibc_unlikely (rc != 0))
                    return rc;
            }
            else
            {
                struct gaih_servtuple **pst = &st;
                for (tp++; tp->name[0]; tp++)
                {
                    struct gaih_servtuple *newp;

                    if ((tp->protoflag & GAI_PROTO_NOSERVICE) != 0)
                    continue;

                    if (req->ai_socktype != 0
                        && req->ai_socktype != tp->socktype)
                    continue;
                    if (req->ai_protocol != 0
                        && !(tp->protoflag & GAI_PROTO_PROTOANY)
                        && req->ai_protocol != tp->protocol)
                    continue;

                    newp = (struct gaih_servtuple *)
                    alloca_account (sizeof (struct gaih_servtuple),
                            alloca_used);

                    if (gaih_inet_serv (service->name,
                                tp, req, newp, tmpbuf) != 0)
                    continue;

                    *pst = newp;
                    pst = &(newp->next);
                }

                if (st == (struct gaih_servtuple *) &nullserv)
                    return -EAI_SERVICE;
            }
        }
        else
        {
            port = htons (service->num);
            goto got_port;
        }
    }
    else
    {
    got_port:

        if (req->ai_socktype || req->ai_protocol)
        {
            st = alloca_account (sizeof (struct gaih_servtuple), alloca_used);
            st->next = NULL;
            st->socktype = tp->socktype;
            st->protocol = ((tp->protoflag & GAI_PROTO_PROTOANY)
                    ? req->ai_protocol : tp->protocol);
            st->port = port;
        }
        else
        {
            /* Neither socket type nor protocol is set.  Return all socket types
                we know about.  */
            struct gaih_servtuple **lastp = &st;
            for (++tp; tp->name[0]; ++tp)
                if (tp->defaultflag)
                {
                    struct gaih_servtuple *newp;

                    newp = alloca_account (sizeof (struct gaih_servtuple),
                                alloca_used);
                    newp->next = NULL;
                    newp->socktype = tp->socktype;
                    newp->protocol = tp->protocol;
                    newp->port = port;

                    *lastp = newp;
                    lastp = &newp->next;
                }
        }
    }

    bool malloc_name = false;
    struct gaih_addrtuple *addrmem = NULL;
    char *canonbuf = NULL;
    int result = 0;

    if (name != NULL)
    {
      at = alloca_account (sizeof (struct gaih_addrtuple), alloca_used);
      at->family = AF_UNSPEC;
      at->scopeid = 0;
      at->next = NULL;

        if (req->ai_flags & AI_IDN)
        {
        char *out;
        result = __idna_to_dns_encoding (name, &out);
        if (result != 0)
            return -result;
        name = out;
        malloc_name = true;
        }

        if (__inet_aton_exact (name, (struct in_addr *) at->addr) != 0)
        {
        if (req->ai_family == AF_UNSPEC || req->ai_family == AF_INET)
            at->family = AF_INET;
        else if (req->ai_family == AF_INET6 && (req->ai_flags & AI_V4MAPPED))
            {
            at->addr[3] = at->addr[0];
            at->addr[2] = htonl (0xffff);
            at->addr[1] = 0;
            at->addr[0] = 0;
            at->family = AF_INET6;
            }
        else
            {
            result = -EAI_ADDRFAMILY;
            goto free_and_return;
            }

        if (req->ai_flags & AI_CANONNAME)
            canon = name;
        }
        else if (at->family == AF_UNSPEC)
        {
            char *scope_delim = strchr (name, SCOPE_DELIMITER);
            int e;
            if (scope_delim == NULL)
                e = inet_pton (AF_INET6, name, at->addr);
            else
                e = __inet_pton_length (AF_INET6, name, scope_delim - name,
                            at->addr);

            if (e > 0)
            {
                if (req->ai_family == AF_UNSPEC || req->ai_family == AF_INET6)
                    at->family = AF_INET6;
                else if (req->ai_family == AF_INET
                    && IN6_IS_ADDR_V4MAPPED (at->addr))
                {
                at->addr[0] = at->addr[3];
                at->family = AF_INET;
                }
                else
                {
                    result = -EAI_ADDRFAMILY;
                    goto free_and_return;
                }

                if (scope_delim != NULL
                    && __inet6_scopeid_pton ((struct in6_addr *) at->addr,
                        scope_delim + 1,
                        &at->scopeid) != 0)
                {
                    result = -EAI_NONAME;
                    goto free_and_return;
                }

                if (req->ai_flags & AI_CANONNAME)
                    canon = name;
            }
        }

        if (at->family == AF_UNSPEC && (req->ai_flags & AI_NUMERICHOST) == 0)
        {
            struct gaih_addrtuple **pat = &at;
            int no_data = 0;
            int no_inet6_data = 0;
            nss_action_list nip;
            enum nss_status inet6_status = NSS_STATUS_UNAVAIL;
            enum nss_status status = NSS_STATUS_UNAVAIL;
            int no_more;
            struct resolv_context *res_ctx = NULL;

            /* If we do not have to look for IPv6 addresses or the canonical
                name, use the simple, old functions, which do not support
                IPv6 scope ids, nor retrieving the canonical name.  */
            if (req->ai_family == AF_INET
                && (req->ai_flags & AI_CANONNAME) == 0)
            {
                int rc;
                struct hostent th;
                struct hostent *h;

                while (1)
                {
                    rc = __gethostbyname2_r (name, AF_INET, &th,
                                tmpbuf->data, tmpbuf->length,
                                &h, &h_errno);
                    if (rc != ERANGE || h_errno != NETDB_INTERNAL)
                        break;
                    if (!scratch_buffer_grow (tmpbuf))
                    {
                        result = -EAI_MEMORY;
                        goto free_and_return;
                    }
                }

                if (rc == 0)
                {
                    if (h != NULL)
                    {
                        /* We found data, convert it.  */
                        if (!convert_hostent_to_gaih_addrtuple
                        (req, AF_INET, h, &addrmem))
                        {
                        result = -EAI_MEMORY;
                        goto free_and_return;
                        }
                        *pat = addrmem;
                    }
                    else
                    {
                        if (h_errno == NO_DATA)
                            result = -EAI_NODATA;
                        else
                            result = -EAI_NONAME;
                        goto free_and_return;
                    }
                }
                else
                {
                    if (h_errno == NETDB_INTERNAL)
                        result = -EAI_SYSTEM;
                    else if (h_errno == TRY_AGAIN)
                        result = -EAI_AGAIN;
                    else
                        /* We made requests but they turned out no data.
                        The name is known, though.  */
                        result = -EAI_NODATA;

                    goto free_and_return;
                }

                goto process_list;
            }

#ifdef USE_NSCD
            if (__nss_not_use_nscd_hosts > 0
                && ++__nss_not_use_nscd_hosts > NSS_NSCD_RETRY)
                __nss_not_use_nscd_hosts = 0;

            if (!__nss_not_use_nscd_hosts
                && !__nss_database_custom[NSS_DBSIDX_hosts])
            {
                /* Try to use nscd.  */
                struct nscd_ai_result *air = NULL;
                int err = __nscd_getai (name, &air, &h_errno);
                if (air != NULL)
                {
                    /* Transform into gaih_addrtuple list.  */
                    bool added_canon = (req->ai_flags & AI_CANONNAME) == 0;
                    char *addrs = air->addrs;

                    addrmem = calloc (air->naddrs, sizeof (*addrmem));
                    if (addrmem == NULL)
                    {
                        result = -EAI_MEMORY;
                        goto free_and_return;
                    }

                    struct gaih_addrtuple *addrfree = addrmem;
                    for (int i = 0; i < air->naddrs; ++i)
                    {
                        socklen_t size = (air->family[i] == AF_INET
                                ? INADDRSZ : IN6ADDRSZ);

                        if (!((air->family[i] == AF_INET
                            && req->ai_family == AF_INET6
                            && (req->ai_flags & AI_V4MAPPED) != 0)
                            || req->ai_family == AF_UNSPEC
                            || air->family[i] == req->ai_family))
                        {
                            /* Skip over non-matching result.  */
                            addrs += size;
                            continue;
                        }

                        if (*pat == NULL)
                        {
                            *pat = addrfree++;
                            (*pat)->scopeid = 0;
                        }

                        uint32_t *pataddr = (*pat)->addr;
                        (*pat)->next = NULL;

                        if (added_canon || air->canon == NULL)
                            (*pat)->name = NULL;
                        else if (canonbuf == NULL)
                        {
                            canonbuf = __strdup (air->canon);
                            if (canonbuf == NULL)
                            {
                                result = -EAI_MEMORY;
                                goto free_and_return;
                            }
                            canon = (*pat)->name = canonbuf;
                        }

                        if (air->family[i] == AF_INET
                            && req->ai_family == AF_INET6
                            && (req->ai_flags & AI_V4MAPPED))
                        {
                            (*pat)->family = AF_INET6;
                            pataddr[3] = *(uint32_t *) addrs;
                            pataddr[2] = htonl (0xffff);
                            pataddr[1] = 0;
                            pataddr[0] = 0;
                            pat = &((*pat)->next);
                            added_canon = true;
                        }
                        else if (req->ai_family == AF_UNSPEC
                            || air->family[i] == req->ai_family)
                        {
                            (*pat)->family = air->family[i];
                            memcpy (pataddr, addrs, size);
                            pat = &((*pat)->next);
                            added_canon = true;
                            if (air->family[i] == AF_INET6)
                                got_ipv6 = true;
                        }
                        addrs += size;
                    }

                    free (air);

                    if (at->family == AF_UNSPEC)
                    {
                        result = -EAI_NONAME;
                        goto free_and_return;
                    }

                    goto process_list;
                }
                else if (err == 0)
                    /* The database contains a negative entry.  */
                    goto free_and_return;
                else if (__nss_not_use_nscd_hosts == 0)
                {
                    if (h_errno == NETDB_INTERNAL && errno == ENOMEM)
                        result = -EAI_MEMORY;
                    else if (h_errno == TRY_AGAIN)
                        result = -EAI_AGAIN;
                    else
                        result = -EAI_SYSTEM;

                    goto free_and_return;
                }
            }
#endif

            no_more = !__nss_database_get (nss_database_hosts, &nip);

            /* If we are looking for both IPv4 and IPv6 address we don't
                want the lookup functions to automatically promote IPv4
                addresses to IPv6 addresses, so we use the no_inet6
                function variant.  */
            res_ctx = __resolv_context_get ();
            if (res_ctx == NULL)
                no_more = 1;

            while (!no_more)
            {
                no_data = 0;
                nss_gethostbyname4_r *fct4 = NULL;

                /* gethostbyname4_r sends out parallel A and AAAA queries and
                is thus only suitable for PF_UNSPEC.  */
                if (req->ai_family == PF_UNSPEC)
                    fct4 = __nss_lookup_function (nip, "gethostbyname4_r");

                if (fct4 != NULL)
                {
                    while (1)
                    {
                        status = DL_CALL_FCT (fct4, (name, pat,
                                    tmpbuf->data, tmpbuf->length,
                                    &errno, &h_errno,
                                    NULL));
                        if (status == NSS_STATUS_SUCCESS)
                            break;
                        if (status != NSS_STATUS_TRYAGAIN
                        || errno != ERANGE || h_errno != NETDB_INTERNAL)
                        {
                            if (h_errno == TRY_AGAIN)
                                no_data = EAI_AGAIN;
                            else
                                no_data = h_errno == NO_DATA;
                            break;
                        }

                        if (!scratch_buffer_grow (tmpbuf))
                        {
                            __resolv_context_put (res_ctx);
                            result = -EAI_MEMORY;
                            goto free_and_return;
                        }
                    }

                    if (status == NSS_STATUS_SUCCESS)
                    {
                        assert (!no_data);
                        no_data = 1;

                        if ((req->ai_flags & AI_CANONNAME) != 0 && canon == NULL)
                            canon = (*pat)->name;

                        while (*pat != NULL)
                        {
                            if ((*pat)->family == AF_INET
                                && req->ai_family == AF_INET6
                                && (req->ai_flags & AI_V4MAPPED) != 0)
                            {
                                uint32_t *pataddr = (*pat)->addr;
                                (*pat)->family = AF_INET6;
                                pataddr[3] = pataddr[0];
                                pataddr[2] = htonl (0xffff);
                                pataddr[1] = 0;
                                pataddr[0] = 0;
                                pat = &((*pat)->next);
                                no_data = 0;
                            }
                            else if (req->ai_family == AF_UNSPEC
                                || (*pat)->family == req->ai_family)
                            {
                                pat = &((*pat)->next);

                                no_data = 0;
                                if (req->ai_family == AF_INET6)
                                    got_ipv6 = true;
                            }
                            else
                                *pat = ((*pat)->next);
                        }
                    }

                    no_inet6_data = no_data;
                }
                else
                {
                    nss_gethostbyname3_r *fct = NULL;
                    if (req->ai_flags & AI_CANONNAME)
                        /* No need to use this function if we do not look for
                        the canonical name.  The function does not exist in
                        all NSS modules and therefore the lookup would
                        often fail.  */
                        fct = __nss_lookup_function (nip, "gethostbyname3_r");
                    if (fct == NULL)
                        /* We are cheating here.  The gethostbyname2_r
                        function does not have the same interface as
                        gethostbyname3_r but the extra arguments the
                        latter takes are added at the end.  So the
                        gethostbyname2_r code will just ignore them.  */
                        fct = __nss_lookup_function (nip, "gethostbyname2_r");

                    if (fct != NULL)
                    {
                        if (req->ai_family == AF_INET6
                        || req->ai_family == AF_UNSPEC)
                        {
                        gethosts (AF_INET6);
                        no_inet6_data = no_data;
                        inet6_status = status;
                        }
                        if (req->ai_family == AF_INET
                        || req->ai_family == AF_UNSPEC
                        || (req->ai_family == AF_INET6
                            && (req->ai_flags & AI_V4MAPPED)
                            /* Avoid generating the mapped addresses if we
                            know we are not going to need them.  */
                            && ((req->ai_flags & AI_ALL) || !got_ipv6)))
                        {
                        gethosts (AF_INET);

                        if (req->ai_family == AF_INET)
                            {
                            no_inet6_data = no_data;
                            inet6_status = status;
                            }
                        }

                        /* If we found one address for AF_INET or AF_INET6,
                        don't continue the search.  */
                        if (inet6_status == NSS_STATUS_SUCCESS
                        || status == NSS_STATUS_SUCCESS)
                        {
                            if ((req->ai_flags & AI_CANONNAME) != 0
                                && canon == NULL)
                            {
                                canonbuf = getcanonname (nip, at, name);
                                if (canonbuf == NULL)
                                {
                                    __resolv_context_put (res_ctx);
                                    result = -EAI_MEMORY;
                                    goto free_and_return;
                                }
                                canon = canonbuf;
                            }
                            status = NSS_STATUS_SUCCESS;
                        }
                        else
                        {
                            /* We can have different states for AF_INET and
                                AF_INET6.  Try to find a useful one for both.  */
                            if (inet6_status == NSS_STATUS_TRYAGAIN)
                                status = NSS_STATUS_TRYAGAIN;
                            else if (status == NSS_STATUS_UNAVAIL
                                && inet6_status != NSS_STATUS_UNAVAIL)
                                status = inet6_status;
                        }
                    }
                    else
                    {
                        /* Could not locate any of the lookup functions.
                        The NSS lookup code does not consistently set
                        errno, so we need to supply our own error
                        code here.  The root cause could either be a
                        resource allocation failure, or a missing
                        service function in the DSO (so it should not
                        be listed in /etc/nsswitch.conf).  Assume the
                        former, and return EBUSY.  */
                        status = NSS_STATUS_UNAVAIL;
                        __set_h_errno (NETDB_INTERNAL);
                        __set_errno (EBUSY);
                    }
                }

                if (nss_next_action (nip, status) == NSS_ACTION_RETURN)
                    break;

                nip++;
                if (nip->module == NULL)
                    no_more = -1;
            }

            __resolv_context_put (res_ctx);

            /* If we have a failure which sets errno, report it using
                EAI_SYSTEM.  */
            if ((status == NSS_STATUS_TRYAGAIN || status == NSS_STATUS_UNAVAIL)
                && h_errno == NETDB_INTERNAL)
            {
                result = -EAI_SYSTEM;
                goto free_and_return;
            }

            if (no_data != 0 && no_inet6_data != 0)
            {
                /* If both requests timed out report this.  */
                if (no_data == EAI_AGAIN && no_inet6_data == EAI_AGAIN)
                    result = -EAI_AGAIN;
                else
                    /* We made requests but they turned out no data.  The name
                    is known, though.  */
                    result = -EAI_NODATA;

                goto free_and_return;
            }
        }

        process_list:
        if (at->family == AF_UNSPEC)
        {
            result = -EAI_NONAME;
            goto free_and_return;
        }
    }
    else
    {
        struct gaih_addrtuple *atr;
        atr = at = alloca_account (sizeof (struct gaih_addrtuple), alloca_used);
        memset (at, '\0', sizeof (struct gaih_addrtuple));

        if (req->ai_family == AF_UNSPEC)
        {
            at->next = __alloca (sizeof (struct gaih_addrtuple));
            memset (at->next, '\0', sizeof (struct gaih_addrtuple));
        }

        if (req->ai_family == AF_UNSPEC || req->ai_family == AF_INET6)
        {
            at->family = AF_INET6;
            if ((req->ai_flags & AI_PASSIVE) == 0)
                memcpy (at->addr, &in6addr_loopback, sizeof (struct in6_addr));
            atr = at->next;
        }

        if (req->ai_family == AF_UNSPEC || req->ai_family == AF_INET)
        {
            atr->family = AF_INET;
            if ((req->ai_flags & AI_PASSIVE) == 0)
                atr->addr[0] = htonl (INADDR_LOOPBACK);
        }
    }

    {
        struct gaih_servtuple *st2;
        struct gaih_addrtuple *at2 = at;
        size_t socklen;
        sa_family_t family;

        /*
        buffer is the size of an unformatted IPv6 address in printable format.
        */
        while (at2 != NULL)
        {
            /* Only the first entry gets the canonical name.  */
            if (at2 == at && (req->ai_flags & AI_CANONNAME) != 0)
            {
                if (canon == NULL)
                    /* If the canonical name cannot be determined, use
                    the passed in string.  */
                    canon = orig_name;

                bool do_idn = req->ai_flags & AI_CANONIDN;
                if (do_idn)
                {
                    char *out;
                    int rc = __idna_from_dns_encoding (canon, &out);
                    if (rc == 0)
                        canon = out;
                    else if (rc == EAI_IDN_ENCODE)
                        /* Use the punycode name as a fallback.  */
                        do_idn = false;
                    else
                    {
                        result = -rc;
                        goto free_and_return;
                    }
                }

                if (!do_idn)
                {
                    if (canonbuf != NULL)
                        /* We already allocated the string using malloc, but
                            the buffer is now owned by canon.  */
                        canonbuf = NULL;
                    else
                    {
                        canon = __strdup (canon);
                        if (canon == NULL)
                        {
                        result = -EAI_MEMORY;
                        goto free_and_return;
                        }
                    }
                }
            }

            family = at2->family;
            if (family == AF_INET6)
            {
                socklen = sizeof (struct sockaddr_in6);

                /* If we looked up IPv4 mapped address discard them here if
                the caller isn't interested in all address and we have
                found at least one IPv6 address.  */
                if (got_ipv6
                && (req->ai_flags & (AI_V4MAPPED|AI_ALL)) == AI_V4MAPPED
                && IN6_IS_ADDR_V4MAPPED (at2->addr))
                goto ignore;
            }
            else
                socklen = sizeof (struct sockaddr_in);

            for (st2 = st; st2 != NULL; st2 = st2->next)
            {
                struct addrinfo *ai;
                ai = *pai = malloc (sizeof (struct addrinfo) + socklen);
                if (ai == NULL)
                {
                    free ((char *) canon);
                    result = -EAI_MEMORY;
                    goto free_and_return;
                }

                ai->ai_flags = req->ai_flags;
                ai->ai_family = family;
                ai->ai_socktype = st2->socktype;
                ai->ai_protocol = st2->protocol;
                ai->ai_addrlen = socklen;
                ai->ai_addr = (void *) (ai + 1);

                /* We only add the canonical name once.  */
                ai->ai_canonname = (char *) canon;
                canon = NULL;

#ifdef _HAVE_SA_LEN
                ai->ai_addr->sa_len = socklen;
#endif /* _HAVE_SA_LEN */
                ai->ai_addr->sa_family = family;

                /* In case of an allocation error the list must be NULL
                terminated.  */
                ai->ai_next = NULL;

                if (family == AF_INET6)
                {
                    struct sockaddr_in6 *sin6p =
                    (struct sockaddr_in6 *) ai->ai_addr;

                    sin6p->sin6_port = st2->port;
                    sin6p->sin6_flowinfo = 0;
                    memcpy (&sin6p->sin6_addr,at2->addr, sizeof (struct in6_addr));
                    sin6p->sin6_scope_id = at2->scopeid;
                }
                else
                {
                    struct sockaddr_in *sinp = (struct sockaddr_in *) ai->ai_addr;
                    sinp->sin_port = st2->port;
                    memcpy (&sinp->sin_addr, at2->addr, sizeof (struct in_addr));
                    memset (sinp->sin_zero, '\0', sizeof (sinp->sin_zero));
                }

                pai = &(ai->ai_next);
            }

            ++*naddrs;

            ignore:
            at2 = at2->next;
        }
    }

    free_and_return:
    if (malloc_name)
        free ((char *) name);
    free (addrmem);
    free (canonbuf);

    return result;
}
