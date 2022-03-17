as yet unclassified
argumentAt: anIndex

	self assert: [anIndex <= self argCount].
	^ GDVariant fromHandle: (self args setContentType: ExternalType void asPointerType; at: anIndex) getHandle