generating
argumentArrayString

	(arguments notEmpty and: [arguments last isVararg])
		ifTrue: [^ '\{\{{1}\}. {2}\} concatenation' format: {
			(arguments allButLast collect: #argumentParamterName) joinSeparatedBy: '. '.
			arguments last argumentParamterName}]
		ifFalse: [^ '\{{1}\}' format: {(arguments collect: #argumentParamterName) joinSeparatedBy: '. '}]
	