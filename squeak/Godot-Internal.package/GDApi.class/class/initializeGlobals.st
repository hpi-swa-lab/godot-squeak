as yet unclassified
initializeGlobals

	self singletons do: [:singletonName | | singleton gdObject |
		gdObject := (GDApi instance globalGetSingleton: singletonName).
		gdObject isNull
			ifTrue: [
				Transcript showln: singletonName, ' singleton not initialized because GDObject* is NULL']
			ifFalse: [
				singleton := GDBaseObject fromGDObject: gdObject.
				(Smalltalk at: (self singletonSymbol: singletonName) put: singleton)]].