instance creation
fromAPIJsonObject: aJsonObject  className: className

	^ (self newNamed: aJsonObject name
		arguments: #()
		category: self methodCategory
		className: className)
		getterName: aJsonObject getter
		