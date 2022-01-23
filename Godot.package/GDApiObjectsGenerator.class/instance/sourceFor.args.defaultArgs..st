generating
sourceFor: name args: argSpecs defaultArgs: defaultArgSpecs

	| methodName argStrings |
	methodName := GDApiObjectsGenerator squeakIdentifierName: name.
	argStrings := self sourceStringsForArgSpecs: argSpecs defaultArgSpecs: defaultArgSpecs methodName: methodName.
	
	^ '{1}{2}
	
	^ self call: ''{3}'' with: \{{4}\}' format: {methodName. argStrings first. name.  argStrings second}