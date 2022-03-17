explorer
asExplorerString

	^ (self respondsTo: #getName)
		ifTrue: [self getName, ' (', self class godotClass, ')']
		ifFalse: [super asExplorerString]