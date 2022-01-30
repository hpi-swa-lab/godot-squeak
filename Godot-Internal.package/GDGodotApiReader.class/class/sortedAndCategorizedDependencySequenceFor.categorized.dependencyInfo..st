sorting
sortedAndCategorizedDependencySequenceFor: classSpec categorized: category dependencyInfo: dependencyInfo

	classSpec category: (ObjectCategories at: classSpec name ifAbsent: [category]).
	
	^ {classSpec. (dependencyInfo at: classSpec ifAbsent: [{}]) collect: [:childClassSpec |
			self sortedAndCategorizedDependencySequenceFor: childClassSpec
				categorized: classSpec category
				dependencyInfo: dependencyInfo]}