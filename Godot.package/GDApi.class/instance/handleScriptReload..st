message handling
handleScriptReload: scriptReloadData

	| definitions |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	GDApi showln: 'handleScriptReload: ', definitions first name.
	self associatePath: scriptReloadData scriptPath fromCString
		withClass: (Smalltalk classNamed: definitions first name).
	self loadDefinitions: definitions.
	^ ExternalAddress new