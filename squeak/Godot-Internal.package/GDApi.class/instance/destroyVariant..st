sqplugin
destroyVariant: aVariant
	<apicall: void godot_variant_destroy(GDVariant*)>
	^ self externalCallFailed