reading
read

	| contents classSpecs methodSpecs |
	contents := Json readFrom: stream.
	
	classSpecs := OrderedCollection new.
	methodSpecs := OrderedCollection new.
	
	contents do: [:classJsonObject | | classSpec |
		classSpec := classSpecs add: (GDClassSpec fromApiJsonObject: classJsonObject).
		methodSpecs addAll: (classJsonObject methods
			select: [:methodJsonObject | (methodJsonObject name beginsWith: '_') not]
			thenCollect: [:methodJsonObject |
				GDGodotApiMethodSpec allFromApiJsonObject: methodJsonObject forClass: classSpec name])
		].
	
	^ {self class sortAndCategorizeClassSpecs: classSpecs. methodSpecs flatten}