import asyncio


async def coro():
    for i in range(0, 2):
        print(f"Running coro: i={i}")
        await asyncio.sleep(0.1)
        print(f"Finishing coro: i={i}")


def main():
    c = coro()
    assert asyncio.coroutines.iscoroutine(c)

    # NOTE(ywen): Because `c` is a coroutine, `ensure_future` will create a
    # `Task` instance to wrap it. (A `Task` is a sub-class of `Future`.)
    task = asyncio.ensure_future(c)
    loop = asyncio.get_event_loop()

    # NOTE(ywen): When running on Linux, we are using a `_UnixSelectorEventLoop`.
    assert isinstance(loop, asyncio.unix_events._UnixSelectorEventLoop)

    # NOTE(ywen): The coroutine in the future is the one that we passed in.
    assert task._coro is c

    # NOTE(ywen): The event loop that the future uses is the global event loop.
    assert task._loop is loop

    # NOTE(ywen): As soon as the `task` was created above, the `Task._step()`
    # was scheduled to be called soon by calling `self._loop.call_soon(self._step)`.
    assert len(loop._ready) == 1

    loop.run_until_complete(task)


if __name__ == "__main__":
    main()
