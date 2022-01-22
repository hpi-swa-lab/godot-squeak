generating
allSourcesFor: aMethodSpec

	| name sources argSpecs defaultArgSpecs |
	sources := OrderedCollection new.
	name := (aMethodSpec at: 'name').
	argSpecs := (aMethodSpec at: 'arguments') asOrderedCollection.
	defaultArgSpecs := OrderedCollection new.
	
	sources add: (self sourceFor: name args: argSpecs defaultArgs: {}).
	[argSpecs notEmpty and: [argSpecs last at: 'has_default_value']] whileTrue: [
		defaultArgSpecs addFirst: argSpecs removeLast.
		sources add: (self sourceFor: name args: argSpecs defaultArgs: defaultArgSpecs).].
		
	^ sources