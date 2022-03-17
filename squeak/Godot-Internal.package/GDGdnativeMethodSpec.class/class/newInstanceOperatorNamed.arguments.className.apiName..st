instance creation
newInstanceOperatorNamed: methodName arguments: arguments className: className apiName: apiName

	| otherClass operatorSymbols operatorSymbol otherCategory operatorName otherMethodName |
	
	operatorSymbols := Dictionary withAll: {
		'equal' -> '='.
		'less' -> '<'.
		'add' -> '+'.
		'plus' -> '+'.
		'subtract' -> '-'.
		'multiply' -> '*'.
		'divide' -> '/'.
	}.
	
	operatorName := (methodName piecesCutWhere: [:a :b | a isLowercase and: b isUppercase]) first.
	operatorSymbol := operatorSymbols at: operatorName ifAbsent: [
		Transcript showln: 'Operator for ', methodName, ' nyi'.
		^ #()].
	otherClass := (self associationForCType: arguments second type) value.
	otherCategory := (Smalltalk classNamed: otherClass) category = 'Godot-Builtins'
		ifTrue: [self privateCategory]
		ifFalse: [self extensionCategory].
	otherMethodName := operatorName, className.
	
	^ {
		GDGdnativeInstanceMethodSpec newInstanceMethodNamed: otherMethodName
		arguments: {(arguments first copy isSelf: false) name: ''. arguments second copy isSelf: true}
		category: otherCategory
		className: otherClass
		apiName: apiName
		returnsVariant: false "TODO: the proper way would be to look at the return type".
		
		(GDGdnativeOperatorMethodSpec
			newNamed: operatorSymbol
			arguments: {arguments second}
			category: self operatorCategory
			className: className)
			otherMethodName: otherMethodName
	}