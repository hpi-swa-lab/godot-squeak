as yet unclassified
godotClass

	self isGodotClass ifTrue: [^ self].
	
	({self}, self allSuperclasses) do: [:class |
		class isGodotClass ifTrue: [^ class]].
		
	self error: 'Did not find Godot class'