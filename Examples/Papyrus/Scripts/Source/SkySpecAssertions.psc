scriptName SkySpecAssertions

function Fail(string message = "") global native

function Assert(bool value, string message = "") global
    Fail("Foo")
endFunction

function AssertStringEquals(string actual, string expected, string message = "") global
    Fail("bar")
endFunction

