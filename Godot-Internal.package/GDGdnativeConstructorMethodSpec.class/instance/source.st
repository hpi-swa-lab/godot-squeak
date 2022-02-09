as yet unclassified
source
	
	^ '{1}{2}
	<generated>
	
	| instance |
	instance := self allocateExternal.
	GDApi instance perform: #{3} withArguments: \{instance. {4}\}.
	^ instance' format: {self nameString. self methodArgumentString. self fullApiName. self argumentArrayString}