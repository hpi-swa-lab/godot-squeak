using
generateFromSpec: aPath

	"aPath should point to the api.json file that is delivered with godot-headers"
	self new stream: (FileStream readOnlyFileNamed: aPath); generate