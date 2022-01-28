file synchronization
associatePath: aPath withClass: aClass

	pathToClass keysAndValuesRemove: [:key :value | value = aClass].
	pathToClass at: aPath put: aClass