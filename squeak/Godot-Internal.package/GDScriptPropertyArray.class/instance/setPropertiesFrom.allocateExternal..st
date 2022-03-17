as yet unclassified
setPropertiesFrom: aPropertySequence allocateExternal: aBoolean

	| properties |
	aBoolean ifFalse: [self notYetImplemented].
	
	self num: aPropertySequence size.
	properties := GDScriptProperty allocateExternal: self num.
	aPropertySequence withIndexDo: [:propertyDesc :index | | name property |
		property := properties at: index.
		name := (ExternalType char allocateExternal: propertyDesc first size + 1) toCString: propertyDesc first.
		property name: name.
		property defaultValue: GDVariant allocateExternal.
		propertyDesc second insertIntoGDVariant: property defaultValue].
	self properties: properties.