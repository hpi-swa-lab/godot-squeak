message handling
handleNewScript: newScriptData

	| name class source |
	name := newScriptData scriptName fromCString asSymbol.
	GDApi showln: 'new script: ', name.
	class := self instantiateGodotScriptNamed: name.
	source := (TonelWriter sourceCodeOf: class) utf8Encoded.
	(self future: 1000) findAndAssociatePathForClass: class.
	^ (ExternalType char allocateExternal: source size + 1) toCString: source