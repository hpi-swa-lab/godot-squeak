generating
determineDependencies: objectSpecifications

	topLevel := Set new.
	dependants := Dictionary new.
	
	objectSpecifications do: [:spec || className baseClassName |
		className := (spec at: #name) asSymbol.
		baseClassName := (spec at: #base_class) asSymbol.
		baseClassName isEmpty
			ifTrue: [
				topLevel add: className]
			ifFalse: [
				(dependants at: baseClassName ifAbsentPut: [Set new]) add: className]].