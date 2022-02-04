initialization
initialize

	super initialize.
	
	modificationListeningDisabled := false.
	
	pathToClass := Dictionary new.
	instances := PluggableDictionary new.
	instances hashBlock: [:val | val getHandle hash].

	messageHandlers := Dictionary withAll: {
		GDMessage typeSqueakNewScript -> [:message | self handleNewScript: (GDMessageNewScript fromHandle: message data getHandle)].
		GDMessage typeSqueakScriptReload -> [:message | self handleScriptReload: (GDMessageScriptReload fromHandle: message data getHandle)].
		GDMessage typeSqueakNewInstance -> [:message | self handleNewInstance: (GDMessageNewInstance fromHandle: message data getHandle)].
		GDMessage typeSqueakFunctionCall -> [:message | self handleMethodCall: (GDMessageMethodCall fromHandle: message data getHandle)].
		GDMessage typeSqueakSetProperty -> [:message | self handleSetProperty: (GDSetProperty fromHandle: message data getHandle)].
		GDMessage typeSqueakGetProperty -> [:message | self handleGetProperty: (GDGetProperty fromHandle: message data getHandle)].
	}.
	
	SystemChangeNotifier uniqueInstance
		notify: self
		ofAllSystemChangesUsing: #systemChangeEvent: