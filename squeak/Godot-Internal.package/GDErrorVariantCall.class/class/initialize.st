class initialization
initialize

	super initialize.

	TypeToSubclass := Dictionary withAll: {
		self typeInvalidMethod -> GDErrorVariantCallInvalidMethod.
		self typeInvalidArgument -> GDErrorVariantCallInvalidArgument.
		self typeTooManyArguments -> GDErrorVariantCallTooManyArguments.
		self typeTooFewArguments -> GDErrorVariantCallTooFewArguments.
		self typeInstanceIsNull -> GDErrorVariantCallInstanceIsNull.
	}