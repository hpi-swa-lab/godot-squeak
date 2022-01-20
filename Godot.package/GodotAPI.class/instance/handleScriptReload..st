message handling
handleScriptReload: scriptReloadData

	| definitions |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	Transcript showln: 'handleScriptReload: ', definitions first name.
	self associatePath: scriptReloadData scriptPath fromCString withClass: (Smalltalk classNamed: definitions first name).
	definitions do: #load.
	^ ExternalAddress new