as yet unclassified
fromGDVariant: aVariant

	| class className |
	className := (GDApi instance callMethod: 'get_class' onVariant: aVariant with: {}) unwrap.
	class := Smalltalk classNamed: (GDApiObjectsGenerator squeakClassName: className).
	^ class new internalVariant: aVariant
		