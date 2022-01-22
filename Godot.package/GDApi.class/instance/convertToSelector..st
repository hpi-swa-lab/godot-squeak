as yet unclassified
convertToSelector: aString

	^ (aString copyReplaceAll: '_' with: ':') asSymbol