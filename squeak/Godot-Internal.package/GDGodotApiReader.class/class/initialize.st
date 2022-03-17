sorting
initialize

	" GDGodotApiReader initialize "

	super initialize.

	ObjectCategories := Dictionary withAll: ({
		'GlobalConstants' -> 'Core'.
		'Object' -> 'Core'.
		'Node' -> 'Node'.
		'Node2D' -> '2D'.
		'Shape2D' -> '2D'.
		'Spatial' -> 'Spatial'.
		'Shape' -> 'Shape'.
		'Control' -> 'Control'.
		'VisualScriptNode' -> 'VisualScript'.
		'VisualShaderNode' -> 'VisualShader'.
		'Animation' -> 'Animation'.
		'AnimationNode' -> 'Animation'.
		'AudioBusLayout' -> 'Audio'.
		'AudioEffect' -> 'Audio'.
		'AudioStream' -> 'Audio'.
		'InputEvent' -> 'InputEvent'.
		'Mesh' -> 'Mesh'.
		'MeshLibrary' -> 'Mesh'.
		'Material' -> 'Material'.
		'Texture' -> 'Texture'.
		'TextureLayered' -> 'Texture'.
	} do: [:association |
		association key: (GDApiObjectsGenerator squeakClassName: association key) asSymbol.
		association value: (GDApiObjectsGenerator fullCategoryName: association value)])