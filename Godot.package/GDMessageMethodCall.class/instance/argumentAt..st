as yet unclassified
argumentAt: anIndex

	self assert: [anIndex <= self argCount].
	^ (self args setContentType: ExternalType void asPointerType; at: 1)
		setContentType: GDVariant externalType;
		yourself