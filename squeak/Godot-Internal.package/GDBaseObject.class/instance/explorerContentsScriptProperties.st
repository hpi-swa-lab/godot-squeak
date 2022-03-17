explorer
explorerContentsScriptProperties
	
	^ self getScript
		ifNil: [{}]
		ifNotNil: [:script | | properties propertyArray |
			properties := script getScriptPropertyList.
			propertyArray := Array ofSize: properties size.
			0 to: (properties size - 1) do: [:idx |
				propertyArray at: idx + 1 put: (properties getIdx: idx)].
			propertyArray collect: [:property | | name |
				name := property getKey: 'name'.
				ObjectExplorerWrapper
					with: (self getProperty: name)
					name: '#', name
					model: self]]