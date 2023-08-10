import json

from google.protobuf import json_format
from sandbox.v0.author_pb2 import Author
from sandbox.v0.school_pb2 import Student, Teacher, Class, School


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


def _school_demo():
    """This demo focuses on dealing with repeated fields.
    """
    s_ada = Student(name="Ada")
    s_bob = Student(name="Bob")
    t_zach = Teacher(name="Zach")
    class1 = Class(id="class_1")
    class1.head_teacher.CopyFrom(t_zach)

    # `append` doesn't work if the field is a message type, so we have to use
    # either `add` or `extend`.
    class1.students.extend([s_ada, s_bob])

    t_mike = Teacher(name="Mike")
    class2 = Class(id="class_2", head_teacher=t_mike)
    class2.students.add().name="Linda"
    class2.students.add().name="Nicolas"

    school1 = School(name="school demo", classes=[class1, class2])

    json_str = json_format.MessageToJson(message=school1)
    print(json_str)


def _main():
    _author_demo()
    _school_demo()


def entry_point():
    _main()


if __name__ == "__main__":
    entry_point()
