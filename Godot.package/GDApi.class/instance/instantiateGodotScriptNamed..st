as yet unclassified
instantiateGodotScriptNamed: aSymbol

	| class |
	class := GDScript subclass: aSymbol
		instanceVariableNames: ''
		classVariableNames: ''
		poolDictionaries: ''
		category: 'Godot-Scripts'.
	
	class compile: GDApi readyCode.
	class compile: GDApi processCode.
	
	^ class
	