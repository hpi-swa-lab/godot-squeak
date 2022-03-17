names
squeakIdentifierName: aString

	| parts |
	parts := aString subStrings: '_'.
	^ parts first, ((parts collect: #capitalized from: 2 to: parts size) join)