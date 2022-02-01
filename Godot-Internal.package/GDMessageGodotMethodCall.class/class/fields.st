fields
fields
	" self defineFields "
	^ #(
		(methodName 'GDString*')
		(self 'GDVariant*')
		(args 'void*')
		(argCount int)
		(error 'GDVariantCallError*')
		(result 'GDVariant*')
	)