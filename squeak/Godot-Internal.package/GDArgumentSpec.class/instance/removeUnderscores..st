private
removeUnderscores: aString

	| parts |
	parts := aString splitBy: '_'.
	^ parts size = 1
		ifTrue: [aString]
		ifFalse: [{parts first withFirstCharacterDownshifted. parts allButFirst collect: #capitalized} flatten join].