as yet unclassified
source
	
	"TODO: lots of code duplication with GDGodotApiMethodSpec"
	^ '{1}{2}
	<generated>
	
	^ GDApi instance perform: #{3} withArguments: {4}' format: {self nameString. self methodArgumentString. self fullApiName. self argumentArrayString}