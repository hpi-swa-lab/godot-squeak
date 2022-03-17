message handling
handleGetProperty: getPropertyData
	
	GDApi showln: 'get property', getPropertyData propertyName fromCString.
	[ | value |
	value := (self instanceFor: getPropertyData owner) perform: getPropertyData propertyName fromCString asSymbol.
	value insertIntoGDVariant: getPropertyData out]
		on: MessageNotUnderstood "TODO: needs to be more specific"
		do: [^ (ExternalType bool allocateExternal) value: false].
	
	^ (ExternalType bool allocateExternal) value: true.