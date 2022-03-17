as yet unclassified
allPropertyGetters

	^ (self allMethodsInCategory: GDGodotApiPropertyGetterSpec methodCategory)
		select: [:selector | selector isUnary].