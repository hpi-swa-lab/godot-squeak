private
callOnVariant: methodName with: args

	GDApi showln: 'Calling ', methodName, 'on ', self class, ' with ', args.
	^ (GDApi instance callMethod: methodName onVariant: self internalVariant with: args) unwrap