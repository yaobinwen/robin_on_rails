# Protocol Buffers

## References

- [Protocol Buffers](https://protobuf.dev/)
- [Language Guide (proto 3)](https://protobuf.dev/programming-guides/proto3/)
- [Protocol Buffer Basics: Python](https://protobuf.dev/getting-started/pythontutorial/)
- [Protocol Buffers Python API Reference](https://googleapis.dev/python/protobuf/latest/)

## Fields are missing

Check the question [_protobuf MessageToJson removes fields with value 0_](https://stackoverflow.com/q/42987265/630364). According to the GitHub issue [_[proto3] Zero-value Enum types are not converted to text in Stringer implementations_](https://github.com/golang/protobuf/issues/258), the missing fields are by design:

> In proto3, zero value fields are indistinguishable from unset fields, by design. They are not sent on the wire either.
>
> This is working as intended. proto3 zero-values are omitted in the JSON format too.
>
> The zero-value should be a "throwaway" value: it's also what you will see if the sender of a serialized message sets the field to an invalid or unrecognized value.

This behavior has been mentioned in the section ["JSON Mapping"](https://developers.google.com/protocol-buffers/docs/proto3#json) in the language guide:

> If a value is missing in the JSON-encoded data or if its value is null, it will be interpreted as the appropriate default value when parsed into a protocol buffer. **If a field has the default value in the protocol buffer, it will be omitted in the JSON-encoded data by default to save space.** An implementation may provide options to emit fields with default values in the JSON-encoded output.
