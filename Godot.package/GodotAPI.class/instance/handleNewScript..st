message handling
handleNewScript: newScriptData

	| name class source |
	name := newScriptData scriptName fromCString asSymbol.
	GodotAPI showln: 'new script: ', name.
	class := self instantiateGodotScriptNamed: name.
	source := (TonelWriter sourceCodeOf: class) utf8Encoded.
	"[2 seconds wait] fork"
	"(self future: 1000) scanFiles"
	(self future: 1000) findAndAssociatePathForClass: class.
	^ (ExternalType char allocateExternal: source size + 1) toCString: source