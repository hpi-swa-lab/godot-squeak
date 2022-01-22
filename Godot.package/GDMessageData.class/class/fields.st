field definition
fields
	" self defineFields "

	^ #(
		(newScriptMessage GDMessageNewScript)
		(scriptReloadMessage GDMessageScriptReload)
		(newInstanceMessage GDMessageNewInstance)
		(methodCallMessage GDMessageMethodCall)
	)