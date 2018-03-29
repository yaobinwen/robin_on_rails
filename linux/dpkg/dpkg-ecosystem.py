#!/usr/bin/env python
# -*- mode: python; coding: UTF-8; -*-


import argparse
import datetime
import jinja2
import subprocess32


_DEPENDENCIES = {
    'dpkg': set([
        'dpkg-deb',
    ]),

    'dpkg-deb': None,
}


def _syntax():
    parser = argparse.ArgumentParser(
        description='Create dependency graph of "dpkg" tools'
    )
    parser.add_argument('--outfile', '-o', type=str)
    return parser


def _validate():
    # TODO: Implement me!
    pass


def _get_toolset():
    return set(_DEPENDENCIES.keys())


def _get_edges():
    edges = []
    for k, deps in _DEPENDENCIES.iteritems():
        if deps is not None:
            for d in deps:
                edges.append((k, d))
        else:
            edges.append((k, '(N/A)'))
    return edges


def main(outfile):
    # Get all the tools.
    toolset = _get_toolset()

    t = jinja2.Template(
r'''
digraph dpkg_ecosystem {
    graph[label="Dpkg {{now}}" fontsize=24 fontname="times-bold" labelloc=t]

    node[fontname=Courier fontsize=12 shape=box]
    edge[fontname=Courier fontsize=8 arrowhead=empty fontcolor=red]

{% for e in edges %}    "{{e[0]}}" -> "{{e[1]}}"[label="calls"]
{% endfor %}}
'''.lstrip('\n').decode('UTF-8')
    )

    out = t.render(
        now=datetime.datetime.now().strftime('%Y-%m-%dT%H:%M'),
        edges=_get_edges(),
    ).encode('UTF-8')

    print out

    if outfile is not None:
        with open(outfile, 'wb') as fh:
            fh.write(out)

        cmd = ['dot', '-Tsvg', '-O', outfile]
        subprocess32.check_call(cmd)


if __name__ == "__main__":
    main(**vars(_syntax().parse_args()))
