godot info
properties

	^ (self methodDict values collect: [:method | method pragmasAt: #export:])
		select: [:pragmas |
			pragmas size > 1 ifTrue: [self error: 'Can''t handle multiple pragmas!'].
			pragmas notEmpty]
		thenCollect: [:pragmas |
			{pragmas first selector. Compiler evaluate: pragmas first arguments first, ' value'}]