as yet unclassified
explorerContents
	
	"TODO: The proper solution is to let all builtins inherit from a common class with this method"
	^ super explorerContents, (
		(self class allMethodsInCategory: GDGdnativeMethodSpec propertyCategory)
			select: [:selector | selector isUnary]
			thenCollect: [:selector |
				ObjectExplorerWrapper
					with: (self perform: selector)
					name: selector
					model: self])