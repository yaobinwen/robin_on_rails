import asyncio


class StopPolling(Exception):
    def __init__(self, rc=0):
        self.rc = rc


def sync_fun(*, N):
    print("running sync_func: N = ", N)
    if N > 2:
        raise StopPolling(42)


# <https://stackoverflow.com/a/55505152>
async def periodic_callback(*args, timeout_s, fn, **kwds):
    N = 0
    while True:
        try:
            await asyncio.gather(fn(N=N), asyncio.sleep(timeout_s))
            N += 1
        except StopPolling as e:
            return e.rc
        except Exception as e:
            print("[error] ", fn, e)


def main():
    async def async_func(*, N):
        return sync_fun(N=N)
    
    f = asyncio.ensure_future(
        periodic_callback(timeout_s=0.1, fn=async_func)
    )

    loop = asyncio.get_event_loop()

    rc = loop.run_until_complete(f)
    print("rc = ", rc)
    return rc


if __name__ == "__main__":
    main()
