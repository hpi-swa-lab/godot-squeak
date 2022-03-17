as yet unclassified
checkError

	(GDErrorVariantCall from: self) ifNotNil: [:error | error signal]