ffi
destroyString: aVariant
	<apicall: void godot_string_destroy(GDString*)>
	^ self externalCallFailed