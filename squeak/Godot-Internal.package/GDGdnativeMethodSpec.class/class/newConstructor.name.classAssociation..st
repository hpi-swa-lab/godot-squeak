instance creation
newConstructor: aJsonObject name: methodName classAssociation: classAssociation

	| apiName arguments |
	arguments := aJsonObject arguments allButFirst withIndexCollect: [:argTuple :index |
		GDArgumentSpec fromCJsonTuple: argTuple position: index].
	apiName := self apiNameFromJsonObject: aJsonObject.
	
	^ (GDGdnativeConstructorMethodSpec
		newNamed: methodName
		arguments: arguments
		category: self constructorCategory
		className: classAssociation value)
		apiName: apiName