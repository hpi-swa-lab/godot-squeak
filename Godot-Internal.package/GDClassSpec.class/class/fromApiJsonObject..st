instance creation
fromApiJsonObject: aJsonObject

	^ self new
		name: (GDApiObjectsGenerator squeakClassName: aJsonObject name) asSymbol;
		parentName: (aJsonObject base_class
			ifEmpty: [self rootClassName]
			ifNotEmpty: [:name | GDApiObjectsGenerator squeakClassName: name]);
		category: (GDApiObjectsGenerator fullCategoryName: 'Uncategorized')