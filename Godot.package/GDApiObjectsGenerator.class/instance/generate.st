generating
generate

	| objectSpecs |
	progress := ProgressMorph label: 'Generating Godot API objects...'.
	progress subLabel: 'Reading spec...'.
	progress openInWorld.
	
	objectSpecs := Json readFrom: stream.
	progressStep := 1.0 / objectSpecs size.
	
	objectSpecsByName := (objectSpecs collect: [:spec | (spec at: #name) asSymbol -> spec]) asDictionary.
	
	self determineDependencies: objectSpecs.
	topLevel do: [:spec | self generateObjectTree: spec inCategory: GDApiObjectsGenerator baseCategory].
	"objectSpecs do: [:spec |
		progress subLabel: spec name.
		self generateObject: spec.
		progress incrDone: progressStep]."
	
	progress delete.