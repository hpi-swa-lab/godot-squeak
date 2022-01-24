message handling
runMessageListenerHighPrio: aBoolean

	"TODO: split all message handling off into a separate class"
	[
		[ | message |
			message := self readMessage.
			message isNull ifFalse: [
				message result: (self handleMessage: message).
				self signalMessage: message].
			Processor yield] repeat]
	forkAt: (aBoolean ifFalse: [Processor userBackgroundPriority] ifTrue: [Processor highIOPriority]) named: '[Godot message listener]'