sorting
sortedAndCategorizedDependencySequenceFor: classSpec dependencyInfo: dependencyInfo

	^ self sortedAndCategorizedDependencySequenceFor: classSpec
		categorized: (GDApiObjectsGenerator fullCategoryName: 'Uncategorized')
		dependencyInfo: dependencyInfo