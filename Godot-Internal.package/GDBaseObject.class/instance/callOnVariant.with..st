private
callOnVariant: methodName with: args

	GDApi showln: ('Calling {1} on {2} with {3}' format: {methodName. self class. args}).
	^ (self class callMethod: methodName onVariant: self internalVariant with: args) unwrap