# asyncio

Documents:
- [Python 3.6: 18.5. asyncio — Asynchronous I/O, event loop, coroutines and tasks](https://docs.python.org/3.6/library/asyncio.html)

> Note that tasks are futures, and everything said about futures also applies to tasks.


```plantuml
@startuml
class Future
class Task
class CoroutineFunction
class CoroutineObject
@enduml
```

[Example: Chain coroutines](https://docs.python.org/3.6/library/asyncio-task.html#example-chain-coroutines) is a good example:

![Sequence diagram of the chain coroutines](https://docs.python.org/3.6/_images/tulip_coro.png)
