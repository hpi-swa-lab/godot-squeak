as yet unclassified
checkError

	self errorType ~= GDErrorVariantCall typeCallOk ifTrue: [
		(GDErrorVariantCall from: self) signal]