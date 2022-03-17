generating
parentClass

	^ (Smalltalk classNamed: parentName)
		ifNil: [self error: ('Parent class {1} for {2} does not exist' format: {parentName. name})]