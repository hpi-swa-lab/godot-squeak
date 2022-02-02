generating
source

	^ '{1}{2}
	<generated>
		
	^ self {3}: {4}' format: {
		self methodName.
		arguments first methodArgumentString.
		self setterName.
		arguments first argumentParamterName
	}