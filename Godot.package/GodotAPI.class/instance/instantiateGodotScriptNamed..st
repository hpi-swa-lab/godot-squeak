as yet unclassified
instantiateGodotScriptNamed: name

	| class |
	GodotScript subclass: (name asSymbol)
		instanceVariableNames: ''
		classVariableNames: ''
		poolDictionaries: ''
		category: 'Godot-Scripts'.
	
	class := Smalltalk classNamed: name.
	class compile: self readyCode.
	class compile: self processCode.
	