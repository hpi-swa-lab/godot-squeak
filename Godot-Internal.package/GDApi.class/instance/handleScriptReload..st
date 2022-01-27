message handling
handleScriptReload: scriptReloadData

	| definitions functionNames scriptFunctions |
	definitions := TonelParser parseStream: scriptReloadData scriptSource fromCString readStream.
	GDApi showln: 'handleScriptReload: ', definitions first name.
	self associatePath: scriptReloadData scriptPath fromCString
		withClass: (Smalltalk classNamed: definitions first name).
	self loadDefinitions: definitions.
	
	"TODO a similar packing thing happens in callMethod:onVariant:with:, maybe extract?"
	functionNames := definitions allButFirst collect: [:definition | self squeakToGodotMethodName: definition selector].
	scriptFunctions := GDScriptFunctions allocateExternal.
	scriptFunctions numNames: functionNames size.
	scriptFunctions names: (ExternalType void asPointerType allocateExternal: functionNames size).
	functionNames withIndexDo: [:name :index || externalString |
		externalString := (ExternalType char allocateExternal: name size + 1) toCString: name.
		scriptFunctions names setContentType: ExternalType void asPointerType; at: index put: externalString getHandle].
	
	^ scriptFunctions