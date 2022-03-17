instance creation
newVararg

	^ self new
		name: 'withArguments';
		type: 'sequence';
		isVararg: true;
		suffix: 'vararg'