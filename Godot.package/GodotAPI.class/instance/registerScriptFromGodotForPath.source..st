as yet unclassified
registerScriptFromGodotForPath: aPath source: aSource
	<callback: void (*)(GodotString*, GodotString*)>
	self registerScriptForPath: (aPath extractString) source: (aSource extractString)