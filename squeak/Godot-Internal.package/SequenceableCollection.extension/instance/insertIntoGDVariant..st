*Godot-Internal
insertIntoGDVariant: aVariant

	| array |
	array := GDArray allocateExternal.
	GDApi instance arrayNew: array.
	
	self do: [:element | | variant |
		variant := element asGDVariantAllocateExternal.
		GDApi instance arrayPushBack: array with: variant].
	
	GDApi instance variantNewArray: aVariant with: array.