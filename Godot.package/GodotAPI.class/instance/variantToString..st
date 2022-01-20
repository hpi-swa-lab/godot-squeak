ffi
variantToString: aVariant
	<apicall: char* godot_variant_string_to_c_str(GDVariant*)>
	^ self externalCallFailed