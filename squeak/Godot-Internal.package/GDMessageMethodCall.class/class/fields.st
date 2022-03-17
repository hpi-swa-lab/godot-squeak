fields
fields
	" self defineFields "
	^ #(
		(methodName 'char*')
		(owner 'GDObject*')
		(args 'void*')
		(argCount int)
		(error 'GDVariantCallError*')
		(result 'GDVariant*')
	)