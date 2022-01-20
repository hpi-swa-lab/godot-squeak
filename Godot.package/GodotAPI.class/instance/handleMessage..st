message handling
handleMessage: aMessage

	"returning values here may not be useful"
	^ messageHandlers at: aMessage type
		ifPresent: [ :handler | handler value: aMessage ]
		ifAbsent: [ self error: 'Unknown message type' ]