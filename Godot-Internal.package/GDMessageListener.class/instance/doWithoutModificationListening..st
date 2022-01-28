file synchronization
doWithoutModificationListening: aBlock

	modificationListeningDisabled := true.
	^ [aBlock value] ensure: [modificationListeningDisabled := false].