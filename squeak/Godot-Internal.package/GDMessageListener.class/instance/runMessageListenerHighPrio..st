message handling
runMessageListenerHighPrio: aBoolean

	[
		[ | message |
			message := GDApi instance readMessage.
			message isNull ifFalse: [self handleMessage: message].
			Processor yield
		] repeat
	] forkAt: (aBoolean ifFalse: [Processor userBackgroundPriority] ifTrue: [Processor highIOPriority]) named: '[Godot message listener]'