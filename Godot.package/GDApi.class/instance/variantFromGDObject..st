ffi
variantFromGDObject: aGDObject

	| variant |
	variant := GDVariant allocate.
	self variant: variant insertObject: aGDObject.
	^ variant