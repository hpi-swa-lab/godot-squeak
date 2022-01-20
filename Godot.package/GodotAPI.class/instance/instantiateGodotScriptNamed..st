as yet unclassified
instantiateGodotScriptNamed: aSymbol

	| class |
	GodotScript subclass: aSymbol
		instanceVariableNames: ''
		classVariableNames: ''
		poolDictionaries: ''
		category: 'Godot-Scripts'.
	
	class := Smalltalk classNamed: aSymbol.
	class compile: GodotAPI readyCode.
	class compile: GodotAPI processCode.
	^ class
	