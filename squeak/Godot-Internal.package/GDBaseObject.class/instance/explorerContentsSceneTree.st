explorer
explorerContentsSceneTree

	^ (self respondsTo: #getTree)
		ifTrue: [
			{ObjectExplorerWrapper
				with: (self getTree)
				name: 'Scene Tree'
				model: self}]
		ifFalse: [{}]