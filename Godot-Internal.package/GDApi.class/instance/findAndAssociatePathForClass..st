as yet unclassified
findAndAssociatePathForClass: aClass

	| projectRoot |
	projectRoot := FileDirectory on: self getProjectRoot.
	(self findPathForClass: aClass in: projectRoot)
		ifNil: [self error: 'Did not find a path for ', aClass]
		ifNotNil: [:path | self associatePath: path withClass: aClass]