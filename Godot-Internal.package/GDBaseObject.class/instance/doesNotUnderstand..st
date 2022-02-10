as yet unclassified
doesNotUnderstand: aMessage

	self internalVariant ifNil: [super doesNotUnderstand: aMessage].
	
	"Try godot functions first, then squeak functions."
	[[^ self callOnVariant: (aMessage selector copyUpTo: $:) with: aMessage arguments]
		on: GDErrorVariantCall
		do: [:error |
			error isInvalidMethod ifFalse: [error signal].
			^ self callOnVariant: (aMessage selector copyReplaceAll: ':' with: '_') with: aMessage arguments]]
		on: GDErrorVariantCallInvalidMethod
		do: [super doesNotUnderstand: aMessage]