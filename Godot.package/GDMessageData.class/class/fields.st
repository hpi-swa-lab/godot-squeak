as yet unclassified
fields
	" self defineFields "

	^ #(
		(newScriptMessage GDMessageNewScript)
		(newInstanceMessage GDMessageNewInstance)
		(methodCallMessage GDMessageMethodCall)
	)