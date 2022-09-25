import typing

from sandbox.sandbox_pb2 import SandboxRequest


def to_str(request: SandboxRequest):
    return (
        f"<SandboxRequest ids={request.ids}>"
    )


def _main():
    r = SandboxRequest()
    print(to_str(r))


def entry_point():
    _main()


if __name__ == "__main__":
    entry_point()
