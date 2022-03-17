generating
source

	^ '{1}
	<generated>
		
	^ self {2}' format: {self methodName. self getterName}