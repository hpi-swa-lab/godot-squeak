initialization
initialize

	super initialize.
	
	scriptLoadingActive := false.
	
	pathToClass := Dictionary new.
	instances := PluggableDictionary new.
	instances hashBlock: [:val | val getHandle hash].

	messageHandlers := Dictionary new.
	messageHandlers at: GDApi messageTypeNewScript put:
		[:message | self handleNewScript: message data newScriptMessage].
	messageHandlers at: GDApi messageTypeScriptReload put:
		[:message | self handleScriptReload: (message data scriptReloadMessage)].
	messageHandlers at: GDApi messageTypeNewInstance put:
		[:message | self handleNewInstance: (message data newInstanceMessage)].
	messageHandlers at: GDApi messageTypeMethodCall put:
		[:message | self handleMethodCall: (message data methodCallMessage)].
	
	SystemChangeNotifier uniqueInstance
		notify: self
		ofAllSystemChangesUsing: #systemChangeEvent: