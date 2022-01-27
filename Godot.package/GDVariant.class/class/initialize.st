class initialization
initialize

	VariantClasses := Dictionary withAll: {
		self variantNil -> UndefinedObject.
		self variantBoolean -> Boolean.
		self variantInteger -> Integer.
		self variantFloat -> Float.
		self variantString -> String.
		self variantVector2 -> GDVector2.
		self variantObject -> GDBaseObject.
	}