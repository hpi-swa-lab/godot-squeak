message handling
processResponses

	[ | message |
		message := GDApi instance readMessageBlocking.
		
		message isFinishProcessing
			ifTrue: [^ message data]
			ifFalse: [self handleMessage: message]
	] repeat.