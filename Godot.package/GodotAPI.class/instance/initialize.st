initialization
initialize

	super initialize.
	
	pathToClass := Dictionary new.
	instances := PluggableDictionary new.
	instances hashBlock: [:val | val getHandle hash].

	messageHandlers := Dictionary new.
	messageHandlers at: GodotAPI messageTypeNewScript put:
		[:message | self handleNewScript: message data newScriptMessage].
	messageHandlers at: GodotAPI messageTypeScriptReload put:
		[:message | self handleScriptReload: (message data scriptReloadMessage)].
	messageHandlers at: GodotAPI messageTypeNewInstance put:
		[:message | self handleNewInstance: (message data newInstanceMessage)].
	messageHandlers at: GodotAPI messageTypeMethodCall put:
		[:message | self handleMethodCall: (message data methodCallMessage)].
	
	SystemChangeNotifier uniqueInstance
		notify: self
		ofAllSystemChangesUsing: #systemChangeEvent: