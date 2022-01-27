generating
sourceStringsForArgSpecs: argSpecs defaultArgSpecs: defaultArgSpecs methodName: methodName

	| args argsString passedArgsString firstArgString |
	args := argSpecs collect: [:argSpec | GDApiObjectsGenerator squeakIdentifierName: (argSpec at: 'name')].
	
	argsString := args isEmpty ifTrue: [''] ifFalse: [ | firstArgName firstArgNameCap |
		firstArgName := args first.
		firstArgNameCap := args first capitalized.
		firstArgString := ((methodName endsWith: firstArgNameCap) ifTrue: [''] ifFalse: [firstArgNameCap]), ': ', firstArgName, ' '.
		firstArgString, ((args allButFirst collect: [:arg | arg, ': ', arg])  joinSeparatedBy: ' ')].
	
	passedArgsString := args joinSeparatedBy: '. '.
	
	^ {argsString. passedArgsString}