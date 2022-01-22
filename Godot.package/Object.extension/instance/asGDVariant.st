*Godot
asGDVariant

	"NOTE: A GDVariant created this way will likely need to be destroyed within Squeak (godot_variant_destroy)."
	| variant |
	variant := GDVariant allocate.
	self insertIntoGDVariant: variant.
	^ variant