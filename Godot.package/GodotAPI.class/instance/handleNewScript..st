message handling
handleNewScript: newScriptData

	| name source |
	name := newScriptData scriptName fromCString.
	Transcript cr; show: 'new script: '; show: name.
	self instantiateGodotScriptNamed: name.
	source := (TonelWriter sourceCodeOf: (Smalltalk classNamed: name)) utf8Encoded.
	"[2 seconds wait] fork"
	"(self future: 1000) scanFiles"
	^ (ExternalType char allocateExternal: source size + 1) toCString: source