sqplugin
printString: aString
	<apicall: void godot_print_string(char*)>
	^ self externalCallFailed