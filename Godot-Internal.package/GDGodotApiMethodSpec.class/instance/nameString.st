generating
nameString

	| parts |
	parts := name subStrings: '_'.
	^ parts first, (parts allButFirst collect: #capitalized) join