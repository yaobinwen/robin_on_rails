import asyncio
import functools


def deco(func):
    @functools.wraps(func)
    async def wrapper(self):
        coro = func(self)
        await coro
        await self._internal()

    return wrapper


class A(object):
    @deco
    async def work(self):
        await self._internal()

    async def _internal(self):
        print("[internal] do work")


def main():
    a = A()

    loop = asyncio.get_event_loop()
    loop.run_until_complete(a.work())


if __name__ == "__main__":
    main()
