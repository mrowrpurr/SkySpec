scriptName SkySpecExpectations

SkySpecExpectations function ReturnExpectation() global
    return None
endFunction

function To(bool anything)
endFunction

SkySpecExpectations function ExpectString(string value) global
    return ReturnExpectation()
endFunction

bool function EqualString(string value) global
    return true
endFunction
