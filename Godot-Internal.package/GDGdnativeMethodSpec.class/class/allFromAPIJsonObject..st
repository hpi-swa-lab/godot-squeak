instance creation
allFromAPIJsonObject: aJsonObject

	| classAssociation methodName |
	classAssociation := self associationForCType: aJsonObject name.
	classAssociation ifNil: [^ #()].

	methodName := self nameFor: aJsonObject name withoutPrefix: classAssociation key.
	
	(methodName beginsWith: 'new') ifTrue: [
		^ #().
		"^ {self newConstructor: aJsonObject name: methodName classAssociation: classAssociation}"].
	
	^ self newInstanceMethods: aJsonObject name: methodName classAssociation: classAssociation
		