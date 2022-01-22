generating
sourceFor: name args: argSpecs defaultArgs: defaultArgSpecs

	"TODO: proper default argument handling"
	| methodName argStrings |
	methodName := GDApiObjectsGenerator squeakIdentifierName: name.
	"args := argSpecs collect: [:argSpec | self squeakIdentifierName: (argSpec at: 'name')].
	firstArgString := args isEmpty ifTrue: ['']
		ifFalse: [((methodName endsWith: args first capitalized) ifTrue: [''] ifFalse: [args first capitalized]), ': ', args first, ' '].
	argsString := firstArgString, ((args allButFirst collect: [:arg | arg, ': ', arg])  joinSeparatedBy: ' ').
	passedArgsString := (args, (defaultArgSpecs collect: [:argSpec | nil])) joinSeparatedBy: '. '."
	argStrings := self sourceStringsForArgSpecs: argSpecs defaultArgSpecs: defaultArgSpecs methodName: methodName.
	
	^ '{1}{2}
	
	^ self call: ''{3}'' with: \{{4}\}' format: {methodName. argStrings first. name.  argStrings second}