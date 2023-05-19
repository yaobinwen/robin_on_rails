# Protocol Buffers

## References

- [Protocol Buffers](https://protobuf.dev/)
- [Language Guide (proto 3)](https://protobuf.dev/programming-guides/proto3/)
- [Protocol Buffer Basics: Python](https://protobuf.dev/getting-started/pythontutorial/)
- [Protocol Buffers Python API Reference](https://googleapis.dev/python/protobuf/latest/)
- Code locations:
  - GitHub: [protocolbuffers/protobuf](https://github.com/protocolbuffers/protobuf)
  - C++ headers: `/usr/include/google/protobuf` (by `libprotobuf-dev`)
  - Python modules: `/usr/lib/python3/dist-packages/google/protobuf` (by `python3-protobuf`)
  - `Value` (used in `Struct`): [`struct.proto`](https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/struct.proto)

## Version

On [`protocolbuffers/protobuf` Releases page](https://github.com/protocolbuffers/protobuf/releases), you'll find they maintain two tracks of version schemes:
- `x.y.z`
- `x.y`

The document [_Version Support_](https://protobuf.dev/support/version-support/) explains them:

> Starting with the v20.x protoc release, we changed our versioning scheme to enable nimbler updates to language-specific parts of Protocol Buffers. In the new scheme, each language has its own major version that can be incremented independently of other languages. The minor and patch versions, however, remain coupled. This allows us to introduce breaking changes into some languages without requiring a bump of the major version in languages that do not experience a breaking change. For example, a single release might include **protoc version 24.0, Java runtime version 4.24.0 and C# runtime version 3.24.0**.

## Tips

- `util::Status` (returned by functions such as `google::protobuf::util::JsonStringToMessage`) is not just an integer. In fact, it contains descriptive error message that can help with debugging. One can simply `std::cout << ret` to print it on console:

```cpp
util::Status ret = util::JsonStringToMessage(oss.str(), &provider);
cout << "JsonStringToMessage return value: " << ret << endl;
```

## Fields are missing

Check the question [_protobuf MessageToJson removes fields with value 0_](https://stackoverflow.com/q/42987265/630364). According to the GitHub issue [_[proto3] Zero-value Enum types are not converted to text in Stringer implementations_](https://github.com/golang/protobuf/issues/258), the missing fields are by design:

> In proto3, zero value fields are indistinguishable from unset fields, by design. They are not sent on the wire either.
>
> This is working as intended. proto3 zero-values are omitted in the JSON format too.
>
> The zero-value should be a "throwaway" value: it's also what you will see if the sender of a serialized message sets the field to an invalid or unrecognized value.

This behavior has been mentioned in the section ["JSON Mapping"](https://developers.google.com/protocol-buffers/docs/proto3#json) in the language guide:

> If a value is missing in the JSON-encoded data or if its value is null, it will be interpreted as the appropriate default value when parsed into a protocol buffer. **If a field has the default value in the protocol buffer, it will be omitted in the JSON-encoded data by default to save space.** An implementation may provide options to emit fields with default values in the JSON-encoded output.
