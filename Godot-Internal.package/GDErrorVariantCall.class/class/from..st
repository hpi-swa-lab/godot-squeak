instance creation
from: aGDVariantCallError

	^ TypeToSubclass at: aGDVariantCallError errorType
		ifPresent: [:subclass | subclass new]
		ifAbsent: [nil]