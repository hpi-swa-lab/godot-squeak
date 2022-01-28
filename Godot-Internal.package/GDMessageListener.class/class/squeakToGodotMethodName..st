method remapping
squeakToGodotMethodName: aString

	^ aString copyReplaceAll: ':' with: '_'