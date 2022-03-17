using
generateGdnativeApiForFile: aPath

	(self new apiReader: (GDGdnativeApiReader forStream: (FileStream readOnlyFileNamed: aPath))) generate