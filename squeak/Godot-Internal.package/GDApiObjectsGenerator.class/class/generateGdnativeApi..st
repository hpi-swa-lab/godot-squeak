using
generateGdnativeApi: aJsonStream

	(self new apiReader: (GDGdnativeApiReader forStream: aJsonStream)) generate