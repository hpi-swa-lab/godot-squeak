as yet unclassified
findAndAssociatePathForClass: aClass

	| projectRoot |
	projectRoot := FileDirectory on: self getProjectRoot.
	self associatePath: (self findPathForClass: aClass in: projectRoot) withClass: aClass.