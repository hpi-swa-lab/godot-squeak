ffi
variantToBoolean: aVariant
	<apicall: bool godot_variant_bool_extract(GDVariant*)>
	^ self externalCallFailed