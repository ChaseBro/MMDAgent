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
        try:
            result = self.proxy.parse({'uuid': uid.int, 'qid': 0, 'type': 'question', 'plainText': plainText, 'query': jsonQuery, 'context': {}, 'plural': False })
            print result
        except:
            result = None
        return result

myInfo = {'name' : 'Fergus', 'email': 'fergus at c.m.u dot e.d.u', 'department': 'Language Technologies', 'firstName': 'Fergus', 'lastName': 'Carnegie'}
infoOutput = {'lastName': 'last name', 'firstName': 'name', 'phone': 'phone number', 'fax': 'fax number', 'email': 'email address', 'name': 'name', 'department': 'department'}

def procParse(plainText, inParse):
    inParse = str(inParse)
    if 'No parses found' in inParse:
        return "I'm sorry, I'm a little hard of hearing, can ya repeat that?"
    parse = json.loads(inParse)

    server = JsonServer('http://peace.isri.cs.cmu.edu:9080/constellation/')

    if 'Query' in parse.keys():
        query = parse['Query']
        jsonQuery = None
        if 'extracts' in query.keys():
            for extract in query['extracts']:
                if 'InfoQuery' in extract.keys():
                    infoQuery = extract['InfoQuery']
                    if 'Info' in infoQuery.keys():
                        infoNode = extractLeaves(infoQuery['Info'])
                    else:
                        infoNode = extractLeaves(infoQuery['InfoVerb'])
                    specNode = infoQuery['SpecNode']
                    if extractLeaves(specNode) == 'you':
                        if infoNode == 'phone':
                            response = 'Why do ya want my phone number? If you want a date just say so.'
                        elif infoNode == 'lastName':
                            response = 'My last name is Carnegie, just like Andrew\'s.'
                        elif infoNode == 'firstName' or infoNode == 'name':
                            response = 'My name is Fergus.'
                        else:
                            response = 'My ' + (infoNode, 'last name')[infoNode == 'lastName'] + ' is ' + myInfo[infoNode] + '.'
                    else:
                        jsonQuery = jsonSpecNode(infoQuery['SpecNode']) + (('Traverse', None), ('Type', infoNode))
                        answer = server.callNell(plainText, jsonQuery)
                        if answer is not None and answer['plainText'] is not None:
                            response = extractLeaves(specNode) + '\'s ' + infoOutput[infoNode] + ' is ' + answer['plainText']
                        else:
                            response = 'I\'m sorry, I don\'t what ' + extractLeaves(specNode) + '\'s ' + infoOutput[infoNode] + ' is.'

                elif 'NodeQuery' in extract.keys():
                    nodeQuery = extract['NodeQuery']
                    genNode = extractLeaves(nodeQuery['GenNode'])
                    specNode = nodeQuery['SpecNode']
                    relVerb = nodeQuery['RelVerb']

                    jsonQuery = jsonSpecNode(specNode)

                    if 'AllLinking' in nodeQuery.keys() and ( 'Prep' not in nodeQuery.keys() or nodeQuery['Prep'][0] != 'BY' ):
                        response = 'I think you want to know what ' + genNode + ' ' + extractLeaves(nodeQuery['AllLinking']) + ' ' + extractLeaves(specNode) + ' ' + extractLeaves(relVerb) + '.'
                        jsonQuery += (('Traverse', extractLeaves(relVerb)),)
                    else:
                        response = 'I think you want to know what ' + genNode + ' ' + extractLeaves(relVerb) + ' ' + extractLeaves(specNode) + '.'
                        jsonQuery += (('TraverseIn', extractLeaves(relVerb)),)

                    jsonQuery += getNodeType(nodeQuery['GenNode'])

                    answer = server.callNell(plainText, jsonQuery)
                    if answer is not None and answer['plainText'] is not None:
                        response = genNodeType + '\'s ' + infoNode + ' is ' + answer['plainText']

                elif 'LocationQuery' in extract.keys():
                    locQuery = extract['LocationQuery']
                    if 'SpecLocation' in locQuery:
                        jsonQuery = '["Text", "' + extractLeaves(locQuery['SpecLocation']) + '"]'
                    elif 'GenLocation' in locQuery:
                        genLoc = extract['GenLocation']
                        genLocType = getNodeType(genLoc)
                        if genLocType is None:
                            genLocType = 'location'

                    specNode = extract['SpecNode']
                    response = 'I think you want to know something about a location.'

                elif 'KadeQuery' in extract.keys():
                    kadeQuery = extractLeaves(extract['KadeQuery'])
                    if kadeQuery == 'HOW OLD ARE YOU':
                        response = 'Old enough to mind my manners sonny. I might think you should do the same.'
                    elif kadeQuery == 'WHO MADE YOU' or kadeQuery == 'WHO BUILT YOU':
                        response = 'I was made by the Kade team from the Language Technologies Institute her at Carnegie Mellon.'
                    elif kadeQuery == 'WHERE DID YOU COME FROM':
                        response = 'I came from your mother, and I learned some manners, unlike you.'
                    else:
                        response = 'Something else.'

    else:
        answer = server.callNell(plainText, None)
        if answer is not None and answer['plainText'] is not None:
            response = 'I think the answer you\'re looking for is ' + answer['plainText']
        response = 'I\'m sorry, I don\'t know how to help you with that.'

    return response

def extractLeaves(node):
    if isinstance(node, list):
        return ' '.join(node)

    if isinstance(node, dict):
        if node[node.keys()[0]] is not None:
            return extractLeaves(node[node.keys()[0]])
        else:
            return node.keys()[0]
    return node

# Extract the json query from a SpecNode (Specific Node)
def jsonSpecNode(specNode):
    if 'SpecPerson' in specNode:
        if 'SpecProfessor' in specNode['SpecPerson'].keys():
            if 'AmbiguousPerson' not in specNode['SpecPerson']['SpecProfessor'].keys():
                jsonQuery = (('Text', specNode['SpecPerson']['SpecProfessor'].keys()[0]),)
            else:
                jsonQuery = (('Text', extractLeaves(specNode)),)
        else:
            jsonQuery = (('Text', extractLeaves(specNode)),)
        jsonQuery += (('Type', 'person'),)
    elif 'SpecLocation' in specNode:
        jsonQuery = ('Text', extractLeaves(specNode))
        jsonQuery += (('Type', 'location'),)
    else:
        jsonQuery = ('Text', extractLeaves(specNode))

    return jsonQuery

# Extracts the node type of a node (such as 'person', 'location', etc.)
def getNodeType(node):
    label = node.keys()[0]
    ret = ()
    if label == 'SpecPerson' or label == 'GenPerson':
        ret += (('Type', 'person'),)
    elif label == 'SpecLocation' or label == 'GenLocation':
        ret += (('Type', 'location'),)
    return ret

#print parse("[WhereQuestion] ( WHERE [Linking] ( IS ) [LocationQuery] ( [Person] ( [Professor] ( NYBERG ) ) 'S [Location] ( [_Office] ( OFFICE ) ) ) ) ")
if __name__ == '__main__':
    sys.argv.pop(0)
    args = sys.argv

    if len(args) < 5:
        print 'Usage: python kade.py [path to parse_text] -dir [path to grammar dir] -grammar [grammar file name]'
        quit()

    print 'Usage: input a simple sentence of the from:'
    print '"What is/are [Person]\'s [info]?"'
    print

    while True:
        s = raw_input()
        if s == "quit":
            break

        if s is not '':
            out, err = subprocess.Popen(args, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE).communicate(s)
            print out
            print procParse(s, out)
            print

        else:
            print "Invalid input."

