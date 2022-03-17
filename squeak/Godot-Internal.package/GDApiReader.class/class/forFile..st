instance creation
forFile: aPath

	^ self forStream: (FileStream readOnlyFileNamed: aPath)