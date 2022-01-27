generating
generateObject: aSpec inCategory: categoryName

	| class |
	progress subLabel: aSpec name.
	
	class := self generateObjectClass: (aSpec at: #name) parent: (aSpec at: #base_class) category: categoryName.
	(aSpec at: #methods) do: [:methodSpec | self generateMethod: methodSpec on: class].
	
	progress incrDone: progressStep.