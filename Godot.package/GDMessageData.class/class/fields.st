as yet unclassified
fields
	" self defineFields "

	^ #(
		(newScriptMessage GDMessageNewScript)
		(scriptReloadMessage GDMessageScriptReload)
		(newInstanceMessage GDMessageNewInstance)
		(methodCallMessage GDMessageMethodCall)
	)