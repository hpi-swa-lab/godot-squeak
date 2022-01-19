as yet unclassified
exampleListener
	" runs a process that fetches messages added to the external queue.
	Kill the process using Tools>ProcessBrowser "

	Instance := nil.
	GodotAPI instance
		runMessageListenerHighPrio: false