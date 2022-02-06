*Godot-Internal
asGDVariantAllocateExternal: aBoolean

	| variant |
	variant := aBoolean
		ifTrue: [GDVariant allocateExternal]
		ifFalse: [GDVariant allocate].
	self insertIntoGDVariant: variant.
	^ variant