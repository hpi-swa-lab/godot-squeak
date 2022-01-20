as yet unclassified
initialize

	VariantClasses := Dictionary withAll: {
		self variantNil -> UndefinedObject.
		self variantBoolean -> Boolean.
		self variantInteger -> Integer.
		self variantFloat -> Float.
		self variantString -> String.
	}