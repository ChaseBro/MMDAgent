import json
import uuid

while True:
   s = raw_input()
   if s == "quit":
      break

   uid = uuid.uuid1()

   qObject = json.dumps({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': s, 'context': {}})

   print qObject

