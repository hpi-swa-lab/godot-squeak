file synchronisation
systemChangeEvent: anEvent

	| class |
	class := anEvent itemKind = SystemChangeNotifier methodKind
		ifTrue: [anEvent item methodClass]
		ifFalse: [anEvent itemKind = SystemChangeNotifier classKind ifTrue: [anEvent item]].
	
	class ifNotNil: [
		((class inheritsFrom: GodotScript) and: anEvent class ~= AddedEvent) ifTrue: [self writeClassToFile: class]]