utility
variantFromGDObject: aGDObject

	| variant |
	variant := GDVariant allocate.
	self variantNewObject: variant with: aGDObject.
	^ variant