from txjsonrpc.web import jsonrpc
from twisted.web import server
from twisted.internet import reactor
import json

class Math(jsonrpc.JSONRPC):
    """
    An example object to be published.
    """
    def jsonrpc_parse(self, a):
        """
        Return sum of arguments.
        """
        print json.dumps(a);
        return 'test'

reactor.listenTCP(8080, server.Site(Math()))
reactor.run()
