explorer
explorerContentsChildren

	^ (self respondsTo: #getChildren)
		ifTrue: [ | children childrenArray |
			children := self getChildren.
			childrenArray := Array ofSize: children size.
			(0 to: (children size - 1)) do: [:idx |
				childrenArray at: idx + 1 put: (children getIdx: idx)].
			{ObjectExplorerWrapper
				with: childrenArray
				name: 'Children'
				model: self}]
		ifFalse: [{}]