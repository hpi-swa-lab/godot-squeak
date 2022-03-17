explorer
explorerContents
	
	^ self explorerContentsChildren,
		self explorerContentsSceneTree,
		super explorerContents,
		self explorerContentsProperties