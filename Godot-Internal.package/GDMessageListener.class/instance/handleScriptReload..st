message handling
handleScriptReload: scriptReloadData

	| definitions scriptDescription class |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	GDApi showln: 'handleScriptReload: ', definitions first name.
	class := (Smalltalk classNamed: definitions first name).
	self associatePath: scriptReloadData scriptPath fromCString withClass: class.
	self loadDefinitions: definitions.
	
	scriptDescription := GDScriptDescription allocateExternal.
	scriptDescription functions setNames: class functionNames allocateExternal: true.
	scriptDescription signals setNames: class signals allocateExternal: true.
	scriptDescription properties setPropertiesFrom: class properties allocateExternal: true.
	
	^ scriptDescription