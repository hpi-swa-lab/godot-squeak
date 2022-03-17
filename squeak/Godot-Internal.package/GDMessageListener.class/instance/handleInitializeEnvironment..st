message handling
handleInitializeEnvironment: initializeEnvironmentData

	GDApi initializeGlobals.
	initializeEnvironmentData inEditor ifFalse: [self currentWorld setAsBackground: Color cayenne].
	
	^ ExternalAddress new