accessing
argumentParamterName

	| fixedType prefix |
	self isSelf ifTrue: [^ 'self'].
	
	fixedType := ((self removeUnderscores: type) capitalized).
	prefix := 'a', (fixedType first isVowel ifTrue: ['n'] ifFalse: ['']).
	^ prefix, fixedType, '_', suffix