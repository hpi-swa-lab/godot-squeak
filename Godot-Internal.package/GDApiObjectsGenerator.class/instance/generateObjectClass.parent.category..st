generating
generateObjectClass: className parent: parentName category: categoryName

	| parentClass |
	parentClass := (Smalltalk classNamed: (GDApiObjectsGenerator squeakClassName: parentName)) ifNil: [GDBaseObject].
	^ parentClass subclass: (GDApiObjectsGenerator squeakClassName: className) asSymbol
		instanceVariableNames: ''
		classVariableNames: ''
		poolDictionaries: ''
		category: categoryName.