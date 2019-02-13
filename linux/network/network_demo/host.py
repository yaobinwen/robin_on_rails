from network_demo.roles.socket_manager import SocketManager


class Host(SocketManager):

    def __init__(self, socket_num):
        '''Initialize a router.

        Args:
            socket_num: The number of sockets this router has.
        '''
        super(Host, self).__init__(socket_num)
