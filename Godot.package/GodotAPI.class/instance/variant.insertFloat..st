ffi
variant: aVariant insertFloat: aFloat
	<apicall: void godot_variant_real_insert(GDVariant*, double)>
	^ self externalCallFailed