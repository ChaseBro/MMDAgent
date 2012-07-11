#Parse Tester
import json;

def procParse(inParse):
    print "I see you have a question..."
    inParse = str(inParse)
    if inParse=="no parse found":
        return "Sorry, I don't understand what you said."
    parse = json.loads(inParse)
    if 'Query' in parse.keys():
        query = parse['Query']
        if 'extracts' in query.keys():
            for extract in query['extracts']:
                if 'InfoQuery' in extract.keys():
                    infoQuery = extract['InfoQuery']
                    info = infoQuery['Info'][0]
                    node = extractLeaves(infoQuery['SpecNode'])
                    response = 'I think you are looking for info about ' + node + '\'s ' + info + "."

    else:
        response = "Sorry, I don't know how to help you with that."

    return response

def extractLeaves(node):
    if isinstance(node, list):
        print ' '.join(node)
        return ' '.join(node)

    if node[node.keys()[0]] is not None:
        return extractLeaves(node[node.keys()[0]])
    else:
        return node.keys()[0]

#print parse("[WhereQuestion] ( WHERE [Linking] ( IS ) [LocationQuery] ( [Person] ( [Professor] ( NYBERG ) ) 'S [Location] ( [_Office] ( OFFICE ) ) ) ) ")
