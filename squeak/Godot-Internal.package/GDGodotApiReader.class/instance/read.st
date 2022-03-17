reading
read

	| contents classSpecs methodSpecs singletonClassSpecs |
	contents := Json readFrom: stream.
	
	classSpecs := OrderedCollection new.
	singletonClassSpecs := OrderedCollection new.
	methodSpecs := OrderedCollection new.
	
	contents do: [:classJsonObject | | classSpec |
		classSpec := classSpecs add: (GDClassSpec fromApiJsonObject: classJsonObject).
		classSpec isSingleton ifTrue: [singletonClassSpecs add: classSpec].
		
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
					(propertyJsonObject getter notEmpty
						and: (propertyJsonObject getter beginsWith: '_') not) ifTrue: [propertyMethods add: (
							GDGodotApiPropertyGetterSpec fromAPIJsonObject: propertyJsonObject className: classSpec name)].
					(propertyJsonObject setter notEmpty
						and: ((propertyJsonObject setter beginsWith: '_') not)) ifTrue: [propertyMethods add: (
							GDGodotApiPropertySetterSpec fromAPIJsonObject: propertyJsonObject className: classSpec name)].
					].
				propertyMethods])].
	
	^ {
		self class sortAndCategorizeClassSpecs: classSpecs.
		{methodSpecs. GDGodotApiSingletonListMethodSpec newFromClassSpecs: singletonClassSpecs} flatten
	}