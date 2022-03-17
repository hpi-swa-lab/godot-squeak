as yet unclassified
read

	| specs apiSpec |
	apiSpec := (Json readFrom: stream) core.
	specs := OrderedCollection new.
	
	[apiSpec] whileNotNil: [
		specs add: (apiSpec api collect: [:methodJsonObject |
			GDGdnativeMethodSpec allFromAPIJsonObject: methodJsonObject]).
		apiSpec := apiSpec next].
	
	^ {{}. specs flatten}