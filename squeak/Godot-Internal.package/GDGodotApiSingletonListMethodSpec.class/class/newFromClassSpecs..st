instance creation
newFromClassSpecs: classSpecs

	^ (self newNamed: self methodName
		arguments: #()
		category: self methodCategory
		className: self className)
		singletonNames: (classSpecs collect: #singletonName)