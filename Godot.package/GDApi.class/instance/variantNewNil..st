ffi
variantNewNil: aVariant
	<apicall: void godot_variant_new_nil(GDVariant*)>
	^ self externalCallFailed