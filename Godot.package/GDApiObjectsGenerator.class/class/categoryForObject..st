as yet unclassified
categoryForObject: aSymbol

	^ ObjectCategories at: aSymbol
		ifPresent: [:category | self fullCategoryName: category]
		ifAbsent: [nil]