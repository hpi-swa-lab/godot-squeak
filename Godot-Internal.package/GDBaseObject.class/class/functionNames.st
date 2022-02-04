godot info
functionNames

	^ self selectors collect: [:selector | GDMessageListener squeakToGodotMethodName: selector]