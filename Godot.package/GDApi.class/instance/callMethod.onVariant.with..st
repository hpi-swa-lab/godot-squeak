ffi
callMethod: methodName onVariant: aVariant with: aSequence

	"TODO: All of the variants and strings here need to be destroyed"
	| callError result methodNameGDString |
	callError := GDVariantCallError allocate.

	methodNameGDString := GDString allocate.
	methodName insertIntoGDString: methodNameGDString.
		
	"argVariants := GDVariant allocate: aSequence size.
	argVariantPointers := ExternalType void asPointerType allocate: aSequence size.
	aSequence withIndexDo: [:arg :index || variant |
		variant := argVariants at: index.
		arg insertIntoGDVariant: variant.
		(argVariantPointers) at: index put: variant]."
	
	result := self callMethodOnVariant: aVariant
		name: methodNameGDString
		args: (ExternalType int asPointerType allocate)
		argCount: aSequence size
		err: callError.
	callError errorType ~= GDApi variantCallErrorOk ifTrue: [self error: 'Variant method call failed with ', callError errorType].
	^ result.