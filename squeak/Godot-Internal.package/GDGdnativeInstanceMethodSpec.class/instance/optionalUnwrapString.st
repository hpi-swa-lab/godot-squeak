as yet unclassified
optionalUnwrapString

	^ self returnsVariant
		ifTrue: ['unwrap']
		ifFalse: ['']