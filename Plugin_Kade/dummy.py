import json
import uuid
import re

print 'Usage: input a simple sentence of the from:'
print '"What is/are [Person]\'s [info]?"'
print

while True:
   s = raw_input()
   if s == "quit":
      break

   m = re.match('[wW]hat (is|are) (\w+ ?\w*)\'s ([\w\s]+)\??', s)
   if m is not None:
      plural = (False, True)[m.group(1) == "are"]
      print "Plural:", plural, ", Person:", m.group(2), ", Property:", m.group(3), "\n"

      uid = uuid.uuid1()

      qObject = json.dumps({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': s, 'query': [['text',m.group(2)],['type','professor'],['traverse',''],['type',m.group(3)]], 'context': {}, 'plural': plural })

      print qObject
   else:
      print "Invalid input."

