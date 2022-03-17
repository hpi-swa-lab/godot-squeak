method remapping
godotToSqueakMethodName: aString

	^ (aString copyReplaceAll: '_' with: ':') asSymbol