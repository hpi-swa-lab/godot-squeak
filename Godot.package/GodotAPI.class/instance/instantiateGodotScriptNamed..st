as yet unclassified
instantiateGodotScriptNamed: aSymbol

	| class |
	GodotScript subclass: aSymbol
		instanceVariableNames: ''
		classVariableNames: ''
		poolDictionaries: ''
		category: 'Godot-Scripts'.
	
	class := Smalltalk classNamed: aSymbol.
	class compile: self readyCode.
	class compile: self processCode.
	^ class
	