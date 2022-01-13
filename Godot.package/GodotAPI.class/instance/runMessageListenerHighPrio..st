message handling
runMessageListenerHighPrio: aBoolean

	[
		[ | message |
			message := self readMessage.
			message isNull ifFalse: [
				message result: (self handleMessage: message).
				self signalMessage: message
				"Transcript showln: message type.
				message type = self messageTypeFunctionCall ifTrue: [
					Transcript showln: 'got function call'.
					]."
				
				"m type = self messageTypeInstance ifTrue: [ | payload result |
					payload := (m data asType: GDMessageInstanceData externalType) value.
					Transcript showln: payload request str fromCString.
					result := payload request a + payload request b.
					payload response result: result].
				m type = self messageTypeSignal ifTrue: [Transcript showln: 'signal received']."].
			Processor yield] repeat]
	forkAt: (aBoolean ifFalse: [Processor userBackgroundPriority] ifTrue: [Processor highIOPriority]) named: '[Godot message listener]'