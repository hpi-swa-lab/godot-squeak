message handling
handleMethodCall: methodCallData

	| result |
	GodotAPI showln: 'method called: ', methodCallData methodName fromCString, ' ; arg count: ', methodCallData argCount.
	result := self callMethod: (self convertToSelector: methodCallData methodName fromCString)
		with: (methodCallData allArgsAsVariants collect: #unwrap)
		on: (instances at: methodCallData owner).
	GodotAPI showln: 'result ', result class, ' ', result.
	result putIntoGDVariant: methodCallData result.
	^ ExternalAddress new