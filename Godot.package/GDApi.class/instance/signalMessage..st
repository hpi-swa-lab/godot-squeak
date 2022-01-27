sqplugin
signalMessage: aMessage
	<apicall: void signal_message(GDMessage*)>
	^ self externalCallFailed