generating
generateObjectTree: objectName inCategory: categoryName

	| newCategoryName |
	newCategoryName := (GDApiObjectsGenerator categoryForObject: objectName) ifNil: [categoryName].
	
	self generateObject: (objectSpecsByName at: objectName) inCategory: newCategoryName.
	dependants at: objectName ifPresent: [:specs |
		specs do: [:spec | self generateObjectTree: spec inCategory: newCategoryName]].