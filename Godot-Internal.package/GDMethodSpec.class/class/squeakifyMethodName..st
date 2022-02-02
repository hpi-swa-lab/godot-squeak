as yet unclassified
squeakifyMethodName: aString

	| parts |
	parts := aString subStrings: '_'.
	^ parts first, (parts allButFirst collect: #capitalized) join