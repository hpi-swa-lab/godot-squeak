instance creation
forJson: aPath

	^ self forStream: (FileStream readOnlyFileNamed: aPath)