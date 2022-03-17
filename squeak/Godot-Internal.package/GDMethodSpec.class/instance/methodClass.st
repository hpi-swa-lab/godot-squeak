accessing
methodClass

	| methodClass |
	methodClass := Smalltalk classNamed: className.
	^ self isClassMethod
		ifTrue: [methodClass class]
		ifFalse: [methodClass]