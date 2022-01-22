ffi
getVariantType: aVariant
	<apicall: int godot_get_variant_type(GDVariant*)>
	^ self externalCallFailed