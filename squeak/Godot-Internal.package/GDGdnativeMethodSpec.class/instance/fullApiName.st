as yet unclassified
fullApiName

	arguments
		ifEmpty: [^ apiName]
		"TODO: is there a better way to repeat a string x times?"
		ifNotEmpty: [^ apiName, ':', ((OrderedCollection new add: 'with:' withOccurrences: arguments size - 1; yourself) join)]