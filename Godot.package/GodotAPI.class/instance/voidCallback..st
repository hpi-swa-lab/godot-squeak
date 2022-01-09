as yet unclassified
voidCallback: aBlock
	<callback: void (*)()>
	
	| signature |
	self assert: aBlock numArgs = 0.
	signature := ((thisContext method pragmaAt: #callback:) argumentAt: 1) copy.
	^ aBlock signature: signature