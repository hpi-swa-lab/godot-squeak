ffi
variant: aVariant insertBoolean: aBoolean
	<apicall: void godot_variant_bool_insert(GDVariant*, bool)>
	^ self externalCallFailed