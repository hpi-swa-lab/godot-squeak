message handling
runMessageListener

	[
		[ | m |
			m := GDStartup new readMessage.
			m isNull ifFalse: [
				m type = self messageTypeInstance ifTrue: [ | payload result |
					payload := (m data asType: GDMessageInstanceData externalType) value.
					Transcript showln: payload request str fromCString.
					result := payload request a + payload request b.
					
					" now that we have read all data, we can safely write into the union: "
					payload response result: result].
				m type = self messageTypeSignal ifTrue: [Transcript showln: 'signal received'].
				self signalMessage: m].
			Processor yield] repeat]
	forkAt: (true ifTrue: [Processor userBackgroundPriority] ifFalse: [Processor highIOPriority]) named: '[Godot message listener]'