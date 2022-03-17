message handling
handleScriptReload: scriptReloadData

	| definitions scriptDescription class |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	GDApi showln: 'handleScriptReload: ', definitions first name.
	self loadDefinitions: definitions.
	class := (Smalltalk classNamed: definitions first className).
	scriptReloadData scriptPath fromCString "TODO: this is a temporary fix. The actual solution is figuring out why we receive empty paths here"
		ifNotEmpty: [:path |self associatePath: path withClass: class].
	
	scriptDescription := GDScriptDescription allocateExternal.
	scriptDescription functions setNames: class functionNames allocateExternal: true.
	scriptDescription signals setNames: class signals allocateExternal: true.
	scriptDescription properties setPropertiesFrom: class properties allocateExternal: true.
	
	^ scriptDescription