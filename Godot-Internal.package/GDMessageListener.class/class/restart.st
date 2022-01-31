listening
restart

	SystemChangeNotifier uniqueInstance noMoreNotificationsFor: Instance.
	Instance := nil.
	self startListening.