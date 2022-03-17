instance creation
forGodotApiJson: aJsonStream

	^ self new apiReader: (GDGodotApiReader forStream: aJsonStream)