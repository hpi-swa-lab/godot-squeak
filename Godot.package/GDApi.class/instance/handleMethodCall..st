message handling
handleMethodCall: methodCallData

	| result |
	GDApi showln: 'METHOD CALLED: ', methodCallData methodName fromCString, ' -- ARG COUNT: ', methodCallData argCount.
	result := (instances at: methodCallData owner)
		callMethod: (self convertToSelector: methodCallData methodName fromCString)
		with: (methodCallData allArgsAsVariants collect: #unwrap).
	GDApi showln: 'METHOD RESULT: ', result class, ' ', result.
	result insertIntoGDVariant: methodCallData result.
	^ ExternalAddress new