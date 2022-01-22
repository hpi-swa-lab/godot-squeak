as yet unclassified
exampleListener
	" runs a process that fetches messages added to the external queue.
	Kill the process using Tools>ProcessBrowser "

	SystemChangeNotifier uniqueInstance noMoreNotificationsFor: Instance.
	Instance := nil.
	GDApi instance
		runMessageListenerHighPrio: false