as yet unclassified
associationForCType: aString

	CToSqueakNames associationsDo: [:association |
		(aString beginsWith: association key) ifTrue: [^ association]].
	^ nil