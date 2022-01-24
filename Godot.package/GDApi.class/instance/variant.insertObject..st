ffi
variant: aVariant insertObject: aGDObject
	<apicall: void godot_variant_object_insert(GDVariant*, GDObject*)>
	^ self externalCallFailed