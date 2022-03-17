instance creation
fromApiJsonObject: aJsonObject position: aNumber

	^ self new
		name: aJsonObject name;
		type: aJsonObject type;
		hasDefaultValue: aJsonObject has_default_value;
		suffix: aNumber asString