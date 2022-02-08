instance creation
newInstanceMethodNamed: methodName arguments: arguments category: category className: className apiName: apiName
	
	^ (self newNamed: methodName
		arguments: arguments
		category: category
		className: className)
		apiName: apiName