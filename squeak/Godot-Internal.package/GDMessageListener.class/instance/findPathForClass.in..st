file synchronization
findPathForClass: aClass in: aDirectory

	(aDirectory fileExists: '.gdignore') ifTrue: [^ nil].
	
	aDirectory fileEntries do: [:fileEntry |
		fileEntry extension = 'st' ifTrue: [
			((TonelParser parseStream: fileEntry readStream) first className = aClass name) ifTrue: [
				^ fileEntry fullName]
			]
		].
	
	aDirectory directoryEntries do: [:directoryEntry |
		(self findPathForClass: aClass in: directoryEntry asFileDirectory) ifNotNil: [:path | ^ path]
		].
	
	^ nil