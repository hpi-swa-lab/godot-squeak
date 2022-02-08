instance creation
newInstanceOperatorNamed: methodName arguments: arguments className: className apiName: apiName

	| otherClass operatorSymbols operatorSymbol otherMethodName |
	
	operatorSymbols := Dictionary withAll: {
		'equal' -> '='.
		'less' -> '<'.
		'add' -> '+'.
		'plus' -> '+'.
		'subtract' -> '-'.
		'multiply' -> '*'.
		'divide' -> '/'.
	}.
	
	operatorSymbol := operatorSymbols at: methodName ifAbsent: [
		Transcript showln: 'Operator for ', methodName, ' nyi'.
		^ #()].
	otherClass := (self associationForCType: arguments second type) value.
	otherMethodName := methodName, otherClass.
	
	^ {
		self newInstanceMethodNamed: otherMethodName
		arguments: {(arguments first copy isSelf: false) name: ''. arguments second copy isSelf: true}
		category: self operatorCategory
		className: otherClass
		apiName: apiName.
	}