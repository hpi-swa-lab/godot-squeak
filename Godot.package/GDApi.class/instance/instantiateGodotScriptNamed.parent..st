as yet unclassified
instantiateGodotScriptNamed: name parent: parentName

	^ self doWithoutModificationListening: [ | class parentClass |
		parentClass := Smalltalk classNamed: (GDApiObjectsGenerator squeakClassName: parentName).
		class := parentClass subclass: name asSymbol
			instanceVariableNames: ''
			classVariableNames: ''
			poolDictionaries: ''
			category: 'Godot-Scripts'.
	
		class compile: GDApi readyCode.
		class compile: GDApi processCode.
	
		class]
	