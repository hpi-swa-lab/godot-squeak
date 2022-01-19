ffi
tempGodotPrintVariant: aVariant
	<apicall: void godot_print_variant(GDVariant*)>
	^ self externalCallFailed