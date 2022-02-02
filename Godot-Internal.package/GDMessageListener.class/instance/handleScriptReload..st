message handling
handleScriptReload: scriptReloadData

	| definitions functionNames scriptDescription class |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	GDApi showln: 'handleScriptReload: ', definitions first name.
	class := (Smalltalk classNamed: definitions first name).
	self associatePath: scriptReloadData scriptPath fromCString withClass: class.
	self loadDefinitions: definitions.
	
	scriptDescription := GDScriptDescription allocateExternal.
	functionNames := class selectors collect: [:selector | GDMessageListener squeakToGodotMethodName: selector].
	scriptDescription functions setNames: functionNames allocateExternal: true.
	scriptDescription signals setNames: class signals allocateExternal: true.
	
	^ scriptDescription