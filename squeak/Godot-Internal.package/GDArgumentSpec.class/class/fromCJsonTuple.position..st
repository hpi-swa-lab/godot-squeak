instance creation
fromCJsonTuple: aTuple position: aNumber

	| argName argType |
	argName := (aTuple second copyAfter: $_).
	argType := ((aTuple first findTokens) reject: [:string | string = 'const' or: string = '*']) first.

	^ self new
		name: argName;
		type: argType;
		suffix: aNumber asString;
		isSelf: (argName includesSubstring: 'self')
	