as yet unclassified
exampleListener
	" runs a process that fetches messages added to the external queue.
	Kill the process using Tools>ProcessBrowser "

	GodotAPI new
		initSqueakCallbacks: nil;
		runMessageListenerHighPrio: false