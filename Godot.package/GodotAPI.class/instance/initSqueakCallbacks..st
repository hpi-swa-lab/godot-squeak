as yet unclassified
initSqueakCallbacks: aBlock
	<apicall: void init_squeak_callbacks(FFICallback*)>
	^ self externalCallFailed