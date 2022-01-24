as yet unclassified
doWithoutModificationListening: aBlock

	modificationListeningDisabled := true.
	^ [aBlock value] ensure: [modificationListeningDisabled := false].