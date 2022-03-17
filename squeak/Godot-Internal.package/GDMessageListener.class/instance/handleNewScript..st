message handling
handleNewScript: newScriptData

	| name class source parentName |
	name := self class scriptPrefix , newScriptData scriptName fromCString.
	parentName := newScriptData parentName fromCString.
	GDApi showln: 'new script: ', name, ' parent ', parentName.
	class := self instantiateGodotScriptNamed: name parent: parentName.
	source := (TonelWriter sourceCodeOf: class) utf8Encoded.
	(self future: 1000) findAndAssociatePathForClass: class.
	^ (ExternalType char allocateExternal: source size + 1) toCString: source