as yet unclassified
argumentArrayString
	
	^ '\{{1}\}' format: {(arguments collect: #argumentParamterName) joinSeparatedBy: '. '}
	"^ '\{{1}. {2}\}' format: {
		arguments first isSelf ifTrue: ['self'] ifFalse: [arguments first argumentParamterName].
		(arguments allButFirst collect: #argumentParamterName) joinSeparatedBy: '. '
	}"