#Parse Tester
import json
import uuid
import os
import re
import sys
import shlex
import subprocess
import jsonrpclib

class JsonServer:
    def __init__(self, url):
        self.proxy = jsonrpclib.Server(url)

    def callNell(self, plainText, jsonQuery):
        uid = uuid.uuid1()
        jsonDump = json.dumps({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': plainText, 'query': jsonQuery, 'context': {}, 'plural': False })
        print jsonDump
        result = self.proxy.parse({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': plainText, 'query': jsonQuery, 'context': {}, 'plural': False })
        print result
        return result

myInfo = {'name' : 'Fergus', 'email': 'fergus at c.m.u dot e.d.u', 'department': 'Language Technologies', 'firstName': 'Fergus', 'lastName': 'Carnegie'}

def procParse(plainText, inParse):
    print "I see you have a question..."
    inParse = str(inParse)
    if inParse=="no parse found":
        return "I'm sorry, I'm a little hard of hearing, can ya repeat that?"
    parse = json.loads(inParse)

    server = JsonServer('http://128.237.232.50:8080')

    if 'Query' in parse.keys():
        query = parse['Query']
        jsonQuery = None
        if 'extracts' in query.keys():
            for extract in query['extracts']:
                if 'InfoQuery' in extract.keys():
                    infoQuery = extract['InfoQuery']
                    infoNode = infoQuery['Info']
                    specNode = infoQuery['SpecNode']
                    if extractLeaves(specNode) == 'you':
                        if infoNode == 'phone':
                            response = 'Why do ya want my phone number? If you want a date just say so.'
                        else:
                            response = 'My ' + (infoNode, 'last name')[infoNode == 'lastName'] + ' is ' + myInfo[infoNode] + '.'
                    else:
                        jsonQuery = jsonSpecNode(infoQuery['SpecNode']) , ('Traverse', None), ('Type', infoNode)
                        answer = server.callNell(plainText, jsonQuery)
                        if answer is not None and answer['plainText'] is not None:
                            response = extractLeaves(specNode) + '\'s ' + infoNode + ' is ' + answer['plainText']
                        response = 'I think you are looking for info about ' + extractLeaves(specNode) + '\'s ' + infoNode + "."

                elif 'NodeQuery' in extract.keys():
                    nodeQuery = extract['NodeQuery']
                    genNode = nodeQuery['GenNode']
                    genNodeType = getNodeType(nodeQuery['GenNode'])
                    specNode = nodeQuery['SpecNode']
                    relVerb = nodeQuery['RelVerb']

                    jsonQuery = jsonSpecNode(specNode)

                    if 'AllLinking' in nodeQuery.keys():
                        response = 'I think you want to know what ' + genNodeType + ' ' + extractLeaves(nodeQuery['AllLinking']) + ' ' + extractLeaves(specNode) + ' ' + extractLeaves(relVerb) + '.'
                        jsonQuery += ', ["Traverse", "' + extractLeaves(relVerb) + '"]'
                    else:
                        response = 'I think you want to know what ' + genNodeType + ' ' + extractLeaves(relVerb) + ' ' + extractLeaves(specNode) + '.'
                        jsonQuery += ', ["TraverseIn", "' + extractLeaves(relVerb) + '"]'

                    if genNodeType is not None:
                        jsonQuery += ', ["Type", "' + genNodeType + '"]'

                elif 'LocationQuery' in extract.keys():
                    locQuery = exract['LocationQuery']
                    if 'SpecLocation' in locQuery:
                        jsonQuery = '["Text", "' + extractLeaves(locQuery['SpecLocation']) + '"]'
                    elif 'GenLocation' in locQuery:
                        genLocType = getNodeType(locQuery['GenLocation'])
                        if genLocType is None:
                            genLocType = 'location'
                    genLoc = extract['GenLocation']
                    specNode = extract['SpecNode']
                    response = 'I think you want to know something about a location.'

        if jsonQuery is not None:
            print jsonQuery
    else:
        response = "Sorry, I don't know how to help you with that."

    return response

def extractLeaves(node):
    if isinstance(node, list):
        return ' '.join(node)

    if node[node.keys()[0]] is not None:
        return extractLeaves(node[node.keys()[0]])
    else:
        return node.keys()[0]

# Extract the json query from a SpecNode (Specific Node)
def jsonSpecNode(specNode):
    if 'SpecPerson' in specNode:
        jsonQuery = ('Type', 'Person')
        jsonQuery += ('Text', extractLeaves(specNode))
    elif 'SpecLocation' in specNode:
        jsonQuery = ('Type', 'Location')
        jsonQuery += ('Text', extractLeaves(specNode))
    else:
        jsonQuery = ('Text', extractLeaves(specNode))

    return jsonQuery

# Extracts the node type of a node (such as 'person', 'location', etc.)
def getNodeType(node):
    label = node.keys()[0]
    if label == 'SpecPerson' or label == 'GenPerson':
        return 'person'
    elif label == 'SpecLocation' or label == 'GenLocation':
        return 'location'
    else:
        return None

#print parse("[WhereQuestion] ( WHERE [Linking] ( IS ) [LocationQuery] ( [Person] ( [Professor] ( NYBERG ) ) 'S [Location] ( [_Office] ( OFFICE ) ) ) ) ")
if __name__ == '__main__':
    sys.argv.pop(0)
    args = sys.argv

    if len(args) < 5:
        print 'Usage: python kade.py [path to parse_text] -dir [path to grammar dir] -grammar [grammar file name]'
        quit()
    print args
    print

    print 'Usage: input a simple sentence of the from:'
    print '"What is/are [Person]\'s [info]?"'
    print

    while True:
        s = raw_input()
        if s == "quit":
            break

        if s is not '':
            out, err = subprocess.Popen(args, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE).communicate(s)
            print 'err:',err
            print 'out:',out
            print procParse(s, out)

        else:
            print "Invalid input."

