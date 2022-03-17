as yet unclassified
nameFor: aCMethodName withoutPrefix: aPrefix

	| parts |
	parts := ((aCMethodName copyReplaceAll: aPrefix with: '') splitBy: '_') select: #notEmpty.
	^ parts first, ((parts allButFirst collect: #capitalized) join)