generating
methodArgumentString

	| firstArg |
	arguments isEmpty ifTrue: [^ ''].
	
	firstArg := arguments first methodArgumentString capitalized.
	(self nameString endsWith: (firstArg copyUpTo: $:)) ifTrue: [
		firstArg := ':', (firstArg copyAfter: $:)].
	
	^ ({firstArg}, (arguments allButFirst collect: #methodArgumentString)) joinSeparatedBy: ' '