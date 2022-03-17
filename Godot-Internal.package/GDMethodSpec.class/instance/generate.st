generating
generate

	| method selector source |
	source := self source.
	selector := self methodClass
		compileSilently: self source
		classified: methodCategory.
	method := self methodClass compiledMethodAt: selector.
	method becomeForward: (method copyWithSourceCode: self source)