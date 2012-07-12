import json
import uuid
import re
import sys

from twisted.internet import reactor
from txjsonrpc.web.jsonrpc import Proxy



print 'Usage: input a simple sentence of the from:'
print '"What is/are [Person]\'s [info]?"'
print

def printValue(value):
    print "Result: %s" % str(value)

def printError(error):
    print 'error', error

def shutDown(data):
    print "Shutting down reactor..."
    reactor.stop()

proxy = Proxy('http://127.0.0.1:8080/')


while True:
   s = raw_input()
   if s == "quit":
      break

   m = re.match('[wW]hat (is|are) (\w+ ?\w*)\'s ([\w\s]+)\??', s)
   if m is not None:
      plural = (False, True)[m.group(1) == "are"]
      print "Plural:", plural, ", Person:", m.group(2), ", Property:", m.group(3), "\n"

      uid = uuid.uuid1()
      jsonDump = json.dumps({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': s, 'query': [['text',m.group(2)],['type','professor'],['traverse',''],['type',m.group(3)]], 'context': {}, 'plural': plural })
      print jsonDump
      
      #qObject = json.loads(server.parse(json.dumps({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': s, 'query': [['text',m.group(2)],['type','professor'],['traverse',''],['type',m.group(3)]], 'context': {}, 'plural': plural })))
      d = proxy.callRemote('parse', jsonDump)
      d.addCallback(printValue).addErrback(printError).addBoth(shutDown)
      reactor.run()
   else:
      print "Invalid input."
