#!/usr/bin/env python
# -*- mode: python; coding: UTF-8; -*-


"""Display relations between git concepts."""


import argparse
import datetime
import jinja2
import subprocess32


_DEPENDENCIES = {
    'object': set([
        ('has property of', 'reachable'),
    ]),
    'object database': ('stores', 'object'),
    'object identifier': ('is synonym for', 'object name'),
    'object name': set([
        ('is a', 'SHA-1'),
        ('identifies', 'object'),
    ]),
    'reachable': None,
    'ref': set([
        ('points to', 'object name'),
        ('points to', 'symbolic ref'),
    ]),
    'remote': ('is synonym for', 'remote repository'),
    'remote repository': ('is a', 'repository'),
    'remote-tracking branch': set([
        ('is a', 'ref'),
        ('follows', 'remote repository'),
    ]),
    'repository': set([
        ('is a collection of', 'ref'),
        ('has an', 'object database'),
    ]),
    'SHA-1': None,
    'symref': ('is short for', 'symbolic ref'),
    'symbolic ref': None,
}


def _syntax():
    parser = argparse.ArgumentParser(
        description='Create relation graph of git concepts.'
    )
    parser.add_argument('--outfile', '-o', type=str)
    return parser


def _get_edges():
    edges = []
    for k, deps in _DEPENDENCIES.iteritems():
        if deps is not None:
            if isinstance(deps, tuple):
                edges.append((k, deps[0], deps[1]))
            else:
                for r, d in deps:
                    edges.append((k, r, d))
        else:
            edges.append((k, '(N/A)', '(N/A)'))
    return edges


def main(outfile):
    t = jinja2.Template(
r'''
digraph git_concepts {
    graph[label="Git Concepts {{now}}" fontsize=24 fontname="times-bold" labelloc=t]

    node[fontname=Courier fontsize=12 shape=box]
    edge[fontname=Courier fontsize=8 arrowhead=empty fontcolor=red]

{% for e in edges %}    "{{e[0]}}" -> "{{e[2]}}"[label="{{e[1]}}"]
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
