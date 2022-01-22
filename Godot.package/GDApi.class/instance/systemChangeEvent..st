file synchronisation
systemChangeEvent: anEvent

	| class |
	scriptLoadingActive ifTrue: [^ self].
	
	class := anEvent itemKind = SystemChangeNotifier methodKind
		ifTrue: [anEvent item methodClass]
		ifFalse: [anEvent itemKind = SystemChangeNotifier classKind ifTrue: [anEvent item]].
	
	"TODO i mean what even is this"
	class ifNotNil: [
		((class inheritsFrom: GDScript)
			and: (anEvent itemKind = SystemChangeNotifier methodKind
				or: (anEvent class ~= AddedEvent and: anEvent class ~= RemovedEvent))) ifTrue: [self writeClassToFile: class]]