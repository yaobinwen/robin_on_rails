# Build a Postgres image that just creates the database (initdb) and quits.
# This allows us to bind mount /etc/passwd through to the Postgres container
# only while creating the database, rather than having to leave it mounted
# while running the production container.
#
# See https://docs.docker.com/samples/library/postgres/#arbitrary---user-notes.

ARG FROM=postgres:9.6-alpine
FROM $FROM

# The cp/cmp is to double check that sed(1) actually made a substitution.
# 2019-12-06 it turned out that the sed command had become ineffective for a
# while due to a Postgres Docker image update.
RUN cp docker-entrypoint.sh docker-entrypoint.sh.orig && \
    sed -i -e '/exec "$@"/d' docker-entrypoint.sh && \
    cp -p docker-entrypoint.sh /usr/local/bin/ && \
    (cmp -s docker-entrypoint.sh.orig docker-entrypoint.sh; test $? -eq 1) && \
    rm docker-entrypoint.sh.orig

# vim: set expandtab shiftwidth=4:
