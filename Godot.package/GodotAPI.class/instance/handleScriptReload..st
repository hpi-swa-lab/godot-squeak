message handling
handleScriptReload: scriptReloadData

	| definitions |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	definitions do: #load.
	self associatePath: scriptReloadData scriptPath fromCString withClass: (Smalltalk classNamed: definitions first name).
	^ ExternalAddress new