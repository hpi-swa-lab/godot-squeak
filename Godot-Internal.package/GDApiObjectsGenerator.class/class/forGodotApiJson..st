instance creation
forGodotApiJson: aPath

	^ self new apiReader: (GDGodotApiReader forFile: aPath)