instance creation
forGodotApiJson: aPath

	^ self new apiReader: (GDGodotApiReader forJson: aPath)