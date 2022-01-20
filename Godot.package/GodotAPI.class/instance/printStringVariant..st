ffi
printStringVariant: aVariant
	"This could be solved better by using godot_string directly instead of godot_variant"
	<apicall: void godot_print_variant(GDVariant*)>
	^ self externalCallFailed