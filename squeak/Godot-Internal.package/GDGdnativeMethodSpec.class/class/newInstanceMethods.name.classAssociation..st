instance creation
newInstanceMethods: aJsonObject name: name classAssociation: classAssociation
	
	| methodName specs arguments apiName returnsVariant |
	"TODO: consider automatically converting GDStrings to Strings instead"
	methodName := name = 'asString' ifTrue: ['asGDString'] ifFalse: [name].
	arguments := aJsonObject arguments collectWithIndex: [:argTuple :index |
		GDArgumentSpec fromCJsonTuple: argTuple position: index].
	apiName := self apiNameFromJsonObject: aJsonObject.
	returnsVariant := aJsonObject return_type beginsWith: 'godot_variant'.

	specs := OrderedCollection with: (
		GDGdnativeInstanceMethodSpec newInstanceMethodNamed: methodName
			arguments: arguments
			category: self methodCategory
			className: classAssociation value
			apiName: apiName
			returnsVariant: returnsVariant).
	
	(#(get set) anySatisfy: [:prefix | methodName beginsWith: prefix]) ifTrue: [specs add: (
		GDGdnativeInstanceMethodSpec newInstanceMethodNamed: (methodName allButFirst: 3) withFirstCharacterDownshifted
			arguments: arguments
			category: self propertyCategory
			className: classAssociation value
			apiName: apiName
			returnsVariant: returnsVariant)].
	
	(methodName beginsWith: 'operator') ifTrue: [specs addAll: (
		self newInstanceOperatorNamed: (methodName allButFirst: 8) withFirstCharacterDownshifted
		arguments: arguments
		className: classAssociation value
		apiName: apiName)].
	
	^ specs