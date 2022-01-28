initialization
initialize

	super initialize.
	
	modificationListeningDisabled := false.
	
	pathToClass := Dictionary new.
	instances := PluggableDictionary new.
	instances hashBlock: [:val | val getHandle hash].

	messageHandlers := Dictionary new.
	messageHandlers at: GDMessageListener messageTypeNewScript put:
		[:message | self handleNewScript: message data newScriptMessage].
	messageHandlers at: GDMessageListener messageTypeScriptReload put:
		[:message | self handleScriptReload: (message data scriptReloadMessage)].
	messageHandlers at: GDMessageListener messageTypeNewInstance put:
		[:message | self handleNewInstance: (message data newInstanceMessage)].
	messageHandlers at: GDMessageListener messageTypeMethodCall put:
		[:message | self handleMethodCall: (message data methodCallMessage)].
	
	SystemChangeNotifier uniqueInstance
		notify: self
		ofAllSystemChangesUsing: #systemChangeEvent: