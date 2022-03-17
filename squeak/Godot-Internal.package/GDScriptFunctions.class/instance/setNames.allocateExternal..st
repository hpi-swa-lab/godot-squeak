as yet unclassified
setNames: aSequence allocateExternal: aBoolean

	aBoolean ifFalse: [self notYetImplemented].

	self numNames: aSequence size.
	self names: (ExternalType void asPointerType allocateExternal: self numNames).
	aSequence withIndexDo: [:name :index || externalString |
		externalString := (ExternalType char allocateExternal: name size + 1) toCString: name.
		self names setContentType: ExternalType void asPointerType; at: index put: externalString getHandle].