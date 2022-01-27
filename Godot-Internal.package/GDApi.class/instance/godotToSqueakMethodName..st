as yet unclassified
godotToSqueakMethodName: aString

	^ (aString copyReplaceAll: '_' with: ':') asSymbol