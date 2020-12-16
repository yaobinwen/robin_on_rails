#!/usr/bin/python3
# -*- coding: utf-8 -*-

# This code is written against Python 3.6.9. It demonstrates how to create an
# XML document using `xml.dom` module. A demo is necessary because the
# official documentation [1] doesn't provide one.
# [1] https://docs.python.org/3.6/library/xml.dom.html
#
# We are going to create the following document:
#
# ```xml
# <?xml version="1.0" encoding="UTF-8"?>
# <gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
#     <graph mode="static" defaultedgetype="directed">
#         <attributes class="node">
#             <attribute id="ALL" title="ALL" type="boolean">
#                 <default>true</default>
#             </attribute>
#         </attributes>
# 
#         <attributes class="edge">
#             <attribute id="recommends" title="recommends" type="boolean">
#                 <default>false</default>
#             </attribute>
#         </attributes>
# 
#         <nodes>
#             <node id="A" />
#             <node id="E" />
#         </nodes>
# 
#         <edges>
#             <edge source="B" target="A" />
#             <edge source="C" target="A" />
#             <edge source="C" target="E" />
#             <edge source="D" target="B" />
#             <edge source="folder/H" target="dir/D" />
#         </edges>
#     </graph>
# </gexf>
# ```


import inspect as I
import xml.dom as D


def main():
    impl = D.getDOMImplementation(name=None, features=())

    # `impl` is a `minidom` implementation.
    # <class 'xml.dom.minidom.DOMImplementation'>
    print(f"{impl.__class__}")

    doc = impl.createDocument(
        namespaceURI="http://www.gexf.net/1.2draft",
        qualifiedName="gexf",
        doctype=None,
    )

    # <gexf>
    elem_gexf = doc.documentElement
    for n, v in {
        "xmlns": "http://www.gexf.net/1.2draft",
        "version": "1.2",
    }.items():
        elem_gexf.setAttribute(attname=n, value=v)
    # Now we have `<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">`.

    # <graph>
    elem_graph = doc.createElement(tagName="graph")
    for n, v in {
        "mode": "static",
        "defaultedgetype": "directed",
    }.items():
        elem_graph.setAttribute(attname=n, value=v)
    elem_gexf.appendChild(elem_graph)

    # <attributes class="node">
    elem_attributes_node = doc.createElement(tagName="attributes")
    elem_attributes_node.setAttribute(attname="class", value="node")

    # <attribute id="ALL">
    elem_attr_ALL = doc.createElement(tagName="attribute")
    for n, v in {
        "id": "ALL",
        "title": "ALL",
        "type": "boolean",
    }.items():
        elem_attr_ALL.setAttribute(attname=n, value=v)

    # <default>true</default>
    elem_default_true = doc.createElement(tagName="default")

    # Writing a TEXT node is a bit tricky. The TEXT node must be the only child
    # of the parent node in order to be written by `writexml()`.
    # See `minidom.Element.writexml()` for the implementation.
    node_text_true = doc.createTextNode("true")

    elem_default_true.appendChild(node_text_true)
    elem_attr_ALL.appendChild(elem_default_true)
    elem_attributes_node.appendChild(elem_attr_ALL)
    elem_graph.appendChild(elem_attributes_node)

    # <attributes class="edge">
    elem_attributes_edge = doc.createElement(tagName="attributes")
    elem_attributes_edge.setAttribute(attname="class", value="edge")

    # <attribute id="recommends">
    elem_attr_recommends = doc.createElement(tagName="attribute")
    for n, v in {
        "id": "recommends",
        "title": "recommends",
        "type": "boolean",
    }.items():
        elem_attr_recommends.setAttribute(attname=n, value=v)

    # <default>false</default>
    elem_default_false = doc.createElement(tagName="default")
    node_text_false = doc.createTextNode("false")

    elem_default_false.appendChild(node_text_false)
    elem_attr_recommends.appendChild(elem_default_false)
    elem_attributes_edge.appendChild(elem_attr_recommends)
    elem_graph.appendChild(elem_attributes_edge)

    # <nodes>
    elem_nodes = doc.createElement(tagName="nodes")
    for v in ("A", "E"):
        elem_node = doc.createElement(tagName="node")
        elem_node.setAttribute(attname="id", value=v)
        elem_nodes.appendChild(elem_node)
    elem_graph.appendChild(elem_nodes)

    # <edges>
    elem_edges = doc.createElement(tagName="edges")
    for s, t in {
        "B": "A",
        "C": "E",
        "D": "B",
        "folder/H": "dir/D",
    }.items():
        elem_edge = doc.createElement(tagName="edge")
        elem_edge.setAttribute(attname="source", value=s)
        elem_edge.setAttribute(attname="target", value=t)
        elem_edges.appendChild(elem_edge)
    elem_graph.appendChild(elem_edges)

    # Print the document.
    xml_text = doc.toprettyxml(encoding="UTF-8").decode("UTF-8")
    print(xml_text)


if __name__ == "__main__":
    main()
