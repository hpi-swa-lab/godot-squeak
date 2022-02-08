class initialization
initialize

	| squeakClassNames |
	super initialize.
	
	BuiltinClassNames := {
		'GDAABB'.
		'GDArray'.
		'GDBasis'.
		'GDBool'.
		'GDColor'.
		'GDDictionary'.
		'GDInt'.
		'GDNodePath'.
		'GDObject'.
		'GDPlane'.
		'GDPoolByteArray'.
		'GDPoolColorArray'.
		'GDPoolIntArray'.
		'GDPoolRealArray'.
		'GDPoolStringArray'.
		'GDPoolVector2Array'.
		'GDPoolVector3Array'.
		'GDQuat'.
		'GDReal'.
		'GDRect2'.
		'GDRid'.
		'GDTransform2D'.
		'GDTransform'.
		'GDVector2'.
		'GDVector3'
	}.
	
	squeakClassNames := BuiltinClassNames asOrderedCollection addAll: {
		'GDVariant'
	}; yourself.
	
	CToSqueakNames := squeakClassNames collect: [:class |
		'godot_', (((class allButFirst: 2) piecesCutWhere: [:left :right |
			(left isLowercase or: left isDigit) and: (right isUppercase and: right ~= $D)])
			joinSeparatedBy: '_')
			asLowercase -> class].