sqplugin
sendMessage: aMessage
	<apicall: void squeak_send_message(GDMessage*)>
	^ self externalCallFailed