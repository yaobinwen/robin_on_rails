import json

from google.protobuf import json_format
from sandbox.v0.author_pb2 import Author


def _author_demo():
    author = Author(
        name="John Doe",
        email="john@doe.com",
    )

    # Method 1: Convert the message to a dict and then serialize to a JSON
    # string.
    json_str1 = json.dumps(
        json_format.MessageToDict(
            message=author,
            preserving_proto_field_name=True,
        )
    )
    print("author encoded to JSON: ", json_str1)

    # Method 2: Call `MessageToJson`.
    json_str2 = json_format.MessageToJson(
        message=author,
        preserving_proto_field_name=True,
    )
    print("author encoded to JSON: ", json_str2)

    # Parse from a JSON string.
    author_from_json_str1 = json_format.Parse(json_str1, message=Author())
    print("author parsed from json_str1: ", author_from_json_str1)
    author_from_json_str2 = json_format.Parse(json_str2, message=Author())
    print("author parsed from json_str2: ", author_from_json_str2)


def _main():
    _author_demo()


def entry_point():
    _main()


if __name__ == "__main__":
    entry_point()
