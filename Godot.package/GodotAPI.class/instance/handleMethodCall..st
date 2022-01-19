message handling
handleMethodCall: methodCallData

	Transcript showln: methodCallData methodName fromCString.
	Transcript showln: methodCallData argCount.
	self callMethod: (self convertToSelector: methodCallData methodName fromCString)
		with: methodCallData allArgsAsVariants
		on: (instances at: methodCallData owner).
	^ ExternalAddress new