using
deleteGdnativeFunctions

	{GDGdnativeMethodSpec methodCategory.
	GDGdnativeMethodSpec propertyCategory.
	GDGdnativeMethodSpec operatorCategory.
	GDGdnativeMethodSpec constructorCategory.
	GDGdnativeMethodSpec privateCategory} do: [:category |
		(Smalltalk environment organization listAtCategoryNamed: #'Godot-Builtins')
			do: [:className | | builtinClass |
				builtinClass := (Smalltalk classNamed: className).
				builtinClass removeCategory: category.
				builtinClass class removeCategory: category]].
		
	Smalltalk allClasses do: [:class |
		class removeCategory: GDGdnativeMethodSpec extensionCategory]