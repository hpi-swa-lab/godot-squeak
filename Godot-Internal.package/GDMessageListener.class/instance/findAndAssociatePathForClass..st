file synchronization
findAndAssociatePathForClass: aClass

	| projectRoot |
	projectRoot := FileDirectory on: GDApi instance getProjectRoot.
	(self findPathForClass: aClass in: projectRoot)
		ifNil: [self error: 'Did not find a path for ', aClass]
		ifNotNil: [:path | self associatePath: path withClass: aClass]