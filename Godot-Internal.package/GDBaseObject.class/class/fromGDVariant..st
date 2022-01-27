as yet unclassified
fromGDVariant: aVariant

	| class className baseObject |
	className := (GDApi instance callMethod: 'get_class' onVariant: aVariant with: {}) unwrap.
	class := Smalltalk classNamed: (GDApiObjectsGenerator squeakClassName: className).
	baseObject := class newWithEmptyVariant.
	GDApi instance variantNewCopy: baseObject internalVariant with: aVariant.
	^ baseObject
		