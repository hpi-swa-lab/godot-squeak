ffi
gdString: aGDString insertString: aString
	<apicall: void godot_string_new_with_value(GDString*, char*)>
	^ self externalCallFailed