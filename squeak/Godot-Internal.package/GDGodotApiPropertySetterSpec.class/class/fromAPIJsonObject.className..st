instance creation
fromAPIJsonObject: aJsonObject  className: className

	^ (self newNamed: aJsonObject name
		arguments: {GDArgumentSpec fromApiPropertiesJsonObject: aJsonObject}
		category: self methodCategory
		className: className)
		setterName: aJsonObject setter
		