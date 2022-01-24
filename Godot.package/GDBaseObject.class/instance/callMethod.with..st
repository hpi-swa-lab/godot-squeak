as yet unclassified
callMethod: aSymbol with: arguments

	| result |
	result := [self perform: aSymbol withArguments: arguments]
		on: MessageNotUnderstood
		do: [:exception | Transcript showln: 'Encountered exception while trying to call ', aSymbol, ' on ', self class, ': ', exception. nil].
	
	^ result == self ifTrue: [nil] ifFalse: [result]