instance creation
newInstanceMethodNamed: methodName arguments: arguments category: category className: className apiName: apiName returnsVariant: returnsVariant

	^ (self newNamed: methodName
		arguments: arguments
		category: category
		className: className)
		apiName: apiName;
		returnsVariant: returnsVariant