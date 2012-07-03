#Parse Tester



def reply(inParse):
    parse = str(inParse)
    if parse=="no parse found":
        return "Sorry, I don't understand what you said."
    elif 'WhereQuestion' in parse:
        return "Sorry, I don't know where that is."
    elif 'WhoQuestion' in parse:
        return "Sorry, I don't know who that is."
    elif 'WhatQuestion' in parse:
        return "Sorry, I don't know the answer to that."
    else:
        return "Sorry, I don't know how to help you with that."

#print parse("[WhereQuestion] ( WHERE [Linking] ( IS ) [LocationQuery] ( [Person] ( [Professor] ( NYBERG ) ) 'S [Location] ( [_Office] ( OFFICE ) ) ) ) ")
