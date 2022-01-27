as yet unclassified
getNode: node property: property

	^ 'Godot says that the property {1} of the node named {2} has the value {3}'
		format: {property. node getName. node getProperty: property}