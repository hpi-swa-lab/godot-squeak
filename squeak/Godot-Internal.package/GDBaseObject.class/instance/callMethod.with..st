as yet unclassified
callMethod: aSymbol with: arguments

	| result |
	result := self perform: aSymbol withArguments: arguments.
	^ result == self ifTrue: [nil] ifFalse: [result]