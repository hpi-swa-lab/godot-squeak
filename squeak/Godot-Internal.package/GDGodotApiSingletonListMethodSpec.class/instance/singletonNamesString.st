as yet unclassified
singletonNamesString

	^ (self singletonNames collect: [:name | String tab, String tab, '''', name, '''']) joinSeparatedBy: String cr