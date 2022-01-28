file synchronization
systemChangeEvent: anEvent

	| class |
	modificationListeningDisabled ifTrue: [^ self].
	
	class := anEvent itemKind = SystemChangeNotifier methodKind
		ifTrue: [anEvent item methodClass]
		ifFalse: [anEvent itemKind = SystemChangeNotifier classKind ifTrue: [anEvent item]].
	
	"TODO i mean what even is this"
	class ifNotNil: [
		((pathToClass includes: class)
			and: ((anEvent itemKind = SystemChangeNotifier methodKind)
				or: (anEvent class ~= AddedEvent and: anEvent class ~= RemovedEvent))) ifTrue: [self writeClassToFile: class]]