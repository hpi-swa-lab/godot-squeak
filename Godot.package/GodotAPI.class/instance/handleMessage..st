message handling
handleMessage: aMessage

	messageHandlers at: aMessage type
		ifPresent: [ :handler | handler value: aMessage ]
		ifAbsent: [ self error: 'Unknown message type' ]