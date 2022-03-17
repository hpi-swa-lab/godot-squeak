as yet unclassified
argumentArrayString
	
	^ (arguments collect: [:arg | | parameterName conversionCall |
			parameterName := arg argumentParamterName.
			conversionCall := arg typeIsGDVariant
				ifTrue: ['asGDVariant']
				ifFalse: [
					arg typeIsGDString
						ifTrue: ['asGDString']
						ifFalse: ['']].
			parameterName, ' ', conversionCall]) joinSeparatedBy: '. '