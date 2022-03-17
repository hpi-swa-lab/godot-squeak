*Godot-Internal
asGDString

	| string |
	string := GDString allocateExternal.
	self asString insertIntoGDString: string.
	^ string