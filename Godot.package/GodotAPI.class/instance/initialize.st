initialization
initialize

	super initialize.

	messageHandlers := Dictionary new.
	messageHandlers at: self messageTypeNewScript put: [:message | self handleNewScript: (message data newScriptMessage)].
	messageHandlers at: self messageTypeScriptReload put: [:message | self handleScriptReload: (message data scriptReloadMessage)].
	messageHandlers at: self messageTypeNewInstance put: [:message | self handleNewInstance: (message data newInstanceMessage)].
	messageHandlers at: self messageTypeMethodCall put: [:message | self handleMethodCall: (message data methodCallMessage)].