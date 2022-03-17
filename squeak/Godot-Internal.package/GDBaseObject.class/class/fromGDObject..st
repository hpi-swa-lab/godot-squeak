as yet unclassified
fromGDObject: aGDObject

	| godotObject actualClass |
	godotObject := GodotObject newWithEmptyVariant.
	GDApi instance variantNewObject: godotObject internalVariant with: aGDObject.
	actualClass := (Smalltalk classNamed:
		(GDApiObjectsGenerator squeakClassName: godotObject getClass)).
	^ godotObject as: actualClass
		