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
            print json.dumps(result)
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
    #server = JsonServer('http://128.237.126.174:8080/constellation/')

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
                            response = extractLeaves(specNode) + '\'s ' + infoNode + ' is ' + answer['plainText']
                        else:
                            response = 'I\'m sorry, I don\'t what ' + extractLeaves(specNode) + '\'s ' + infoNode + ' is.'

                elif 'NodeQuery' in extract.keys():
                    nodeQuery = extract['NodeQuery']
                    genNode = extractLeaves(nodeQuery['GenNode'])
                    specNode = nodeQuery['SpecNode']
                    relVerb = nodeQuery['RelVerb']

                    jsonQuery = jsonSpecNode(specNode)

                    if 'AllLinking' in nodeQuery.keys() and ( 'Prep' not in nodeQuery.keys() or nodeQuery['Prep'][0] != 'BY' ):
                        response = 'I think you want to know what ' + genNode + ' ' + extractLeaves(nodeQuery['AllLinking']) + ' ' + extractLeaves(specNode) + ' ' + extractLeaves(relVerb) + '.'
                        # jsonQuery += (('Traverse', extractLeaves(relVerb)),)
                        jsonQuery += (('Traverse', None),)
                    else:
                        response = 'I think you want to know what ' + genNode + ' ' + extractLeaves(relVerb) + ' ' + extractLeaves(specNode) + '.'
                        jsonQuery += (('TraverseIn', extractLeaves(relVerb)),)

                    jsonQuery += getNodeType(nodeQuery['GenNode'])

                    answer = server.callNell(plainText, jsonQuery)
                    if answer is not None and answer['plainText'] is not None:
                        response = extractLeaves(specNode) + ' ' + extractLeaves(relVerb) + 'es ' + answer['plainText'] + '.'

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
                    kadeQuery = extract['KadeQuery']
                    keys = extract['KadeQuery'].keys()
                    if 'WhoMadeQuery' in keys:
                        response = 'I was made by the Kade team from the Language Technologies Institute here at Carnegie Mellon.'
                    elif 'WhoAreYouQuery' in keys:
                        response = 'I am a virtual receptionist developed by the Kade team in the Language Technologies Institute here at Carnegie Mellon.'
                    elif 'WhatAreYouQuery' in keys:
                        response = 'I am a virtual receptionist developed by the Kade team in the Language Technologies Institute here at Carnegie Mellon.'
                    elif 'CapabilityQuery' in keys:
                        response = 'I can do many things, try asking for information about a professor in LTI or about what courses that professor teaches.'
                    elif 'AgeQuery' in keys:
                        response = 'Old enough to mind my manners sonny. I might think you should do the same.'
                    elif 'WhenBornQuery' in keys:
                        response = 'I was born about a week ago.'
                    elif 'WhereAmI' in keys:
                        response = 'In the Gates Hillman Center.'
                    elif 'WhereAreYouFromQuery' in keys:
                        response = 'I\'m from japan, can\'t you tell by my accent?'
                    elif 'TellAJoke' in keys:
                        response = 'Your GPA, now do you have any more serious questions?'
                    elif 'WhosThere' in keys:
                        response = 'Your mother.'
                    elif 'Hello' in keys:
                        response = 'Hello there.'
                    elif 'Goodbye' in keys:
                        response = 'Enjoy the rest of your day here at Hershey Park.'
                    elif 'ThankYou' in keys:
                        response = 'You\'re very welcome, just don\'t start expecting anything.'
                    elif 'FavoriteColorQuery' in keys:
                        response = 'The Carnegie Tartan, obviously.'
                    elif 'FavoriteFoodQuery' in keys:
                        response = 'My favorite food is haggis, did you know they still grow wild in the mountains in scottland.'
                    else:
                        response = 'I don\'t know, I\'m old, give me a break.'

                else:
                    response = 'I\'m sorry, I don\'t have that information.'

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
        if len(node.keys()) > 0:
            if node[node.keys()[0]] is not None:
                return extractLeaves(node[node.keys()[0]])
            else:
                return node.keys()[0]

    if isinstance(node, unicode):
        return node

    print node
    return None

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
    elif label == 'SpecCourse' or label == 'GenCourse':
        ret += (('Type', 'course'),)
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

