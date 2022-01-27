generating
withProgressDo: aBlock

	"TODO: A custom version of do:displayingProgress: would have a default UI"
	progress := ProgressMorph label: 'Generating Godot API objects...'.
	progress subLabel: 'Reading spec...'.
	progress openInWorld.
	aBlock ensure: [progress delete]