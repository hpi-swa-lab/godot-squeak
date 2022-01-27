generating
generate

	| objectSpecs |
	self withProgressDo: [
		objectSpecs := Json readFrom: stream.
		progressStep := 1.0 / (objectSpecs size + 1).
		progress incrDone: progressStep.
	
		objectSpecsByName := (objectSpecs collect: [:spec | (spec at: #name) asSymbol -> spec]) asDictionary.
	
		self determineDependencies: objectSpecs.
		topLevel do: [:spec | self generateObjectTree: spec inCategory: GDApiObjectsGenerator baseCategory]]