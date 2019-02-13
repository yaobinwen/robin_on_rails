from network_demo.roles.socket_manager import SocketManager


class Router(SocketManager):

    def __init__(self, socket_num):
        '''Initialize a router.

        Args:
            socket_num: The number of sockets this router has.
        '''
        super(Router, self).__init__(socket_num)
