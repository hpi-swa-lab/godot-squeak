reading
read

	| contents classSpecs methodSpecs |
	contents := Json readFrom: stream.
	
	classSpecs := OrderedCollection new.
	methodSpecs := OrderedCollection new.
	
	contents do: [:classJsonObject | | classSpec |
		classSpec := classSpecs add: (GDClassSpec fromApiJsonObject: classJsonObject).
		
		methodSpecs addAll: (classJsonObject methods
			select: [:methodJsonObject |
				(methodJsonObject name beginsWith: '_') not]
			thenCollect: [:methodJsonObject |
				GDGodotApiMethodSpec allFromApiJsonObject: methodJsonObject forClass: classSpec name]).
			
		methodSpecs addAll: (classJsonObject properties
			select: [:propertyJsonObject |
				(propertyJsonObject name beginsWith: '_') not]
			thenCollect: [:propertyJsonObject | | propertyMethods |
				propertyMethods := OrderedCollection new.
				(propertyJsonObject name includes: $/) ifFalse: [
					propertyJsonObject getter ifNotEmpty: [propertyMethods add: (
							GDGodotApiPropertyGetterSpec fromAPIJsonObject: propertyJsonObject className: classSpec name)].
					propertyJsonObject setter ifNotEmpty: [propertyMethods add: (
							GDGodotApiPropertySetterSpec fromAPIJsonObject: propertyJsonObject className: classSpec name)].
					].
				propertyMethods])].
	
	^ {self class sortAndCategorizeClassSpecs: classSpecs. methodSpecs flatten}