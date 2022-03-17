instance creation
newNamed: name arguments: argumentSpecs category: category className: className

	^ self new
		name: name;
		arguments: argumentSpecs;
		methodCategory: category;
		className: className