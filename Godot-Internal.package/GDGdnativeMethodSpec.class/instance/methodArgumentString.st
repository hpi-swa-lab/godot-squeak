as yet unclassified
methodArgumentString

	| firstArg methodArguments |
	methodArguments := arguments.
	methodArguments first isSelf ifTrue:
		[methodArguments := methodArguments allButFirst].
	
	methodArguments isEmpty ifTrue: [^ ''].
	
	firstArg := methodArguments first methodArgumentString capitalized.
	(self nameString asLowercase endsWith: (firstArg copyUpTo: $:) asLowercase) ifTrue: [
		firstArg := ':', (firstArg copyAfter: $:)].
	
	^ ({firstArg}, (methodArguments allButFirst collect: #methodArgumentString)) joinSeparatedBy: ' '