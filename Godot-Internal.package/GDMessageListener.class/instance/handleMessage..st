message handling
handleMessage: aMessage

	| responseData |
	responseData := messageHandlers at: aMessage type
		ifPresent: [ :handler | handler value: aMessage ]
		ifAbsent: [ self error: 'Unknown message type' ].
	
	^ self createDataResponse: responseData