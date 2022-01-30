generating
generate

	| specs |
	specs := apiReader read.

	specs first
		do: #generate
		displayingProgress: [:classSpec | 'Generating {1}...' format: {classSpec name}].
	specs second
		do: #generate
		displayingProgress: [:methodSpec | 'Generating {1}>>{2}...' format: {methodSpec className. methodSpec name}].
	"| objectSpecs |
	objectSpecs := Json readFrom: stream.
	
	self withProgressSteps: objectSpecs size do: [
		objectSpecsByName := (objectSpecs collect: [:spec | (spec at: #name) asSymbol -> spec]) asDictionary.
	
		self determineDependencies: objectSpecs.
		topLevel do: [:spec | self generateObjectTree: spec inCategory: GDApiObjectsGenerator baseCategory]]"