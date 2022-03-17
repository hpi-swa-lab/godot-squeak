sorting
sortAndCategorizeClassSpecs: aClassSpecsSequence

	| dependencyInfo topLevel specsByName |
	topLevel := OrderedCollection new.
	dependencyInfo := Dictionary new.
	specsByName := Dictionary withAll: (aClassSpecsSequence collect: [:spec | spec name -> spec]).
	
	aClassSpecsSequence do: [:spec |
		spec parentName = GDClassSpec rootClassName
			ifTrue: [
				topLevel add: spec]
			ifFalse: [
				(dependencyInfo at: (specsByName at: spec parentName) ifAbsentPut: [OrderedCollection new]) add: spec]].
	
	^ (topLevel collect: [:topSpec | self sortedAndCategorizedDependencySequenceFor: topSpec dependencyInfo: dependencyInfo]) flatten