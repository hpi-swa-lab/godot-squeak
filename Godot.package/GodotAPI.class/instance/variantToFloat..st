ffi
variantToFloat: aVariant
	<apicall: double godot_variant_real_extract(GDVariant*)>
	^ self externalCallFailed