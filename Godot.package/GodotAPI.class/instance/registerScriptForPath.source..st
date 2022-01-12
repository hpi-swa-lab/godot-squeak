as yet unclassified
registerScriptForPath: aPath source: aSource
	| class |
	class := nil.
	"class := aSource compile class."
	scriptRegistry at: aPath put: class