as yet unclassified
call: methodName with: args

	Transcript showln: 'Calling ', methodName, 'on ', self class, ' with ', args.
	^ (GDApi instance callMethod: methodName onVariant: self internalVariant with: args) unwrap