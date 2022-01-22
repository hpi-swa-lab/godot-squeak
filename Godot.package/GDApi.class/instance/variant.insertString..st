ffi
variant: aVariant insertString: aString
	<apicall: void godot_variant_new_string_with_value(GDVariant*, char*)>
	^ self externalCallFailed