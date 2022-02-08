instance creation
newInstanceMethods: aJsonObject name: name classAssociation: classAssociation
	
	| methodName specs arguments apiName |
	"TODO: consider automatically converting GDStrings to Strings instead"
	methodName := name = 'asString' ifTrue: ['asGDString'] ifFalse: [name].
	arguments := aJsonObject arguments collectWithIndex: [:argTuple :index |
		GDArgumentSpec fromCJsonTuple: argTuple position: index].
	"TODO: this name is defined by GDApi>>generateGDNativeApi. refactoring necessary"
	apiName := ((aJsonObject name subStrings: '_')
		allButFirst collect: #capitalized)
		join withFirstCharacterDownshifted.

	specs := OrderedCollection with: (
		self newInstanceMethodNamed: methodName
			arguments: arguments
			category: self methodCategory
			className: classAssociation value
			apiName: apiName).
	
	(#(get set) anySatisfy: [:prefix | methodName beginsWith: prefix]) ifTrue: [specs add: (
		self newInstanceMethodNamed: (methodName allButFirst: 3) withFirstCharacterDownshifted
			arguments: arguments
			category: self propertyCategory
			className: classAssociation value
			apiName: apiName)].
	
	"(methodName beginsWith: 'operator') ifTrue: [specs addAll: (
		self newInstanceOperatorNamed: (methodName allButFirst: 8) withFirstCharacterDownshifted
		arguments: arguments
		className: classAssociation value
		apiName: apiName)]."
	
	^ specs