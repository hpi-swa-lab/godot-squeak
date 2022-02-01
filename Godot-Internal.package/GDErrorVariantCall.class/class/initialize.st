class initialization
initialize

	super initialize.

	TypeNames := Dictionary withAll: {
		self typeCallOk -> 'call ok'.
		self typeInvalidMethod -> 'invalid method'.
		self typeInvalidArgument -> 'invalid argument'.
		self typeTooManyArguments -> 'too many arguments'.
		self typeTooFewArguments -> 'too few arguments'.
		self typeInstanceIsNull -> 'instance is null'.
	}