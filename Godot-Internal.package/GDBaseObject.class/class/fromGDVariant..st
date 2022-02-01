as yet unclassified
fromGDVariant: aVariant

	| class className baseObject |
	className := (self class callMethod: 'get_class' onVariant: aVariant with: {}) unwrap.
	class := Smalltalk classNamed: (GDApiObjectsGenerator squeakClassName: className).
	baseObject := class newWithEmptyVariant.
	GDApi instance variantNewCopy: baseObject internalVariant with: aVariant.
	^ baseObject
		