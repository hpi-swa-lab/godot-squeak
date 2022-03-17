instance creation
fromApiPropertiesJsonObject: aJsonObject

	^ self new
		name: '';
		type: (aJsonObject type copyReplaceAll: ',' with: 'Or') asLegalSelector;
		suffix: '1'