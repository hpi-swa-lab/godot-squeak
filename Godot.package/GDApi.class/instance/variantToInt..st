ffi
variantToInt: aVariant
	<apicall: int64_t godot_variant_int_extract(GDVariant*)>
	^ self externalCallFailed