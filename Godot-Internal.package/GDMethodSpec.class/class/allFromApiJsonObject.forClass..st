instance creation
allFromApiJsonObject: aJsonObject forClass: className

	| specs arguments methodName |
	specs := OrderedCollection new.
	methodName := aJsonObject name.
	arguments := (aJsonObject arguments withIndexCollect:
		[:arg :index | GDArgumentSpec fromApiJsonObject: arg position: index]) asOrderedCollection.
	
	[arguments notEmpty and: [arguments last hasDefaultValue]] whileTrue: [
		specs add: (self newNamed: methodName arguments: arguments copy className: className).
		arguments removeLast].
		
	^ specs add: (self newNamed: methodName arguments: arguments className: className); yourself