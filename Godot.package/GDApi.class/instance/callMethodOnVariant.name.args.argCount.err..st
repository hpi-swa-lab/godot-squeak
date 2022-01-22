ffi
callMethodOnVariant: aGDVariant name: aGDString args: aGDVariantPointerArray argCount: anInteger err: aGDErrorStruct
	<apicall: GDVariant godot_variant_call(GDVariant*, GDString*, void*, int, GDVariantCallError*)>
	^ self externalCallFailed