instance creation
fromApiPropertiesJsonObject: aJsonObject

	^ self new
		name: '';
		type: (aJsonObject type copyReplaceAll: ',' with: 'Or') asLegalSelector;
		hasDefaultValue: false;
		position: 1