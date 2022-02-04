message handling
handleSetProperty: setPropertyData

	GDApi showln: 'set property', setPropertyData propertyName fromCString.
	[(self instanceFor: setPropertyData owner)
		perform: setPropertyData propertyName fromCString asSimpleSetter
		with: setPropertyData value unwrap]
		on: MessageNotUnderstood "TODO: needs to be more specific"
		do: [^ (ExternalType bool allocateExternal) value: false].
	GDApi showln: 'set to', ((instances at: setPropertyData owner) perform: setPropertyData propertyName fromCString asSymbol).
	
	^ (ExternalType bool allocateExternal) value: true.