as yet unclassified
setGlobalsToNil

	"TODO: instead of nil, an object that warns you that the global is not initialized when you try to use it could be better here"
	self singletons do: [:singletonName |
		(Smalltalk at: (self singletonSymbol: singletonName) put: nil)]