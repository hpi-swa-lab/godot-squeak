ffi
variant: aVariant insertInt: anInteger
	<apicall: void godot_variant_int_insert(GDVariant*, int64_t)>
	^ self externalCallFailed