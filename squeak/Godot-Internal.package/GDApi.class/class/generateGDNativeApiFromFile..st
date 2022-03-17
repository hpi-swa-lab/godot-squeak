generating
generateGDNativeApiFromFile: aPath

	self generateGDNativeApi: (FileStream readOnlyFileNamed: aPath)