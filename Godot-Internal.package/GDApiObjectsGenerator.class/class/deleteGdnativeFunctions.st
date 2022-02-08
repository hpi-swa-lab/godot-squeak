using
deleteGdnativeFunctions

	{GDGdnativeMethodSpec methodCategory.
	GDGdnativeMethodSpec propertyCategory.
	GDGdnativeMethodSpec operatorCategory} do: [:category |
		(Smalltalk environment organization listAtCategoryNamed: #'Godot-Builtins')
			do: [:className |
				(Smalltalk classNamed: className) removeCategory: category]]