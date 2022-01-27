utility
callMethod: methodName onVariant: aVariant with: argsSequence

	"TODO: All of the variants and strings here need to be destroyed"
	"TODO: this a chunky func, make it a less chunky func"
	| callError result methodNameGDString variants argsPointer |
	methodNameGDString := GDString allocate.
	methodName insertIntoGDString: methodNameGDString.
	
	variants := GDVariant allocateExternal: argsSequence size.
	argsPointer := ExternalType void asPointerType allocate: argsSequence size.
	argsSequence withIndexDo: [:arg :index || variant |
		variant := variants at: index.
		arg insertIntoGDVariant: variant.
		argsPointer at: index put: variant getHandle].
	
	callError := GDVariantCallError allocate.
	
	result := self variantCall: aVariant
		with: methodNameGDString
		with: argsPointer
		with: argsSequence size
		with: callError.
		
	callError errorType ~= GDApi variantCallErrorOk ifTrue: [self error: 'Variant method call failed with ', callError errorType].
	
	"TODO: should probably use an ensure here"
	methodNameGDString destroy.
	variants do: #destroy.
	argsPointer free.
	
	^ result.