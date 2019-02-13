class SocketManager(object):

    def __init__(self, num):
        '''Initialize a socket manager.

        Args:
            num: The number of the managed sockets.
        '''
        if num <= 0:
            raise ValueError('num must be > 0')

        self._num = num
        
        # True means the socket is available; False means unavailable.
        self._available = [True] * self._num

    def plug(self, cable, index):
        if index < 0:
            raise IndexError('index must be >= 0')

        if not self._available[index]:
            raise RuntimeError(
                'socket[{i}] is not available'.format(i=index)
            )

        self._available[index] = False
