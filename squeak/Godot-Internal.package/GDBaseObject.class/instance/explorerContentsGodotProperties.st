explorer
explorerContentsGodotProperties
	
	^ self class allPropertyGetters collect: [:selector | | value |
			value := [self perform: selector]
				on: Error
				do: [:error | 'Failed to fetch property: ', error].
		
			ObjectExplorerWrapper
				with: value
				name: '#', selector
				model: self]