as yet unclassified
fromGDVariant: aVariant

	| class className baseObject |
	[className := (self callMethod: 'get_class' onVariant: aVariant with: {}) unwrap]
		on: GDErrorVariantCallInstanceIsNull
		do: [^ UndefinedObject fromGDVariant: aVariant].
	class := Smalltalk classNamed: (GDApiObjectsGenerator squeakClassName: className).
	baseObject := class newWithEmptyVariant.
	GDApi instance variantNewCopy: baseObject internalVariant with: aVariant.
	^ baseObject
		